/**
 * plugins/vuetify.js
 *
 * Framework documentation: https://vuetifyjs.com`
 */

// Styles
import '@mdi/font/css/materialdesignicons.css'
import 'vuetify/styles'

// Composables
import { createVuetify } from 'vuetify'

const lightMode = {
  dark: false,
  colors: {
    primary : "#7B1FA2",  // Rich Dark Purple
    onPrimary : "#FFFFFF",
    primaryContainer : "#EDE7F6",
    onPrimaryContainer : "#4A148C",
    secondary : "#6A1B9A",
    onSecondary : "#FFFFFF",
    secondaryContainer : "#D1C4E9",
    onSecondaryContainer : "#311B92",
    tertiary : "#4A148C",
    onTertiary : "#FFFFFF",
    tertiaryContainer : "#F3E5F5",
    onTertiaryContainer : "#6A1B9A",
    error : "#D32F2F",
    errorContainer : "#FFCDD2",
    onError : "#FFFFFF",
    onErrorContainer : "#B71C1C",
    background : "#F8F5FC",  // Light purple
    onBackground : "#2E1A47",  // Dark purple text
    surface : "#F3E5F5", 
    onSurface : "#2E1A47",
    surfaceVariant : "#D1C4E9",
    onSurfaceVariant : "#4A148C",
    outline : "#9575CD",
    inverseOnSurface : "#F8F5FC",
    inverseSurface : "#4A148C",
    inversePrimary : "#9575CD",
    shadow : "#000000",
    surfaceTint : "#7B1FA2",
    outlineVariant : "#D1C4E9",
    scrim : "#000000"
  },
}

const darkMode = {
  dark: true,
  colors: {
    primary : "#D1C4E9",  // Light Purple
    onPrimary : "#4A148C",
    primaryContainer : "#311B92",
    onPrimaryContainer : "#D1C4E9",
    secondary : "#B39DDB",
    onSecondary : "#2E1A47",
    secondaryContainer : "#512DA8",
    onSecondaryContainer : "#EDE7F6",
    tertiary : "#9575CD",
    onTertiary : "#2E1A47",
    tertiaryContainer : "#673AB7",
    onTertiaryContainer : "#F3E5F5",
    error : "#D32F2F",
    errorContainer : "#B71C1C",
    onError : "#FFCDD2",
    onErrorContainer : "#FFEBEE",
    background : "#1A1224",  // Dark purple
    onBackground : "#D1C4E9",  // Light purple text
    surface : "#2E1A47",  
    onSurface : "#D1C4E9",
    surfaceVariant : "#311B92",
    onSurfaceVariant : "#B39DDB",
    outline : "#9575CD",
    inverseOnSurface : "#311B92",
    inverseSurface : "#D1C4E9",
    inversePrimary : "#7B1FA2",
    shadow : "#000000",
    surfaceTint : "#D1C4E9",
    outlineVariant : "#311B92",
    scrim : "#000000"
  },
}

// https://vuetifyjs.com/en/introduction/why-vuetify/#feature-guides
export default createVuetify({
  theme: {
    defaultTheme: 'darkMode',
    themes: {
      lightMode,
      darkMode 
    },
  } 
})
