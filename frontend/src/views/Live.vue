<template>
    <VContainer fluid align="center" color="surface">
        <!-- Top Section (Two Columns) -->
        <VRow max-width="1200px">
            <VCol cols="8">
                <!-- Left Column (2 Rows) -->
                <VRow>
                    <!-- Temperature and Humidity -->
                    <VCol cols="6">
                        <VCard class="mb-5" max-width="500" color="primaryContainer" subtitle="Temperature">
                            <v-icon size="45" icon="mdi:mdi-thermometer"></v-icon>
                            <VCardItem>
                                <span class="text-h3 text-onPrimaryContainer">{{ temperature }}</span>
                            </VCardItem>
                        </VCard>
                    </VCol>

                    <VCol cols="6">
                        <VCard class="mb-5" max-width="500" color="secondaryContainer" subtitle="Humidity">
                            <v-icon size="45" icon="mdi:mdi-cloud-percent"></v-icon>
                            <VCardItem>
                                <span class="text-h3 text-onSecondaryContainer">{{ humidity }}</span>
                            </VCardItem>
                        </VCard>
                    </VCol>
                </VRow>
                <VRow>
                    <!-- Altitude and Pressure -->
                    <VCol cols="6">
                        <VCard class="mb-5" max-width="500" color="tertiaryContainer" subtitle="Pressure">
                            <v-icon size="45" icon="mdi:mdi-gauge"></v-icon>
                            <VCardItem>
                                <span class="text-h3 text-onTertiaryContainer">{{ pressure }}</span>
                            </VCardItem>
                        </VCard>
                    </VCol>

                    <VCol cols="6">
                        <VCard class="mb-5" max-width="500" color="infoContainer" subtitle="Altitude">
                            <v-icon size="45" icon="mdi:mdi-altimeter"></v-icon>
                            <VCardItem>
                                <span class="text-h3 text-onInfoContainer">{{ altitude }}</span>
                            </VCardItem>
                        </VCard>
                    </VCol>
                </VRow>
            </VCol>

            <VCol cols="4">
                <!-- Right Column (Moisture Slider) -->
                <v-card height="360px" width="150px">
                    <v-slider class="slider" readonly thumb-label color="blue" v-model="slider1" direction="vertical"
                        label="Moisture %" track-size="50">
                    </v-slider>
                </v-card>
            </VCol>
        </VRow>

        <!-- Graph Section (Separate Rows) -->
        <VRow max-width="1200px" justify="start">
            <!-- Heat Index Graph -->
            <VCol cols="12">
                <figure class="highcharts-figure">
                    <div id="container"></div>
                </figure>
            </VCol>
        </VRow>

        <VRow max-width="1200px" justify="start">
            <!-- Temperature, Humidity, and Pressure Graphs -->
            <VCol cols="12">
                <figure class="highcharts-figure">
                    <div id="container1"></div>
                </figure>
            </VCol>
        </VRow>
    </VContainer>
</template>

<script setup>
// JAVASCRIPT CODE
import { ref, reactive, watch, onMounted, onBeforeUnmount, computed } from "vue";  
import { useRoute, useRouter } from "vue-router";
import { useMqttStore } from "@/store/mqttStore";
import { storeToRefs } from "pinia";
import Highcharts from 'highcharts'; 
import more from 'highcharts/highcharts-more'; 
import Exporting from 'highcharts/modules/exporting'; 
Exporting(Highcharts); 
more(Highcharts);

const router = useRouter();
const route = useRoute();
const Mqtt = useMqttStore();
const { payload, payloadTopic } = storeToRefs(Mqtt);
const heatIndexChart = ref(null);
const tempHiChart = ref(null);

const slider1 = ref(50);
const points = ref(10); 
const shift = ref(false); 

const graphTime = 300;

// Computed properties for data display
const temperature = computed(()=>{ 
    if(!!payload.value){ 
        return `${payload.value.temperature} °C`; 
    } 
});

const humidity = computed(()=>{ 
    if(!!payload.value){ 
        return `${payload.value.humidity}%`; 
    } 
});

const pressure = computed(()=>{ 
    if(!!payload.value){ 
        return `${payload.value.pressure} hPa`; 
    } 
});

const altitude = computed(()=>{ 
    if(!!payload.value){ 
        return `${payload.value.altitude} m`; 
    } 
});

// Lifecycle hooks
onMounted(()=>{
    CreateCharts(); 
    Mqtt.connect(); 
    setTimeout( ()=>{ 
        Mqtt.subscribe("620162297");
        Mqtt.subscribe("620162297_pub"); 
    },3000);
});

onBeforeUnmount(()=>{
    Mqtt.unsubcribeAll();
});

// Chart Creation
const CreateCharts = async () => { 
    heatIndexChart.value = Highcharts.chart('container', { 
        chart: { zoomType: 'x' }, 
        title: { text: 'Heat Index', align: 'left' }, 
        yAxis: { title: { text: 'Heat Index' , style:{color:'#000000'}}, labels: { format: '{value}' } },
        xAxis: { 
            type: 'datetime', 
            title: { 
                text: 'Time', 
                min: Date.now() - graphTime * 1000,
                max: Date.now(),
                tickInterval: 1000, //Update every second
                style:{color:'#000000'} 
            } 
        },
        tooltip: { shared:true }, 
        series: [{ name: 'Heat Index', type: 'spline', data: [], turboThreshold: 0, color: Highcharts.getOptions().colors[2] }] 
    });

    tempHiChart.value = Highcharts.chart('container1', { 
        chart: { zoomType: 'x' }, 
        title: { text: 'Temperature, Humidity, and Pressure', align: 'left' }, 
        yAxis: { title: { text: 'Value', style:{color:'#000000'} }, labels: { format: '{value}' } },
        xAxis: { 
            type: 'datetime', 
            title: { 
                text: 'Time', 
                min: Date.now() - graphTime * 1000,
                max: Date.now(),
                tickInterval: 1000, //Update every second
                style:{color:'#000000'} 
            } 
        },
        tooltip: { shared:true }, 
        series: [
            { name: 'Temperature', type: 'spline', data: [], turboThreshold: 0, color: Highcharts.getOptions().colors[0] },
            { name: 'Humidity', type: 'spline', data: [], turboThreshold: 0, color: Highcharts.getOptions().colors[1] },
            { name: 'Pressure', type: 'spline', data: [], turboThreshold: 0, color: Highcharts.getOptions().colors[3] }
        ] 
    });
};

// Watch for data changes and update charts
watch(payload, (data) => {
    // Current timestamp in milliseconds
    const currentTime = Date.now(); 

    if(points.value > 0){ points.value -- ; } 
    else{ shift.value = true; } 

    //Update slider for Soil Moisture
    slider1.value = data.moisture;

    // Add new point to Heat Index graph
    heatIndexChart.value.series[0].addPoint(
        { y: parseFloat(data.temperature), x: data.timestamp * 1000 },
        true,
        shift.value //false
    );

    // Add new points to Temperature, Humidity, and Pressure graph
    heatIndexChart.value.series[0].addPoint({y:parseFloat(data.heatIndex) ,x: data.timestamp * 1000 }, true, shift.value);    
    tempHiChart.value.series[0].addPoint({y:parseFloat(data.temperature), x: data.timestamp * 1000}, true, shift.value);
    tempHiChart.value.series[1].addPoint({y:parseFloat(data.humidity), x: data.timestamp * 1000}, true, shift.value);
    tempHiChart.value.series[2].addPoint({y:parseFloat(data.pressure), x: data.timestamp * 1000}, true, shift.value);
    

    // Update the xAxis to always show the last 5 minutes
    // heatIndexChart.value.xAxis[0].update({
    //     min: currentTime - graphTime * 1000,
    //     max: currentTime
    // });
    // tempHiChart.value.xAxis[0].update({
    //     min: currentTime - graphTime * 1000,
    //     max: currentTime
    // });

    // Shift points if necessary
    heatIndexChart.value.series[0].shift();
    tempHiChart.value.series[0].shift();
    tempHiChart.value.series[1].shift();
    tempHiChart.value.series[2].shift();
});

</script>

<style scoped>
/** CSS STYLE HERE */
figure { 
    border: 2px solid black;
}
</style>
