Page lang:"eN"

  head
    meta (charset:"UTF-8")
    meta (name:"viewport", content:"width=device-width, initial-scale=1.0")
    meta (name:"description", content:"Expert tree trimming and removal services in San Diego, CA. We are licensed, insured, and dedicated to enhancing your landscape's beauty and safety. Get a free estimate today.")
    meta (name:"keywords", content:"tree trimming, tree removal, San Diego, arborist, stump grinding, tree service, licensed, insured")
    title = "San Diego Tree Experts | Professional Tree Trimming & Removal"

    # Tailwind + Fonts
    script (src:"https://cdn.tailwindcss.com")
    link   (rel:"preconnect", href:"https://fonts.googleapis.com")
    link   (rel:"preconnect", href:"https://fonts.gstatic.com", crossorigin)
    link   (rel:"stylesheet", href:"https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700;800&display=swap")

    # Custom CSS Style 
    link   (rel:"stylesheet", href:"style.psc")
      

  body.bg-gray-50.text-gray-800.antialiased

    # Header & Navigation
    header.fixed.top-0.left-0.right-0.z-50.bg-white/80.backdrop-blur-md.shadow-sm
      nav.container.mx-auto.px-4.sm:px-6.lg:px-8.py-4.flex.items-center.justify-between

        a.flex.items-center.text-2xl.font-bold.text-green-700 (href:"#hero")
          svg.w-8.h-8.mr-2.text-green-600 (xmlns:"http://www.w3.org/2000/svg", viewBox:"0 0 24 24", fill:"currentColor")
            path (fill-rule:"evenodd", clip-rule:"evenodd", d:"M12.985 2.113A2.25 2.25 0 0 1 15 4.194v15.612a2.25 2.25 0 0 1-2.015 2.081l-4.204-.618a2.25 2.25 0 0 1-1.99-2.298V5.011a2.25 2.25 0 0 1 2.015-2.298l4.204.618ZM12 4.194a.75.75 0 0 0-.671.465L9.671 7.56a.75.75 0 0 0-.007.828l1.657 2.455a.75.75 0 0 0 1.258-.847l-1.657-2.455a.75.75 0 0 0-.671-.465Z")
          = "San Diego Tree Pros"

        ul.hidden.md:flex.space-x-6.font-semibold
          li: a.hover:text-green-600.transition-colors (href:"#services")     = "Services"
          li: a.hover:text-green-600.transition-colors (href:"#about")        = "About Us"
          li: a.hover:text-green-600.transition-colors (href:"#testimonials") = "Testimonials"
          li: a.hover:text-green-600.transition-colors (href:"#gallery")      = "Gallery"
          li: a.hover:text-green-600.transition-colors (href:"#contact")      = "Contact"

        a.hidden.md:block.bg-green-600.hover:bg-green-700.text-white.font-semibold.py-2.px-6.rounded-full.shadow-lg.transition-all.cta-button (href:"#contact")
          = "Get a Free Estimate"

      # Smooth scrolling (delegated to <nav>)
      on.click (delegate:"a[href^='#']")
        preventDefault()
        targetId = event.target.getAttribute("href")
        el = document.querySelector(targetId)
        if el { el.scrollIntoView({ behavior:"smooth" }) }

    main

      # Hero
      section#hero.relative.bg-gradient-to-br.from-green-50.to-green-100.min-h-screen.flex.items-center.justify-center.pt-24.pb-12.overflow-hidden

        div.absolute.inset-0.z-0.opacity-20
          style = "background-image: url('data:image/svg+xml,%3Csvg width\\'60\\' height\\'60\\' viewBox\\'0 0 60 60\\' xmlns=\\'http://www.w3.org/2000/svg\\'%3E%3Cpath d=\\'M52.535 15.362a.5.5 0 0 0-.486-.486L.073 24.225a.5.5 0 0 0 .618.618l51.986-9.845a.5.5 0 0 0 .486-.486Z\\' fill=\\'#333\\' fill-opacity=\\'0.2\\' /%3E%3C/svg%3E'); background-size: 60px 60px;"

        div.relative.container.mx-auto.px-4.sm:px-6.lg:px-8.flex.flex-col.items-center.text-center.z-10
          h1.text-4xl.sm:text-5xl.md:text-6xl.font-extrabold.leading-tight.text-gray-900.drop-shadow-sm.mb-4
            = "Transform Your San Diego Landscape"
          p.text-lg.sm:text-xl.text-gray-600.max-w-3xl.mb-8
            = "Expert tree trimming, removal, and care to enhance your property's beauty and safety. Licensed, insured, and ready to serve you."
          a.bg-green-600.hover:bg-green-700.text-white.font-bold.text-lg.py-4.px-10.rounded-full.shadow-xl.transition-all.hover:scale-105.cta-button (href:"#contact")
            = "Get a Free Estimate Today"

          div.mt-8.flex.flex-wrap.justify-center.items-center.gap-6.text-gray-500
            span.flex.items-center.text-sm.font-medium
              svg.w-5.h-5.mr-2.text-green-500 (xmlns:"http://www.w3.org/2000/svg", viewBox:"0 0 24 24", fill:"currentColor")
                path (fill-rule:"evenodd", clip-rule:"evenodd", d:"M2.25 12c0-5.385 4.365-9.75 9.75-9.75s9.75 4.365 9.75 9.75-4.365 9.75-9.75 9.75S2.25 17.385 2.25 12Zm13.36-1.814a.75.75 0 1 0-1.22-.882l-3.483 4.79-1.88-1.88a.75.75 0 1 0-1.06 1.061l2.5 2.5a.75.75 0 0 0 1.137-.089l4-5.5Z")
              = "Licensed & Insured"
            span.flex.items-center.text-sm.font-medium
              svg.w-5.h-5.mr-2.text-green-500 (xmlns:"http://www.w3.org/2000/svg", viewBox:"0 0 24 24", fill:"currentColor")
                path (fill-rule:"evenodd", clip-rule:"evenodd", d:"M10.788 3.21c.44-.949 1.706-.949 2.146 0l2.676 5.706a1.5 1.5 0 0 0 1.155.882l6.236.885c1.028.146 1.447 1.378.732 2.04l-4.636 4.44c-.337.323-.52.766-.411 1.22l1.096 6.012c.19.991-.837 1.755-1.745 1.22l-5.604-3.141a1.5 1.5 0 0 0-1.42 0L5.343 20.76c-.908.535-1.935-.229-1.745-1.22l1.096-6.012c.109-.454-.074-.897-.411-1.22L1.082 11.85c-.715-.662-.296-1.894.732-2.04l6.236-.885a1.5 1.5 0 0 0 1.155-.882l2.676-5.706Z")
              = "5-Star Local Service"

      # Services
      section#services.py-16.sm:py-24.bg-white.fade-in
        on.appear (threshold:0.2, rootMargin:"0px 0px -50px 0px")
          this.classList.add("is-visible")

        div.container.mx-auto.px-4.sm:px-6.lg:px-8
          header.text-center.mb-12
            h2.text-3xl.sm:text-4xl.font-extrabold.text-gray-900.mb-4 = "Our Comprehensive Tree Services"
            p.text-lg.text-gray-600.max-w-2xl.mx-auto
              = "We provide a full range of tree care solutions to keep your property beautiful, safe, and healthy."

          div.grid.md:grid-cols-2.lg:grid-cols-3.gap-8
            article.bg-gray-100.p-8.rounded-2xl.shadow-md.hover:shadow-lg.transition-shadow.duration-300.transform.hover:-translate-y-1
              svg.w-10.h-10.text-green-600.mb-4 (xmlns:"http://www.w3.org/2000/svg", viewBox:"0 0 24 24", fill:"currentColor")
                path (fill-rule:"evenodd", clip-rule:"evenodd", d:"M12.97 3.97a.75.75 0 0 1 1.06 0l7.5 7.5a.75.75 0 0 1 0 1.06l-7.5 7.5a.75.75 0 1 1-1.06-1.06l6.22-6.22H3a.75.75 0 0 1 0-1.5h16.19l-6.22-6.22a.75.75 0 0 1 0-1.06Z")
              h3.text-xl.font-bold.text-gray-900.mb-2 = "Tree Trimming & Pruning"
              p.text-gray-600 = "Our experts meticulously prune your trees to promote healthy growth, improve their appearance, and prevent potential hazards."

            article.bg-gray-100.p-8.rounded-2xl.shadow-md.hover:shadow-lg.transition-shadow.duration-300.transform.hover:-translate-y-1
              svg.w-10.h-10.text-green-600.mb-4 (xmlns:"http://www.w3.org/2000/svg", viewBox:"0 0 24 24", fill:"currentColor")
                path (fill-rule:"evenodd", clip-rule:"evenodd", d:"M12.97 3.97a.75.75 0 0 1 1.06 0l7.5 7.5a.75.75 0 0 1 0 1.06l-7.5 7.5a.75.75 0 1 1-1.06-1.06l6.22-6.22H3a.75.75 0 0 1 0-1.5h16.19l-6.22-6.22a.75.75 0 0 1 0-1.06Z")
              h3.text-xl.font-bold.text-gray-900.mb-2 = "Safe Tree Removal"
              p.text-gray-600 = "Whether a tree is diseased, damaged, or simply in the way, we perform safe and efficient removals with minimal impact on your property."

            article.bg-gray-100.p-8.rounded-2xl.shadow-md.hover:shadow-lg.transition-shadow.duration-300.transform.hover:-translate-y-1
              svg.w-10.h-10.text-green-600.mb-4 (xmlns:"http://www.w3.org/2000/svg", viewBox:"0 0 24 24", fill:"currentColor")
                path (fill-rule:"evenodd", clip-rule:"evenodd", d:"M12.97 3.97a.75.75 0 0 1 1.06 0l7.5 7.5a.75.75 0 0 1 0 1.06l-7.5 7.5a.75.75 0 1 1-1.06-1.06l6.22-6.22H3a.75.75 0 0 1 0-1.5h16.19l-6.22-6.22a.75.75 0 0 1 0-1.06Z")
              h3.text-xl.font-bold.text-gray-900.mb-2 = "Stump Grinding"
              p.text-gray-600 = "Complete the tree removal process by grinding the stump below ground level, making the area ready for new landscaping."

      # About / Value props
      section#about.py-16.sm:py-24.bg-gray-50.fade-in
        on.appear (threshold:0.2, rootMargin:"0px 0px -50px 0px")
          this.classList.add("is-visible")

        div.container.mx-auto.px-4.sm:px-6.lg:px-8
          div.grid.md:grid-cols-2.gap-12.items-center
            div.space-y-6
              h2.text-3xl.sm:text-4xl.font-extrabold.text-gray-900 = "Committed to Quality and Safety"
              p.text-lg.text-gray-600
                = "At San Diego Tree Pros, we're not just about cutting trees—we're about providing peace of mind. Our team is fully licensed and insured, ensuring that every project, big or small, is handled with the highest standards of safety and professionalism. We believe in honest work and transparent pricing, with no hidden fees."
              ul.space-y-4.text-lg.text-gray-700
                li.flex.items-start
                  svg.w-6.h-6.text-green-600.mr-3.mt-1.shrink-0 (xmlns:"http://www.w3.org/2000/svg", viewBox:"0 0 24 24", fill:"currentColor")
                    path (fill-rule:"evenodd", clip-rule:"evenodd", d:"M2.25 12c0-5.385 4.365-9.75 9.75-9.75s9.75 4.365 9.75 9.75-4.365 9.75-9.75 9.75S2.25 17.385 2.25 12Zm13.36-1.814a.75.75 0 1 0-1.22-.882l-3.483 4.79-1.88-1.88a.75.75 0 1 0-1.06 1.061l2.5 2.5a.75.75 0 0 0 1.137-.089l4-5.5Z")
                  span = "Free, No-Obligation Estimates"
                li.flex.items-start
                  svg.w-6.h-6.text-green-600.mr-3.mt-1.shrink-0 (xmlns:"http://www.w3.org/2000/svg", viewBox:"0 0 24 24", fill:"currentColor")
                    path (fill-rule:"evenodd", clip-rule:"evenodd", d:"M2.25 12c0-5.385 4.365-9.75 9.75-9.75s9.75 4.365 9.75 9.75-4.365 9.75-9.75 9.75S2.25 17.385 2.25 12Zm13.36-1.814a.75.75 0 1 0-1.22-.882l-3.483 4.79-1.88-1.88a.75.75 0 1 0-1.06 1.061l2.5 2.5a.75.75 0 0 0 1.137-.089l4-5.5Z")
                  span = "Licensed and Fully Insured for Your Protection"
                li.flex.items-start
                  svg.w-6.h-6.text-green-600.mr-3.mt-1.shrink-0 (xmlns:"http://www.w3.org/2000/svg", viewBox:"0 0 24 24", fill:"currentColor")
                    path (fill-rule:"evenodd", clip-rule:"evenodd", d:"M2.25 12c0-5.385 4.365-9.75 9.75-9.75s9.75 4.365 9.75 9.75-4.365 9.75-9.75 9.75S2.25 17.385 2.25 12Zm13.36-1.814a.75.75 0 1 0-1.22-.882l-3.483 4.79-1.88-1.88a.75.75 0 1 0-1.06 1.061l2.5 2.5a.75.75 0 0 0 1.137-.089l4-5.5Z")
                  span = "Local San Diego Tree Care Experts"
              a.inline-block.mt-6.bg-green-600.hover:bg-green-700.text-white.font-bold.py-3.px-8.rounded-full.shadow-lg.transition-all.cta-button (href:"#contact")
                = "Request Your Free Estimate"

            div.w-full.h-80.sm:h-96.md:h-full.rounded-2xl.shadow-xl.overflow-hidden.relative.group
              img (src:"https://placehold.co/800x600/65a30d/ffffff?text=Professional+Service", alt:"Professional arborist trimming a tree", class:"w-full h-full object-cover transition-transform duration-300 group-hover:scale-105", onerror:"this.onerror=null;this.src='https://placehold.co/800x600/65a30d/ffffff?text=Professional+Service';")
              div.absolute.inset-0.bg-green-900.opacity-20

      # Testimonials
      section#testimonials.py-16.sm:py-24.bg-gray-100.fade-in
        on.appear (threshold:0.2, rootMargin:"0px 0px -50px 0px")
          this.classList.add("is-visible")

        div.container.mx-auto.px-4.sm:px-6.lg:px-8.text-center
          h2.text-3xl.sm:text-4xl.font-extrabold.text-gray-900.mb-4 = "What Our Clients Say"
          p.text-lg.text-gray-600.max-w-2xl.mx-auto.mb-12
            = "Don't just take our word for it. Here's what our satisfied customers in San Diego have to say about our service."

          div#testimonial-carousel.grid.lg:grid-cols-3.gap-8
            blockquote.bg-white.p-8.rounded-2xl.shadow-lg.border-t-4.border-green-600
              p.text-lg.text-gray-700.font-medium.mb-4
                = "\"The team was professional, on time, and did an incredible job with our overgrown palm trees. Our yard looks so much better and safer now. Highly recommend!\""
              cite.font-bold.text-gray-900.not-italic = "— Jessica M., La Jolla"

            blockquote.bg-white.p-8.rounded-2xl.shadow-lg.border-t-4.border-green-600
              p.text-lg.text-gray-700.font-medium.mb-4
                = "\"We had a large, dead oak that was a major concern. San Diego Tree Pros removed it safely and efficiently, and the cleanup was perfect. A fantastic service.\""
              cite.font-bold.text-gray-900.not-italic = "— Robert P., Del Mar"

            blockquote.bg-white.p-8.rounded-2xl.shadow-lg.border-t-4.border-green-600
              p.text-lg.text-gray-700.font-medium.mb-4
                = "\"They provided a quick and honest estimate. The work was done exactly as promised, and they left our property spotless. A pleasure to work with from start to finish.\""
              cite.font-bold.text-gray-900.not-italic = "— Carlos G., Poway"

      # Gallery
      section#gallery.py-16.sm:py-24.bg-gray-50.fade-in
        on.appear (threshold:0.2, rootMargin:"0px 0px -50px 0px")
          this.classList.add("is-visible")

        div.container.mx-auto.px-4.sm:px-6.lg:px-8.text-center
          h2.text-3xl.sm:text-4xl.font-extrabold.text-gray-900.mb-4 = "Our Work in Action"
          p.text-lg.text-gray-600.max-w-2xl.mx-auto.mb-12
            = "See the difference professional care makes. Browse our gallery of recent projects in the San Diego area."

          div.grid.sm:grid-cols-2.lg:grid-cols-3.gap-6
            figure.relative.overflow-hidden.rounded-2xl.shadow-lg.group
              img (src:"https://placehold.co/600x400/16a34a/ffffff?text=Before+Trimming", alt:"Before tree trimming", class:"w-full h-full object-cover transition-transform duration-300 group-hover:scale-110")
              figcaption.absolute.inset-0.bg-gray-900.bg-opacity-40.flex.items-center.justify-center.text-white.text-xl.font-bold.opacity-0.transition-opacity.duration-300.group-hover:opacity-100
                = "Before & After"

            figure.relative.overflow-hidden.rounded-2xl.shadow-lg.group
              img (src:"https://placehold.co/600x400/22c55e/ffffff?text=After+Trimming", alt:"After tree trimming", class:"w-full h-full object-cover transition-transform duration-300 group-hover:scale-110")
              figcaption.absolute.inset-0.bg-gray-900.bg-opacity-40.flex.items-center.justify-center.text-white.text-xl.font-bold.opacity-0.transition-opacity.duration-300.group-hover:opacity-100
                = "Perfectly Pruned"

            figure.relative.overflow-hidden.rounded-2xl.shadow-lg.group
              img (src:"https://placehold.co/600x400/4ade80/ffffff?text=Stump+Removal", alt:"Stump removal process", class:"w-full h-full object-cover transition-transform duration-300 group-hover:scale-110")
              figcaption.absolute.inset-0.bg-gray-900.bg-opacity-40.flex.items-center.justify-center.text-white.text-xl.font-bold.opacity-0.transition-opacity.duration-300.group-hover:opacity-100
                = "Stump Ground to Dust"

      # Contact
      section#contact.py-16.sm:py-24.bg-green-600.text-white.fade-in
        on.appear (threshold:0.2, rootMargin:"0px 0px -50px 0px")
          this.classList.add("is-visible")

        div.container.mx-auto.px-4.sm:px-6.lg:px-8.text-center
          h2.text-3xl.sm:text-4xl.font-extrabold.mb-4 = "Ready to Enhance Your Property?"
          p.text-lg.mb-8 = "Get in touch today for a free, no-obligation estimate. We'll assess your needs and provide a clear, fair quote."

          form#contact-form.max-w-xl.mx-auto.bg-white.p-8.sm:p-10.rounded-2xl.shadow-xl.text-left.text-gray-800.space-y-6 (novalidate)
            div
              label.block.text-sm.font-semibold.mb-2 (for:"name")  = "Full Name"
              input#name.w-full.p-3.rounded-lg.border-2.border-gray-300.focus:outline-none.focus:border-green-600.transition-colors (type:"text", name:"name", required:true)
            div
              label.block.text-sm.font-semibold.mb-2 (for:"email") = "Email Address"
              input#email.w-full.p-3.rounded-lg.border-2.border-gray-300.focus:outline-none.focus:border-green-600.transition-colors (type:"email", name:"email", required:true)
            div
              label.block.text-sm.font-semibold.mb-2 (for:"phone") = "Phone Number"
              input#phone.w-full.p-3.rounded-lg.border-2.border-gray-300.focus:outline-none.focus:border-green-600.transition-colors (type:"tel", name:"phone")
            div
              label.block.text-sm.font-semibold.mb-2 (for:"message") = "Tell Us About Your Project"
              textarea#message.w-full.p-3.rounded-lg.border-2.border-gray-300.focus:outline-none.focus:border-green-600.transition-colors (name:"message", rows:"4", required:true)

            button.w-full.bg-green-600.hover:bg-green-700.text-white.font-bold.text-lg.py-4.rounded-full.shadow-lg.transition-all.cta-button (type:"submit")
              = "Send My Free Estimate Request"

            div#form-message.mt-4.text-center.font-bold (role:"alert", aria-live:"polite")

            # Inline form logic (lossless rewrite)
            on.submit
              preventDefault()
              formMessage = document.getElementById("form-message")
              formMessage.textContent = "Sending your request..."
              formMessage.classList.remove("text-red-600", "text-green-600")
              formMessage.classList.add("text-gray-600")
              setTimeout(() => {
                formMessage.textContent = "Thank you! Your request has been sent successfully. We will be in touch shortly."
                formMessage.classList.remove("text-gray-600")
                formMessage.classList.add("text-green-600")
                this.reset()
              }, 1500)

    # Footer
    footer.bg-gray-900.text-white.py-12
      div.container.mx-auto.px-4.sm:px-6.lg:px-8
        div.grid.md:grid-cols-3.gap-8.text-center.md:text-left

          div.space-y-4
            h4.text-xl.font-bold.text-green-500 = "San Diego Tree Pros"
            p.text-gray-400
              = "Professional tree care services in San Diego, CA. Your trusted partner for a beautiful and safe landscape."

          div.space-y-4
            h4.text-xl.font-bold.text-green-500 = "Quick Links"
            ul.space-y-2.text-gray-400
              li: a.hover:text-green-500.transition-colors (href:"#services")     = "Services"
              li: a.hover:text-green-500.transition-colors (href:"#about")        = "About Us"
              li: a.hover:text-green-500.transition-colors (href:"#testimonials") = "Testimonials"
              li: a.hover:text-green-500.transition-colors (href:"#contact")      = "Contact"

          div.space-y-4
            h4.text-xl.font-bold.text-green-500 = "Contact Us"
            ul.space-y-2.text-gray-400
              li: a.hover:text-green-500.transition-colors (href:"tel:555-555-5555") = "(555) 555-5555"
              li: a.hover:text-green-500.transition-colors (href:"mailto:info@sdtree.com") = "info@sdtree.com"
              li = "San Diego, CA"

        div.mt-12.pt-8.border-t.border-gray-700.text-center.text-gray-400
          p = "© " + span#current-year + "San Diego Tree Pros. All rights reserved."
            /* can also be written as 
              ```
                p = "© "
                  - span#current-year
                  - " San Diego Tree Pros. All rights reserved."
              ```
            /*

      # Footer boot logic: set current year on mount
      on.ready
        document.getElementById("current-year").textContent = new Date().getFullYear()

  # (Optional) Raw JS block if you prefer keeping original script verbatim.
  # script |
  #   document.addEventListener("DOMContentLoaded", () => { /* ...original JS... */ })
