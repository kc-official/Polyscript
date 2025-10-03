// polyscriptc.rs
// Prototype Polyscript compiler (Rust version)

use std::collections::HashMap;
use std::env;
use std::fs;
use std::fs::File;
use std::io::{Read, Write};
use std::path::Path;

// ----------------------------
// Value (literal) representation
// ----------------------------
#[derive(Debug, Clone)]
enum Val {
    String(String),
    Number(f64),
    Bool(bool),
}

// ----------------------------
// AST Node (minimal)
// ----------------------------
#[derive(Debug)]
struct ASTNode {
    kind: String,                 // e.g. "new"
    name: String,                 // e.g. "window"
    props: HashMap<String, Val>,  // key -> Val
    children: Vec<ASTNode>,
}

impl ASTNode {
    fn new(kind: &str, name: &str) -> Self {
        Self {
            kind: kind.to_string(),
            name: name.to_string(),
            props: HashMap::new(),
            children: vec![],
        }
    }
}

// ----------------------------
// Tokenizer
// ----------------------------
#[derive(Debug, Clone, PartialEq)]
enum TokenKind {
    EOF,
    Ident,
    String,
    Number,
    LParen,
    RParen,
    LBrace,
    RBrace,
    Colon,
    Semi,
    New,
    True,
    False,
}

#[derive(Debug, Clone)]
struct Token {
    kind: TokenKind,
    lex: String,
}

struct Lexer<'a> {
    src: &'a str,
    i: usize,
    cur: Token,
}

impl<'a> Lexer<'a> {
    fn new(src: &'a str) -> Self {
        let mut l = Lexer {
            src,
            i: 0,
            cur: Token {
                kind: TokenKind::EOF,
                lex: "".into(),
            },
        };
        l.next();
        l
    }

    fn skip_ws(&mut self) {
        while let Some(c) = self.src.chars().nth(self.i) {
            if c.is_whitespace() {
                self.i += 1;
                continue;
            }
            if c == '/' && self.src[self.i..].starts_with("/*") {
                self.i += 2;
                while self.i + 1 < self.src.len()
                    && &self.src[self.i..self.i + 2] != "*/"
                {
                    self.i += 1;
                }
                if self.i + 1 < self.src.len() {
                    self.i += 2;
                }
                continue;
            }
            break;
        }
    }

    fn next(&mut self) {
        self.skip_ws();
        if self.i >= self.src.len() {
            self.cur = Token {
                kind: TokenKind::EOF,
                lex: "".into(),
            };
            return;
        }

        let rest = &self.src[self.i..];
        let c = rest.chars().next().unwrap();

        if c.is_alphabetic() || c == '_' {
            let mut end = self.i;
            while end < self.src.len() {
                let ch = self.src.chars().nth(end).unwrap();
                if ch.is_alphanumeric() || ch == '_' || ch == '-' {
                    end += 1;
                } else {
                    break;
                }
            }
            let lex = &self.src[self.i..end];
            let tk = match lex {
                "new" => TokenKind::New,
                "true" => TokenKind::True,
                "false" => TokenKind::False,
                _ => TokenKind::Ident,
            };
            self.cur = Token {
                kind: tk,
                lex: lex.to_string(),
            };
            self.i = end;
            return;
        }

        if c == '"' {
            self.i += 1;
            let mut end = self.i;
            while end < self.src.len() && self.src.chars().nth(end).unwrap() != '"' {
                end += 1;
            }
            let lex = &self.src[self.i..end];
            self.cur = Token {
                kind: TokenKind::String,
                lex: lex.to_string(),
            };
            self.i = end + 1;
            return;
        }

        if c.is_ascii_digit() || (c == '-' && rest.chars().nth(1).unwrap_or('0').is_ascii_digit()) {
            let mut end = self.i + 1;
            while end < self.src.len() {
                let ch = self.src.chars().nth(end).unwrap();
                if ch.is_ascii_digit() || ch == '.' {
                    end += 1;
                } else {
                    break;
                }
            }
            let lex = &self.src[self.i..end];
            self.cur = Token {
                kind: TokenKind::Number,
                lex: lex.to_string(),
            };
            self.i = end;
            return;
        }

        self.i += 1;
        self.cur = Token {
            kind: match c {
                '(' => TokenKind::LParen,
                ')' => TokenKind::RParen,
                '{' => TokenKind::LBrace,
                '}' => TokenKind::RBrace,
                ':' => TokenKind::Colon,
                ';' => TokenKind::Semi,
                _ => TokenKind::Ident,
            },
            lex: c.to_string(),
        };
    }
}

// ----------------------------
// Parser
// ----------------------------
fn parse_new(lex: &mut Lexer) -> ASTNode {
    if lex.cur.kind != TokenKind::Ident {
        panic!("Expected identifier after new");
    }
    let name = lex.cur.lex.clone();
    lex.next();
    let mut node = ASTNode::new("new", &name);

    if lex.cur.kind == TokenKind::LParen {
        lex.next();
        while lex.cur.kind != TokenKind::RParen && lex.cur.kind != TokenKind::EOF {
            if lex.cur.kind == TokenKind::Ident {
                let k = lex.cur.lex.clone();
                lex.next();
                if lex.cur.kind != TokenKind::Colon {
                    panic!("Expected ':' after property name");
                }
                lex.next();
                let v = match lex.cur.kind {
                    TokenKind::String => {
                        let s = lex.cur.lex.clone();
                        lex.next();
                        Val::String(s)
                    }
                    TokenKind::Number => {
                        let n: f64 = lex.cur.lex.parse().unwrap();
                        lex.next();
                        Val::Number(n)
                    }
                    TokenKind::True => {
                        lex.next();
                        Val::Bool(true)
                    }
                    TokenKind::False => {
                        lex.next();
                        Val::Bool(false)
                    }
                    TokenKind::Ident => {
                        let s = lex.cur.lex.clone();
                        lex.next();
                        Val::String(s)
                    }
                    _ => panic!("Unexpected value in property"),
                };
                node.props.insert(k, v);
                if lex.cur.kind == TokenKind::Semi {
                    lex.next();
                }
            } else {
                lex.next();
            }
        }
        if lex.cur.kind == TokenKind::RParen {
            lex.next();
        }
    }

    if lex.cur.kind == TokenKind::LBrace {
        lex.next();
        while lex.cur.kind != TokenKind::RBrace && lex.cur.kind != TokenKind::EOF {
            lex.next();
        }
        if lex.cur.kind == TokenKind::RBrace {
            lex.next();
        }
    }

    node
}

// ----------------------------
// Codegen (simplified, only web target shown here for brevity)
// ----------------------------
fn gen_web_from_node(node: &ASTNode, outdir: &str) -> String {
    let title = node
        .props
        .get("title")
        .and_then(|v| match v {
            Val::String(s) => Some(s.clone()),
            _ => None,
        })
        .unwrap_or_else(|| node.name.clone());

    let bg = "#000";
    let index = format!(
        "<!DOCTYPE html>\n<html><head><title>{}</title>\
        <style>body{{background:{}}}</style></head><body></body></html>",
        title, bg
    );
    let mainjs = format!(
        "document.addEventListener('DOMContentLoaded',()=>console.log('{} initialized'));",
        title
    );

    let dir = format!("{}/{}-web", outdir, title);
    fs::create_dir_all(&dir).unwrap();
    fs::write(format!("{}/index.html", dir), index).unwrap();
    fs::write(format!("{}/main.js", dir), mainjs).unwrap();
    dir
}

// ----------------------------
// Compile driver
// ----------------------------
fn compile_file(src: &str, outdir: &str) {
    let mut lex = Lexer::new(src);
    while lex.cur.kind != TokenKind::EOF {
        if lex.cur.kind == TokenKind::New {
            lex.next();
            let n = parse_new(&mut lex);
            if n.name == "window" {
                let d4 = gen_web_from_node(&n, outdir);
                println!("Generated: {}", d4);
            } else {
                eprintln!("Unknown construct '{}'", n.name);
            }
        } else {
            lex.next();
        }
    }
}

// ----------------------------
// Main CLI
// ----------------------------
fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        eprintln!("Usage: {} <input.ps> [outdir]", args[0]);
        return;
    }
    let infile = &args[1];
    let outdir = if args.len() >= 3 { &args[2] } else { "out" };
    let mut src = String::new();
    File::open(infile)
        .expect("Cannot open file")
        .read_to_string(&mut src)
        .unwrap();
    compile_file(&src, outdir);
}
