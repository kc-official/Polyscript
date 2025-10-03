
// Footer
footer ( class: "bg-gray-900 text-white py-12" ) {

    div ( class: "container mx-auto px-4 sm:px-6 lg:px-8" ) {

        div ( class: "grid md:grid-cols-3 gap-8 text-center md:text-left" ) {
            
            // Company Info
            div ( class: "space-y-4" ) {

                h4 ( class: "text-xl font-bold text-green-500" ) { 
                    San Diego Tree Pros 
                }
            
                p ( class: "text-gray-400" ) {
                    Professional tree care services in San Diego, CA. Your trusted partner for a beautiful and safe landscape.
                }                    
            }
            
            // Quick Links
            div ( class: "space-y-4" ) {

                h4 ( class: "text-xl font-bold text-green-500" ) { Quick Links}

                ul ( class: "space-y-2 text-gray-400" ) { 
                    li {
                        a (
                            href: "#services"
                            class: "hover:text-green-500 transition-colors"
                        ) { Services }
                    }
                    li {
                        a (
                            href: "#about" 
                            class: "hover:text-green-500 transition-colors"
                        ) { About Us }
                    }
                    li {
                        a (
                            href: "#testimonials"
                            class: "hover:text-green-500 transition-colors"
                        ) { Testimonials }
                    }
                    li {
                        a (
                            href: "#contact"
                            class: "hover:text-green-500 transition-colors"
                        ) { Contact }
                    }
                }                        
            }
            
            // Contact Info
            div ( class: "space-y-4" ) {

                h4 ( class: "text-xl font-bold text-green-500" ) { Contact Us }

                ul ( class: "space-y-2 text-gray-400" ) { 
                    li {
                        a (
                            href: "tel:555-555-5555"
                            class: "hover:text-green-500 transition-colors"
                        ) { (555) 555-5555 }
                    }
                    li {
                        a (
                            href: "mailto:info@sdtree.com"
                            class: "hover:text-green-500 transition-colors"
                        ) { info@sdtree.com }
                    }
                    li {
                        San Diego, CA
                    }
                }
            }
            
            div ( class: "mt-12 pt-8 border-t border-gray-700 text-center text-gray-400" ) {
                p {
                    &copy; span( id: "current-year" ) {} San Diego Tree Pros. All rights reserved.
                }
            }
        }
    } 
} // end of footer  /* shows this as tooltip on hover for every closing brace, with jump to/highlight declaration