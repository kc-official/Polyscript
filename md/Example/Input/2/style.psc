body {
        font-family: "Inter", sans-serif;
        overflow-x: hidden;
      }
      html { scroll-behavior: smooth; }

      /* Fade-in utility */
      .fade-in {
        opacity: 0;
        transform: translateY(20px);
        transition: opacity 0.8s ease-out, transform 0.8s ease-out;
      }
      .fade-in.is-visible {
        opacity: 1;
        transform: translateY(0);
      }

      /* Custom scrollbar */
      ::-webkit-scrollbar        { width: 10px; }
      ::-webkit-scrollbar-track  { background: #f1f1f1; border-radius: 5px; }
      ::-webkit-scrollbar-thumb  { background: #cbd5e1; border-radius: 5px; }
      ::-webkit-scrollbar-thumb:hover { background: #94a3b8; }

      /* Hover bounce for CTA */
      @keyframes bounce {
        0%, 100% { transform: translateY(0); }
        50%      { transform: translateY(-5px); }
      }
      .cta-button:hover { animation: bounce 0.5s ease-in-out; }

      /* Simple slide keyframes (kept for potential carousel) */
      @keyframes slide {
        from { transform: translateX(0); }
        to   { transform: translateX(calc(-100% * var(--items-per-slide))); }
      }