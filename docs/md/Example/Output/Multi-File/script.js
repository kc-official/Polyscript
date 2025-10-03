document.addEventListener("DOMContentLoaded", () => {
  // Set current year in the footer
  const currentYear = new Date().getFullYear();
  document.getElementById("current-year").textContent = currentYear;

  // Smooth scrolling for navigation links
  const navLinks = document.querySelectorAll('nav a[href^="#"]');
  navLinks.forEach((link) => {
    link.addEventListener("click", function (e) {
      e.preventDefault();
      const targetId = this.getAttribute("href");
      const targetElement = document.querySelector(targetId);
      if (targetElement) {
        targetElement.scrollIntoView({ behavior: "smooth" });
      }
    });
  });

  // Form submission handler (simulated)
  const contactForm = document.getElementById("contact-form");
  const formMessage = document.getElementById("form-message");

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
  const faders = document.querySelectorAll(".fade-in");
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
});
