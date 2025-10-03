# Visual Structure Code

<details>
      
<summary>
    Poly.Struct
</summary>      
        
    Poly.Struct

    meta {

        lang { en }

        charset { UTF-8 }

        title { San Diego Tree Experts | Professional Tree Trimming & Removal }

        viewport {
            width = device-width,
            initial-scale = 1.0
        }

        description {
            Expert tree trimming and removal services in San Diego, CA. We are licensed, insured, and dedicated to enhancing your landscape's beauty and safety. Get a free estimate today.
        }

        keywords {
            tree trimming, tree removal, San Diego, arborist, stump grinding, tree service, licensed, insured
        }

        // Link to the external JavaScript file

            Link {
                rel: "stylesheet"
                src: "script.js"
            }

        // Inter Font from Google Fonts

            link { rel: "preconnect" src: "https://fonts.gstatic.com" crossorigin }

            Link {
                rel: "preconnect"
                src: "https://fonts.googleapis.com"
            }

            Link {
                rel: "stylesheet"
                src: "https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700;800&display=swap"
            }

        // Load Tailwind CSS

            Link {
                rel: "script"
                src: "https://cdn.tailwindcss.com"
            }

        // Load Custom Script

            Link {
                rel: "script"
                src: "https://cdn.tailwindcss.com"
            }

        // Load Custom CSS

            Link {
                rel: "stylesheet"
                src: "style.css"
            }
    }

    body (class="bg-gray-50 text-gray-800 antialiased") {

        // Header & Navigation
        header (class="fixed top-0 left-0 right-0 z-50 bg-white/80 backdrop-blur-md shadow-sm") {


</details>

# Visual Styling Code

<details>      
<summary>
    Poly.Style
</summary>

    Poly.Style

    body {
        font-family: "Inter", sans-serif;
        overflow-x: hidden;
    }

    // Smooth scroll for anchor links

        html {
            scroll-behavior: smooth;
        }

    // Utility class for fade-in animation on scroll

        .fade-in {
            opacity: 0;
            transform: translateY(20px);
            transition: opacity 0.8s ease-out, transform 0.8s ease-out;
        }

        .fade-in.is-visible {
            opacity: 1;
            transform: translateY(0);
        }

        // Custom scrollbar
            ::-webkit-scrollbar {
            width: 10px;
        }

        ::-webkit-scrollbar-track {
            background: #f1f1f1;
            border-radius: 5px;
        }

        ::-webkit-scrollbar-thumb {
            background: #cbd5e1;
            border-radius: 5px;
        }

        ::-webkit-scrollbar-thumb:hover {
            background: #94a3b8;
        }

    // Keyframe for button bounce effect on hover

        @keyframes bounce {
            0%, 100% {
                transform: translateY(0);
            }
            50% {
                transform: translateY(-5px);
            }
        }

        .cta-button:hover {
            animation: bounce 0.5s ease-in-out;
        }

    // Simple animation for carousel to make it more visually appealing

        @keyframes slide {
            from {
                transform: translateX(0);
            }
            to {
                transform: translateX(calc(-100% * var(--items-per-slide)));
            }
        }

</details>

# Visual Scripting (Logic) Code

<details>
        
<summary>
    Poly.Script
</summary>     
        
    Poly.Script

    EventListener ( DOMContentLoaded ) {

        // Set current year in the footer

            const currentYear = new Date().getFullYear();

            getElementById("current-year").textContent = currentYear;


        // Smooth scrolling for navigation links

            const navLinks = querySelectorAll('nav a[href^="#"]');

            navLinks.forEach((link) => {

                link.addEventListener("click", function (e) {

                    e.preventDefault();

                    const targetId = this.getAttribute("href");

                    const targetElement = querySelector(targetId);

                    if (targetElement) {

                        targetElement.scrollIntoView({ behavior: "smooth" });

                    }
                });
            });


        // Form submission handler (simulated)

            const contactForm = getElementById("contact-form");

            const formMessage = getElementById("form-message");

            contactForm.addEventListener("submit", function (e) {
                e.preventDefault();

                // Simulate form submission

                    formMessage.textContent = "Sending your request...";

                    formMessage.classList.remove("text-red-600", "text-green-600");

                    formMessage.classList.add("text-gray-600");

                    setTimeout(() => {

                        // Simulate success

                            formMessage.textContent =
                                "Thank you! Your request has been sent successfully. We will be in touch shortly.";

                            formMessage.classList.remove("text-gray-600");

                            formMessage.classList.add("text-green-600");

                            contactForm.reset();
                    }, 1500);
            });


        // Intersection Observer for fade-in effect

            const faders = querySelectorAll(".fade-in");

            const appearOptions = {
                threshold: 0.2, // Trigger when 20% of the element is visible
                rootMargin: "0px 0px -50px 0px", // Start a bit before the bottom of the viewport
            };

            const appearOnScroll = new IntersectionObserver((entries, observer) => {
                entries.forEach((entry) => {
                    if (!entry.isIntersecting) {
                        return;
                    }
                    entry.target.classList.add("is-visible");
                    observer.unobserve(entry.target);
                });
            }, appearOptions);

            faders.forEach((fader) => {
                appearOnScroll.observe(fader);
            });

    }

</details>
