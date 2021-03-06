/*
** TailwindCSS Configuration File
**
** Docs: https://tailwindcss.com/docs/configuration
** Default: https://github.com/tailwindcss/tailwindcss/blob/master/stubs/defaultConfig.stub.js
*/
module.exports = {
  theme: {
    colors: {
      'black': '#000',
      'white': '#fff',

      'gray-0': '#333',
      'gray-1': '#888',
      
      'purple-0': '#04254e',
      'purple-1': '#5954a4',
      'purple-2': '#8a54a2',
      'purple-3': '#8ad5eb',
      'purple-3-1': '#bee7f4',

      'text-gray-1': '#bbb',
      'text-gray-2': '#fff',
      'text-warn-1': '#fc0',
      'text-error-1': '#d40'
    },

    screens: {
      'sm': '660px',
      'md': '768px',
      'lg': '1024px',
      'xl': '1280px',
      'fhd': '1870px',
      'wqhd': '2480px',

      'break-header': '950px',
    }
  },
  variants: {},
  plugins: [],

}
