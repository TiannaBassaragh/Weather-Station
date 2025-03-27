<!-- <template>
    <VToolbar flat density="compact" color="surface" height="100" >
    <VContainer fluid>
            <VRow align="center" justify="center">
                <VCol cols="1" align="left">
                    <VImg src="@/assets/logo.svg" max-height="40" max-width="40" contain />
                </VCol>

                <VCol cols="10"  align="center"> 
                    <RouterLink class="route" :to="{ name: 'Home' }" >
                        <VBtn class="text-subtitle-2"   density="compact"  color="primary" :variant="(route.name == 'Home')? 'tonal':'text'">Home</VBtn>
                    </RouterLink>
                    
                    <RouterLink class="route" :to="{ name: 'Live' }">
                        <VBtn class="text-subtitle-2" density="compact" color="primary" :variant="(route.name == 'Live') ? 'tonal' : 'text'">Live</VBtn>
                    </RouterLink>

                    <RouterLink class="route" :to="{ name: 'Home' }">
                        <VBtn class="text-subtitle-2" density="compact" color="primary" :variant="(route.name == 'Analysis') ? 'tonal' : 'text'">Analysis</VBtn>
                    </RouterLink>
                </VCol>

                <VCol cols="1" align="right">
                    <VBtn size="x-small" :elevation="0"  icon @click="darkmode = !darkmode">                  
                        <VIcon v-if="darkmode" icon="mdi:mdi-weather-night"   ></VIcon>
                        <VIcon v-else  icon="mdi:mdi-white-balance-sunny"  ></VIcon>
                    </VBtn>
                </VCol>
            </VRow>
        </VContainer>
    </VToolbar>
</template> -->

<template>
    <VNavigationDrawer app permanent width="105" class="d-flex flex-column align-center">
      <VContainer fluid class="d-flex flex-column align-center pa-2">
        
        <!-- Small Logo at the Top -->
        <VImg src="@/assets/logo.svg" max-height="40" max-width="40" contain class="mb-3" />
  
        <!-- Navigation Items -->
        <VList dense class="mt-2">
          <VListItem to="/" :active="route.name === 'Home'" color="primary" class="justify-center flex-column text-center">
            <VIcon size="28" class icon>mdi-home</VIcon>
            <span class="nav-text">Home</span>
          </VListItem>
  
          <VListItem to="/live" :active="route.name === 'Live'" color="primary" class="justify-center flex-column text-center">
            <VIcon size="28" class icon>mdi-play-circle</VIcon>
            <span class="nav-text">Live</span>
          </VListItem>
  
          <VListItem to="/analysis" :active="route.name === 'Analysis'" color="primary" class="justify-center flex-column text-center">
            <VIcon size="28" class icon>mdi-chart-line</VIcon>
            <span class="nav-text">Analysis</span>
          </VListItem>
        </VList>
  
        <!-- Dark Mode Toggle at the Bottom -->
        <VSpacer />
        <VBtn icon size="40" class="mb-2" @click="darkmode = !darkmode">
          <VIcon size="28" v-if="darkmode">mdi-weather-night</VIcon>
          <VIcon size="28" v-else>mdi-white-balance-sunny</VIcon>
        </VBtn>
  
      </VContainer>
    </VNavigationDrawer>
</template>

  
  <script setup>
    import { useRoute,useRouter } from "vue-router";
    import { useTheme } from 'vuetify';  
    import { ref ,watch ,onMounted ,onBeforeMount } from 'vue';


  
    // VARIABLES 
    const theme       = useTheme();
    const darkmode    = ref(false); 
    const router      = useRouter();  
    const route       = useRoute();  

    // WATCHERS
    watch(darkmode,  (mode) => {
    theme.global.name.value = mode ?  'darkMode' : 'lightMode';
    localStorage.setItem("theme",mode ? 'darkMode' : 'lightMode');  
    });


    // FUNCTIONS
    onMounted(()=>{
 

    });

    onBeforeMount(()=>{
        // SAVE THEME TO LOCALSTORAGE MAKING IT PERSIST BROWSER REFRESH

        if(localStorage.getItem("theme") != null){
            theme.global.name.value = localStorage.getItem("theme");
            darkmode.value = theme.global.current.value.dark;
        }
        else{
            localStorage.setItem("theme",theme.global.current.value.dark ? 'darkMode' : 'lightMode');
            darkmode.value = theme.global.current.value.dark;    
        }  
    });

 
  </script>

  <style>
    .route {
      text-decoration: none;
      margin-left: 8px;
      margin-right: 8px; 
    }
    /* Style adjustments for smaller width */
    .v-navigation-drawer {
      display: flex;
      align-items: center;
      justify-content: center;
    }

    /* Align text and icons in a column */
    .v-list-item {
      min-width: 100%;
      padding: 10px 0;
      display: flex;
      flex-direction: column;
      align-items: center;
    }

    /* Style for navigation text */
    .nav-text {
      display: block;
      font-size: 0.9rem; /* Smaller text */
      margin-top: 4px; /* Space between icon and text */
      text-align: center;
    }

    /* Active link styling */
    .v-list-item--active {
      background-color: rgba(33, 150, 243, 0.2);
      border-radius: 8px;
    }

    .icon {
      display: block;
      margin-bottom: 4px;
    }
  </style>