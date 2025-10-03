/* polyscriptc.c
   Prototype Polyscript compiler
   Build: gcc -std=c99 -O2 -o polyscriptc polyscriptc.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

/* ----------------------------
   Simple dynamic string helpers
   ---------------------------- */
static char *strdup_safe(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *r = malloc(n);
    if (!r) { perror("malloc"); exit(1); }
    memcpy(r, s, n);
    return r;
}
static char *concat3(const char *a, const char *b, const char *c) {
    size_t na = strlen(a), nb = strlen(b), nc = strlen(c);
    char *r = malloc(na+nb+nc+1);
    memcpy(r, a, na);
    memcpy(r+na, b, nb);
    memcpy(r+na+nb, c, nc);
    r[na+nb+nc] = 0;
    return r;
}

/* ----------------------------
   FNV-1a hash for strings
   ---------------------------- */
static uint64_t fnv1a(const char *s) {
    uint64_t h = 14695981039346656037ULL;
    while (*s) { h ^= (unsigned char)*s++; h *= 1099511628211ULL; }
    return h;
}

/* ----------------------------
   Minimal open-addressing hashmap for char* -> void*
   (power of two capacity, linear probing)
   ---------------------------- */
typedef struct {
    char *key; void *val; uint64_t hash;
} HEntry;
typedef struct {
    size_t cap; size_t count; HEntry *entries;
} HashMap;

static HashMap *hm_create(size_t initial) {
    HashMap *m = malloc(sizeof(*m));
    if (!m) exit(1);
    size_t cap = 8;
    while (cap < initial) cap <<= 1;
    m->cap = cap;
    m->count = 0;
    m->entries = calloc(cap, sizeof(HEntry));
    return m;
}
static void hm_free(HashMap *m) {
    if (!m) return;
    for (size_t i=0;i<m->cap;i++) {
        if (m->entries[i].key) free(m->entries[i].key);
    }
    free(m->entries);
    free(m);
}
static void hm_resize(HashMap *m, size_t newcap) {
    HEntry *old = m->entries; size_t oldcap = m->cap;
    HEntry *entries = calloc(newcap, sizeof(HEntry));
    m->entries = entries; m->cap = newcap; m->count = 0;
    for (size_t i=0;i<oldcap;i++) {
        if (!old[i].key) continue;
        size_t idx = (old[i].hash) & (newcap - 1);
        while (entries[idx].key) idx = (idx + 1) & (newcap - 1);
        entries[idx] = old[i]; // move
        m->count++;
    }
    free(old);
}
static void hm_put(HashMap *m, const char *key, void *val) {
    if (m->count * 2 >= m->cap) hm_resize(m, m->cap * 2);
    uint64_t h = fnv1a(key);
    size_t idx = h & (m->cap - 1);
    while (m->entries[idx].key) {
        if (m->entries[idx].hash == h && strcmp(m->entries[idx].key, key) == 0) {
            m->entries[idx].val = val;
            return;
        }
        idx = (idx + 1) & (m->cap - 1);
    }
    m->entries[idx].key = strdup_safe(key);
    m->entries[idx].val = val;
    m->entries[idx].hash = h;
    m->count++;
}
static void *hm_get(HashMap *m, const char *key) {
    if (!m) return NULL;
    uint64_t h = fnv1a(key);
    size_t idx = h & (m->cap - 1);
    size_t start = idx;
    while (m->entries[idx].key) {
        if (m->entries[idx].hash == h && strcmp(m->entries[idx].key, key) == 0)
            return m->entries[idx].val;
        idx = (idx + 1) & (m->cap - 1);
        if (idx == start) break;
    }
    return NULL;
}

/* ----------------------------
   Value (literal) representation
   ---------------------------- */
typedef enum { VAL_STRING, VAL_NUMBER, VAL_BOOL } ValKind;
typedef struct {
    ValKind kind;
    char *s; double n; int b;
} Val;
static Val *val_new_string(const char *s) { Val *v = malloc(sizeof(*v)); v->kind = VAL_STRING; v->s = strdup_safe(s); return v; }
static Val *val_new_number(double n) { Val *v = malloc(sizeof(*v)); v->kind = VAL_NUMBER; v->n = n; return v; }
static Val *val_new_bool(int b) { Val *v = malloc(sizeof(*v)); v->kind = VAL_BOOL; v->b = b; return v; }

/* ----------------------------
   AST Node (minimal)
   ---------------------------- */
typedef struct ASTNode {
    char *kind;       // e.g. "new"
    char *name;       // e.g. "window"
    HashMap *props;   // key -> Val*
    struct ASTNode **children;
    size_t child_count;
} ASTNode;
static ASTNode *astnode_new(const char *kind, const char *name) {
    ASTNode *n = malloc(sizeof(*n));
    n->kind = strdup_safe(kind); n->name = strdup_safe(name);
    n->props = hm_create(8);
    n->children = NULL; n->child_count = 0;
    return n;
}
static void astnode_free(ASTNode *n) {
    if (!n) return;
    free(n->kind); free(n->name);
    // free props
    for (size_t i=0;i<n->props->cap;i++){
        if (n->props->entries[i].key) {
            free(n->props->entries[i].key);
            Val *v = (Val*)n->props->entries[i].val;
            if (v) {
                if (v->kind == VAL_STRING) free(v->s);
                free(v);
            }
        }
    }
    free(n->props->entries); free(n->props);
    free(n->children);
    free(n);
}

/* ----------------------------
   Lexer (tiny)
   ---------------------------- */
typedef enum {
    T_EOF, T_IDENT, T_STRING, T_NUMBER,
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE, T_COLON, T_SEMI,
    T_NEW, T_TRUE, T_FALSE
} TokenKind;
typedef struct { TokenKind kind; char *lex; } Token;

typedef struct { const char *s; size_t i; Token cur; } Lexer;
static void lex_init(Lexer *L, const char *s) { L->s = s; L->i = 0; L->cur.kind = T_EOF; L->cur.lex = NULL; }
static void lex_skip_ws(Lexer *L) {
    const char *s = L->s;
    while (s[L->i]) {
        if (isspace((unsigned char)s[L->i])) { L->i++; continue; }
        // skip comments /* ... */
        if (s[L->i]=='/' && s[L->i+1]=='*') {
            L->i+=2;
            while (s[L->i] && !(s[L->i]=='*' && s[L->i+1]=='/')) L->i++;
            if (s[L->i]) L->i+=2;
            continue;
        }
        break;
    }
}
static Token lex_make(TokenKind k, const char *start, size_t len) {
    Token t; t.kind = k; t.lex = malloc(len+1); memcpy(t.lex, start, len); t.lex[len]=0; return t;
}
static void lex_next(Lexer *L) {
    lex_skip_ws(L);
    const char *s = L->s; size_t i = L->i;
    if (!s[i]) { L->cur = lex_make(T_EOF, "", 0); return; }
    char c = s[i];
    if (isalpha((unsigned char)c) || c=='_') {
        size_t st = i;
        while (isalnum((unsigned char)s[i]) || s[i]=='_' || s[i]=='-') i++;
        size_t len = i-st;
        char *lex = malloc(len+1); memcpy(lex, s+st, len); lex[len]=0;
        if (strcmp(lex,"new")==0) { free(lex); L->cur = lex_make(T_NEW, "new", 3); }
        else if (strcmp(lex,"true")==0) { free(lex); L->cur = lex_make(T_TRUE, "true", 4); }
        else if (strcmp(lex,"false")==0) { free(lex); L->cur = lex_make(T_FALSE, "false", 5); }
        else { L->cur.lex = lex; L->cur.kind = T_IDENT; }
        L->i = i; return;
    }
    if (c=='"') {
        i++; size_t st = i;
        while (s[i] && s[i] != '"') {
            if (s[i] == '\\' && s[i+1]) i+=2; else i++;
        }
        size_t len = i - st;
        L->cur = lex_make(T_STRING, s+st, len);
        if (s[i]=='"') i++;
        L->i = i; return;
    }
    if (isdigit((unsigned char)c) || (c=='-' && isdigit((unsigned char)s[i+1]))) {
        size_t st = i; if (s[i]=='-') i++;
        while (isdigit((unsigned char)s[i]) || s[i]=='.') i++;
        L->cur = lex_make(T_NUMBER, s+st, i-st);
        L->i = i; return;
    }
    // single char tokens
    L->i++;
    switch (c) {
        case '(' : L->cur = lex_make(T_LPAREN, "(",1); return;
        case ')' : L->cur = lex_make(T_RPAREN, ")",1); return;
        case '{' : L->cur = lex_make(T_LBRACE, "{",1); return;
        case '}' : L->cur = lex_make(T_RBRACE, "}",1); return;
        case ':' : L->cur = lex_make(T_COLON, ":",1); return;
        case ';' : L->cur = lex_make(T_SEMI, ";",1); return;
    }
    // unknown -> treat as ident of one char
    L->cur = lex_make(T_IDENT, s+i-1,1);
}

/* ----------------------------
   Parser (tiny, just enough for new window(...){...})
   ---------------------------- */
static int accept(Lexer *L, TokenKind k) {
    if (L->cur.kind == k) { lex_next(L); return 1; }
    return 0;
}
static void expect(Lexer *L, TokenKind k, const char *msg) {
    if (L->cur.kind == k) { lex_next(L); return; }
    fprintf(stderr, "Parse error: expected %s but got token kind %d (lex=%s)\n", msg, (int)L->cur.kind, L->cur.lex?L->cur.lex:"(null)");
    exit(1);
}
static ASTNode *parse_new(Lexer *L) {
    // we've consumed 'new' already
    if (L->cur.kind != T_IDENT) { fprintf(stderr,"Expected identifier after new\n"); exit(1); }
    char *name = strdup_safe(L->cur.lex); lex_next(L);
    ASTNode *node = astnode_new("new", name);
    if (accept(L, T_LPAREN)) {
        // parse props: ident : value ; ...
        while (L->cur.kind != T_RPAREN && L->cur.kind != T_EOF) {
            if (L->cur.kind == T_IDENT) {
                char *k = strdup_safe(L->cur.lex); lex_next(L);
                expect(L, T_COLON, "':' after property name");
                // value
                Val *v = NULL;
                if (L->cur.kind == T_STRING) { v = val_new_string(L->cur.lex); lex_next(L); }
                else if (L->cur.kind == T_NUMBER) { v = val_new_number(atof(L->cur.lex)); lex_next(L); }
                else if (L->cur.kind == T_TRUE) { v = val_new_bool(1); lex_next(L); }
                else if (L->cur.kind == T_FALSE) { v = val_new_bool(0); lex_next(L); }
                else if (L->cur.kind == T_IDENT) { v = val_new_string(L->cur.lex); lex_next(L); }
                else { fprintf(stderr,"Unexpected value token in property\n"); exit(1); }
                hm_put(node->props, k, v);
                free(k);
                accept(L, T_SEMI); // optional
            } else { /* skip unknowns */ lex_next(L); }
        }
        expect(L, T_RPAREN, ")"); // consume
    }
    if (accept(L, T_LBRACE)) {
        // body is ignored in prototype; skip until matching }
        // (could parse nested constructs here)
        while (L->cur.kind != T_RBRACE && L->cur.kind != T_EOF) lex_next(L);
        expect(L, T_RBRACE, "}");
    }
    return node;
}

/* ----------------------------
   Utilities: replace all occurrences of placeholder in text
   ---------------------------- */
static char *str_replace_all(const char *text, const char *pat, const char *rep) {
    if (!text || !pat) return strdup_safe(text?text:"");
    size_t tlen = strlen(text), plen = strlen(pat), rlen = strlen(rep);
    if (plen == 0) return strdup_safe(text);
    // count matches
    size_t count = 0;
    for (const char *p = text; (p = strstr(p, pat)); p += plen) count++;
    size_t newlen = tlen + count * (rlen - plen);
    char *out = malloc(newlen + 1);
    char *dst = out;
    const char *p = text;
    while (1) {
        const char *m = strstr(p, pat);
        if (!m) { strcpy(dst, p); break; }
        size_t chunk = (size_t)(m - p);
        memcpy(dst, p, chunk); dst += chunk;
        memcpy(dst, rep, rlen); dst += rlen;
        p = m + plen;
    }
    return out;
}

/* ----------------------------
   sanitize class/identifier for target languages
   e.g. "Control Tower" -> "ControlTower"
   ---------------------------- */
static char *sanitize_classname(const char *in) {
    char *out = malloc(strlen(in)+1);
    size_t j=0;
    int upper_next = 1;
    for (size_t i=0;i<strlen(in);i++) {
        char c = in[i];
        if (!isalnum((unsigned char)c)) { upper_next = 1; continue; }
        if (upper_next) { out[j++] = toupper((unsigned char)c); upper_next = 0; }
        else out[j++] = c;
    }
    out[j] = 0;
    if (j==0) { free(out); return strdup_safe("App"); }
    return out;
}

/* ----------------------------
   Built-in templates for "window" equivalent per target.
   These are intentionally simple and approximate your example output.
   ---------------------------- */

/* Windows (XAML + C#) templates */
static const char *tmpl_win_appxaml =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<Application x:Class=\"%s.App\"\n"
"             xmlns=\"http://schemas.microsoft.com/winfx/2006/xaml/presentation\"\n"
"             xmlns:x=\"http://schemas.microsoft.com/winfx/2006/xaml\"\n" 
"             StartupUri=\"MainWindow.xaml\">\n"
"    <Application.Resources>\n"
"        <!-- Define global resources (themes, styles, etc.) here -->\n"
"    </Application.Resources>\n"
"</Application>\n";

static const char *tmpl_win_mainwindow_xaml =
"<Window x:Class=\"%s.MainWindow\"\n"
"        xmlns=\"http://schemas.microsoft.com/winfx/2006/xaml/presentation\"\n"
"        xmlns:x=\"http://schemas.microsoft.com/winfx/2006/xaml\"\n"
"        Title=\"%s\"\n"
"        %s\n"
"        Background=\"%s\">\n"
"    <Grid/>\n"
"</Window>\n";

static const char *tmpl_win_mainwindow_cs =
"using System;\nusing System.Windows;\n\nnamespace %s\n{\n    public partial class MainWindow : Window\n    {\n        public MainWindow()\n        {\n            InitializeComponent();\n\n            // Ensure fullscreen covers all taskbars/overlays safely\n            this.Loaded += (s, e) =>\n            {\n                var screenWidth = SystemParameters.PrimaryScreenWidth;\n                var screenHeight = SystemParameters.PrimaryScreenHeight;\n                this.Left = 0;\n                this.Top = 0;\n                this.Width = screenWidth;\n                this.Height = screenHeight;\n            };\n\n            // Security-conscious defaults:\n            // - No external resources loaded automatically\n            // - No unsafe interop unless explicitly defined later\n            // - No reflection or dynamic loading\n        }\n    }\n}\n";

/* macOS (Swift) template */
static const char *tmpl_mac_main =
"import Cocoa\n\n@main\nclass AppDelegate: NSObject, NSApplicationDelegate {\n    var window: NSWindow!\n\n    func applicationDidFinishLaunching(_ notification: Notification) {\n        guard let screen = NSScreen.main else { fatalError(\"No primary screen available.\") }\n        let screenRect = screen.frame\n\n        window = NSWindow(\n            contentRect: screenRect,\n            styleMask: [.titled, .closable, .miniaturizable],\n            backing: .buffered,\n            defer: false\n        )\n        window.title = \"%s\"\n        window.center()\n        window.setFrame(screenRect, display: true)\n        window.makeKeyAndOrderFront(nil)\n        window.level = .normal\n\n        window.isReleasedWhenClosed = false\n        window.isOpaque = true\n        window.hasShadow = false\n        window.collectionBehavior = [.fullScreenPrimary]\n        window.backgroundColor = .%s\n    }\n\n    func applicationShouldTerminateAfterLastWindowClosed(_ sender: NSApplication) -> Bool { return true }\n}\n";

/* Linux/Qt (C++) template */
static const char *tmpl_qt_maincpp =
"#include <QApplication>\n#include <QMainWindow>\n#include <QScreen>\n\nclass %sWindow : public QMainWindow {\npublic:\n    %sWindow() {\n        setWindowTitle(\"%s\");\n        QScreen *screen = QGuiApplication::primaryScreen();\n        QRect screenGeometry = screen->geometry();\n        setGeometry(screenGeometry);\n        showFullScreen();\n    }\n};\n\nint main(int argc, char *argv[]) {\n    QApplication app(argc, argv);\n    app.setOrganizationName(\"SecureOrg\");\n    app.setApplicationName(\"%s\");\n    %sWindow window;\n    return app.exec();\n}\n";

/* Web templates */
static const char *tmpl_web_index =
"<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n  <meta charset=\"UTF-8\">\n  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n  <meta name=\"description\" content=\"%s Web App\">\n  <meta name=\"author\" content=\"Generated\">\n  <title>%s</title>\n  <link rel=\"icon\" href=\"/favicon.ico\" sizes=\"any\">\n  <style>*,*::before,*::after{box-sizing:border-box;margin:0;padding:0;}html,body{height:100%;font-family:system-ui, -apple-system, BlinkMacSystemFont, \"Segoe UI\", Roboto, sans-serif;background-color:%s;color:#000;}</style>\n  <script type=\"module\" src=\"/main.js\" defer></script>\n</head>\n<body>\n</body>\n</html>\n";

static const char *tmpl_web_mainjs =
"document.addEventListener(\"DOMContentLoaded\", () => { console.log(\"%s Web App initialized (vanilla mode).\") });\n";

/* ----------------------------
   Simple generation for the "window" construct
   ---------------------------- */
static char *gen_windows_from_node(ASTNode *node, const char *outdir) {
    // Title
    Val *titleV = (Val*)hm_get(node->props, "title");
    char *title = titleV && titleV->kind==VAL_STRING ? titleV->s : strdup_safe(node->name);
    // fullscreen?
    Val *fsV = (Val*)hm_get(node->props, "fullscreen");
    int fullscreen = fsV && fsV->kind==VAL_BOOL ? fsV->b : 0;
    // position
    Val *posV = (Val*)hm_get(node->props, "position");
    char *position = posV && posV->kind==VAL_STRING ? posV->s : "center";
    // background color (default black)
    const char *bg = "Black";
    // classname
    char *classname = sanitize_classname(title);
    // Build strings
    // App.xaml
    size_t appsize = strlen(tmpl_win_appxaml) + strlen(classname) + 20;
    char *appx = malloc(appsize);
    snprintf(appx, appsize, tmpl_win_appxaml, classname);
    // MainWindow.xaml
    char winprops[512] = "";
    if (fullscreen) {
        strcat(winprops, "WindowStartupLocation=\"CenterScreen\"\n        WindowState=\"Maximized\"\n        WindowStyle=\"None\"\n        ResizeMode=\"NoResize\"");
    } else {
        if (strcmp(position,"center")==0) strcat(winprops, "WindowStartupLocation=\"CenterScreen\"");
    }
    size_t mainxsize = strlen(tmpl_win_mainwindow_xaml) + strlen(classname) + strlen(title) + strlen(winprops) + 32;
    char *mainx = malloc(mainxsize);
    snprintf(mainx, mainxsize, tmpl_win_mainwindow_xaml, classname, title, winprops, bg);
    // MainWindow.cs
    size_t cslen = strlen(tmpl_win_mainwindow_cs) + strlen(classname) + 10;
    char *cs = malloc(cslen);
    snprintf(cs, cslen, tmpl_win_mainwindow_cs, classname);
    // write files
    char dir[512]; snprintf(dir, sizeof(dir), "%s/%s-windows", outdir, classname);
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "mkdir -p \"%s\"", dir); system(cmd);
    char fpath[1024];
    snprintf(fpath, sizeof(fpath), "%s/App.xaml", dir);
    FILE *f = fopen(fpath,"w"); fprintf(f,"%s",appx); fclose(f);
    snprintf(fpath, sizeof(fpath), "%s/MainWindow.xaml", dir);
    f = fopen(fpath,"w"); fprintf(f,"%s",mainx); fclose(f);
    snprintf(fpath, sizeof(fpath), "%s/MainWindow.xaml.cs", dir);
    f = fopen(fpath,"w"); fprintf(f,"%s",cs); fclose(f);
    free(appx); free(mainx); free(cs); free(classname);
    // one success indicator string
    return strdup_safe(dir);
}

static char *gen_mac_from_node(ASTNode *node, const char *outdir) {
    Val *titleV = (Val*)hm_get(node->props, "title");
    char *title = titleV && titleV->kind==VAL_STRING ? titleV->s : strdup_safe(node->name);
    char *classname = sanitize_classname(title);
    const char *bgcolor = "black";
    size_t s = strlen(tmpl_mac_main) + strlen(title) + strlen(bgcolor) + 20;
    char *main = malloc(s);
    snprintf(main, s, tmpl_mac_main, title, bgcolor);
    char dir[512]; snprintf(dir, sizeof(dir), "%s/%s-macos", outdir, classname);
    char cmd[1024]; snprintf(cmd, sizeof(cmd), "mkdir -p \"%s\"", dir); system(cmd);
    char fpath[1024]; snprintf(fpath, sizeof(fpath), "%s/main.swift", dir);
    FILE *f = fopen(fpath,"w"); fprintf(f,"%s",main); fclose(f);
    free(main); free(classname);
    return strdup_safe(dir);
}

static char *gen_qt_from_node(ASTNode *node, const char *outdir) {
    Val *titleV = (Val*)hm_get(node->props, "title");
    char *title = titleV && titleV->kind==VAL_STRING ? titleV->s : strdup_safe(node->name);
    char *classname = sanitize_classname(title);
    size_t s = strlen(tmpl_qt_maincpp) + strlen(classname)*3 + strlen(title) + 20;
    char *src = malloc(s);
    snprintf(src, s, tmpl_qt_maincpp, classname, classname, title, classname, classname);
    char dir[512]; snprintf(dir, sizeof(dir), "%s/%s-linux", outdir, classname);
    char cmd[1024]; snprintf(cmd, sizeof(cmd), "mkdir -p \"%s\"", dir); system(cmd);
    char fpath[1024]; snprintf(fpath, sizeof(fpath), "%s/main.cpp", dir);
    FILE *f = fopen(fpath,"w"); fprintf(f,"%s",src); fclose(f);
    free(src); free(classname);
    return strdup_safe(dir);
}

static char *gen_web_from_node(ASTNode *node, const char *outdir) {
    Val *titleV = (Val*)hm_get(node->props, "title");
    char *title = titleV && titleV->kind==VAL_STRING ? titleV->s : strdup_safe(node->name);
    char *classname = sanitize_classname(title);
    const char *bg = "#000";
    size_t idxlen = strlen(tmpl_web_index) + strlen(title)*2 + strlen(bg) + 64;
    char *index = malloc(idxlen);
    snprintf(index, idxlen, tmpl_web_index, title, title, bg);
    size_t jslen = strlen(tmpl_web_mainjs) + strlen(title) + 10;
    char *mainjs = malloc(jslen);
    snprintf(mainjs, jslen, tmpl_web_mainjs, title);
    char dir[512]; snprintf(dir, sizeof(dir), "%s/%s-web", outdir, classname);
    char cmd[1024]; snprintf(cmd, sizeof(cmd), "mkdir -p \"%s\"", dir); system(cmd);
    char fpath[1024]; snprintf(fpath, sizeof(fpath), "%s/index.html", dir);
    FILE *f = fopen(fpath,"w"); fprintf(f,"%s",index); fclose(f);
    snprintf(fpath, sizeof(fpath), "%s/main.js", dir);
    f = fopen(fpath,"w"); fprintf(f,"%s",mainjs); fclose(f);
    free(index); free(mainjs); free(classname);
    return strdup_safe(dir);
}

/* ----------------------------
   Top-level compile driver for a single file input
   ---------------------------- */
static void compile_file(const char *src, const char *outdir) {
    Lexer L; lex_init(&L, src); lex_next(&L);
    while (L.cur.kind != T_EOF) {
        if (L.cur.kind == T_NEW) {
            lex_next(&L);
            ASTNode *n = parse_new(&L);
            if (strcmp(n->name, "window")==0) {
                char *d1 = gen_windows_from_node(n, outdir);
                char *d2 = gen_mac_from_node(n, outdir);
                char *d3 = gen_qt_from_node(n, outdir);
                char *d4 = gen_web_from_node(n, outdir);
                printf("Generated: %s , %s , %s , %s\n", d1, d2, d3, d4);
                free(d1); free(d2); free(d3); free(d4);
            } else {
                fprintf(stderr, "Unknown construct '%s' - no equivalent registered\n", n->name);
            }
            astnode_free(n);
        } else lex_next(&L);
    }
}

/* ----------------------------
   Small utility to read a file
   ---------------------------- */
static char *read_entire_file(const char *path) {
    FILE *f = fopen(path,"rb");
    if (!f) { perror("fopen"); return NULL; }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f); fseek(f, 0, SEEK_SET);
    char *buf = malloc(sz+1);
    size_t r = fread(buf,1,sz,f); buf[r]=0; fclose(f);
    return buf;
}

/* ----------------------------
   Main CLI
   ---------------------------- */
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input.ps> [outdir]\n", argv[0]);
        return 1;
    }
    const char *infile = argv[1];
    const char *outdir = argc >=3 ? argv[2] : "out";
    char *src = read_entire_file(infile);
    if (!src) { fprintf(stderr, "Cannot read input file\n"); return 1; }
    compile_file(src, outdir);
    free(src);
    return 0;
}
