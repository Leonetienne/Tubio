export default {
  // Target (https://go.nuxtjs.dev/config-target)
  target: 'static',

  generate: {
    dir: "../Tubio/frontend/"
  },

  server: {
    host: "0.0.0.0"
  },

  // Global page headers (https://go.nuxtjs.dev/config-head)
  head: {
    title: 'Tubio - Video downloader',
    meta: [
      { charset: 'utf-8' },
      { name: 'viewport', content: 'width=device-width, initial-scale=1' },
      { hid: 'description', name: 'description', content: '' },
      { name: 'msapplication-TileColor', content: '#031934' },
      { name: 'theme-color', content: '#031934' },
    ],
    link: [
      { rel: 'apple-touch-icon', sizes: "180x180", href: '/apple-touch-icon.png' },
      { rel: 'icon', type: "image/png", sizes: "32x32", href: '/favicon-32x32.png' },
      { rel: 'icon', type: "image/png", sizes: "16x16", href: '/favicon-16x16.png' },
      { rel: 'manifest', href: '/site.webmanifest' },
      { rel: 'mask-icon', href: '/safari-pinned-tab.svg', color: '#031934' },
    ]
  },

  // Global CSS (https://go.nuxtjs.dev/config-css)
  css: [
    "~/assets/css/reset.css",
    "~/assets/css/main.css",
    "~/assets/css/modular.css",
    "~/assets/css/fonts/zilla-slab.css"
  ],

  // Plugins to run before rendering page (https://go.nuxtjs.dev/config-plugins)
  plugins: [
  ],

  // Auto import components (https://go.nuxtjs.dev/config-components)
  components: true,

  // Modules for dev and build (recommended) (https://go.nuxtjs.dev/config-modules)
  buildModules: [
    // https://go.nuxtjs.dev/stylelint
    '@nuxtjs/stylelint-module',
    // https://go.nuxtjs.dev/tailwindcss
    '@nuxtjs/tailwindcss',
  ],

  // Modules (https://go.nuxtjs.dev/config-modules)
  modules: [
    // https://go.nuxtjs.dev/axios
    '@nuxtjs/axios',
  ],

  // Axios module configuration (https://go.nuxtjs.dev/config-axios)
  axios: {},

  // Build Configuration (https://go.nuxtjs.dev/config-build)
  build: {
  }
}
