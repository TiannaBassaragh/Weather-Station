<template>
    <v-container class="container" fluid>
      <v-row class="row">
        <v-col cols="3" align="center">
          <v-card
            title="Temperature"
            width="250"
            variant="outlined"
            color="primary"
            density="compact"
            rounded="lg"
          >
          <v-card-item class="mb-n5">
              <v-chip-group
                class="d-flex flex-row justify-center"
                color="primaryContainer"
                variant="flat"
              >
                <v-tooltip text="Min" location="start">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ temperature.min }}</v-chip>
                  </template>
                </v-tooltip>
  
                <v-tooltip text="Range" location="top">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ temperature.range }}</v-chip>
                  </template>
                </v-tooltip>
                <v-tooltip text="Max" location="end">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ temperature.max }}</v-chip>
                  </template>
                </v-tooltip>
              </v-chip-group>
            </v-card-item>
            <v-card-item align="center">
              <span class="text-h2 text-primary font-weight-bold">
                {{ temperature.avg }}
              </span>
            </v-card-item>
          </v-card>
        </v-col>
        <v-col cols="3" align="center">
          <v-card
            title="Humidity"
            width="250"
            variant="outlined"
            color="primary"
            density="compact"
            rounded="lg"
            >
            <v-card-item class="mb-n5">
              <v-chip-group
                class="d-flex flex-row justify-center"
                color="primaryContainer"
                variant="flat"
              >
                <v-tooltip text="Min" location="start">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ humidity.min }}</v-chip>
                  </template>
                </v-tooltip>
                
                <v-tooltip text="Range" location="top">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ humidity.range }}</v-chip>
                  </template>
                </v-tooltip>
                <v-tooltip text="Max" location="end">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ humidity.max }}</v-chip>
                  </template>
                </v-tooltip>
              </v-chip-group>
            </v-card-item>
            <v-card-item align="center">
              <span class="text-h2 text-primary font-weight-bold">
                {{ humidity.avg }}
              </span>
            </v-card-item>
          </v-card>
        </v-col>
        <v-col cols="3" align="center">
          <v-card
            title="Air Pressure"
            width="250"
            variant="outlined"
            color="primary"
            density="compact"
            rounded="lg"
            >
            <v-card-item class="mb-n5">
              <v-chip-group
                class="d-flex flex-row justify-center"
                color="primaryContainer"
                variant="flat"
                >
                <v-tooltip text="Min" location="start">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ pressure.min }}</v-chip>
                  </template>
                </v-tooltip>
                
                <v-tooltip text="Range" location="top">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ pressure.range }}</v-chip>
                  </template>
                </v-tooltip>
                <v-tooltip text="Max" location="end">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ pressure.max }}</v-chip>
                  </template>
                </v-tooltip>
              </v-chip-group>
            </v-card-item>
            <v-card-item align="center">
              <span class="text-h2 text-primary font-weight-bold">
                {{ pressure.avg }}
              </span>
            </v-card-item>
          </v-card>
        </v-col>
        <v-col cols="3" align="center">
          <v-card
            title="Soil Moisture"
            width="250"
            variant="outlined"
            color="primary"
            density="compact"
            rounded="lg"
            >
            <v-card-item class="mb-n5">
              <v-chip-group
                class="d-flex flex-row justify-center"
                color="primaryContainer"
                variant="flat"
                >
                <v-tooltip text="Min" location="start">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ moisture.min }}</v-chip>
                  </template>
                </v-tooltip>
                
                <v-tooltip text="Range" location="top">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ moisture.range }}</v-chip>
                  </template>
                </v-tooltip>
                <v-tooltip text="Max" location="end">
                  <template v-slot:activator="{ props }">
                    <v-chip v-bind="props">{{ moisture.max }}</v-chip>
                  </template>
                </v-tooltip>
              </v-chip-group>
            </v-card-item>
            <v-card-item align="center">
              <span class="text-h2 text-primary font-weight-bold">
                {{ moisture.avg }}
              </span>
            </v-card-item>
          </v-card>
        </v-col>
      </v-row>
      <v-row class="ma-5" align="center">
        <v-col align="center">
          <p>Enter date range for Analysis</p>
          <v-divider></v-divider>
          <br />
          <v-sheet class="sheet">
            <v-text-field
              v-model="start"
              label="Start date"
              type="Date"
              dense
              solo-inverted
              class="mr-5"
              :style="{ maxWidth: '210px' }"
              flat
            ></v-text-field>
            <v-text-field
              v-model="end"
              label="End date"
              type="Date"
              dense
              solo-inverted
              class="mr-5"
              :style="{ maxWidth: '210px' }"
              flat
            ></v-text-field>
          </v-sheet>
            <br />
            <v-btn
              text="Analyze"
              class="mr-5"
              @click="
                updateLineCharts(isFarenheit);
                updateCards(isFarenheit);
                updateHistogramCharts(isFarenheit);
              "
              color="primary"
              variant="tonal"
            ></v-btn>
            <v-btn
              text="Celsius/Farenheit"
              class="mr-5"
              @click="convertData()"
              color="primary"
              variant="tonal"
            ></v-btn>
        </v-col>
      </v-row>
      <v-row class="row">
        <v-col cols="12">
          <figure class="highcharts-figure"> <!-- Heat Index Graph -->
            <div id="container"></div>
          </figure>
        </v-col>
        <v-col cols="12">
          <figure class="highcharts-figure"> <!-- Temperature, Humidity & Air Pressure Graph -->
            <div id="container0"></div>
          </figure>
        </v-col>
      </v-row>
      <v-row class="row">
        <v-col class="col1" cols="12">
          <figure class="highcharts-figure"> <!-- Frequency Distribution Graph -->
            <div id="container1"></div>
          </figure>
        </v-col>
      </v-row>
    </v-container>
  </template>
  
  <script setup>
  /** JAVASCRIPT HERE */
  
  import Highcharts from "highcharts";
  import more from "highcharts/highcharts-more";
  import Exporting from "highcharts/modules/exporting";
  import { withDirectives } from "vue";
  Exporting(Highcharts);
  more(Highcharts);
  
  // IMPORTS
  import { useMqttStore } from "@/store/mqttStore"; // Import Mqtt Store
  import { storeToRefs } from "pinia";
  
  import { useAppStore } from "@/store/appStore";
  import {
    ref,
    reactive,
    watch,
    onMounted,
    onBeforeUnmount,
    computed,
  } from "vue";
  import { useRoute, useRouter } from "vue-router";
  
  // VARIABLES
  const Mqtt = useMqttStore();
  const AppStore = useAppStore();
  const router = useRouter();
  const route = useRoute();
  var start = ref(null);
  var end = ref(null);
  var temperature = reactive({ min: 0, max: 0, avg: 0, range: 0 });
  var humidity = reactive({ min: 0, max: 0, avg: 0, range: 0 });
  var pressure = reactive({ min: 0, max: 0, avg: 0, range: 0 });
  var moisture = reactive({ min: 0, max: 0, avg: 0, range: 0 });
  const heatIndexLine = ref(null); // Chart object
  const tempLine = ref(null); // Chart object
  const histo = ref(null); // Chart object
  const isFarenheit = ref(false);
  
  // FUNCTIONS
  
  const CreateCharts = async () => {
  // FIRST GRAPH: Heat Index, Temperature, and Humidity
  heatIndexLine.value = Highcharts.chart("container", {
    chart: { zoomType: "x" },
    title: { text: "Heat Index, Temperature & Humidity", align: "center" },
      subtitle: {
        text:
          " The heat index, also known as the apparent temperature, is a measure that combines air temperature and relative humidity to assess how hot it feels to the human body. " +
          "As humidity increases the apparent temperature feels hotter than it is. Hence the heat index rises as the humidity does.",
      },
    yAxis: [
      {
        title: { text: "Temperature (°C)", style: { color: "#000000" } },
        labels: { format: "{value} °C" },
        opposite: false,
      },
      {
        title: { text: "Humidity (%)", style: { color: "#000000" } },
        labels: { format: "{value} %" },
        opposite: true,
      },
    ],
    xAxis: {
      type: "datetime",
      title: { text: "Time", style: { color: "#000000" } },
    },
    tooltip: { shared: true },
    series: [
      {
        name: "Heat Index",
        type: "line",
        data: [],
        color: Highcharts.getOptions().colors[0],
      },
      {
        name: "Temperature",
        type: "line",
        data: [],
        color: Highcharts.getOptions().colors[1],
      },
      {
        name: "Humidity",
        type: "line",
        data: [],
        yAxis: 1,
        color: Highcharts.getOptions().colors[2],
      },
    ],
  });

  // SECOND GRAPH: Air Pressure and Altitude
  tempLine.value = Highcharts.chart("container0", {
    chart: { zoomType: "x" },
    title: { text: "Air Pressure & Altitude", align: "center" },
    yAxis: [
      {
        title: { text: "Pressure (hPa)", style: { color: "#000000" } },
        labels: { format: "{value} hPa" },
        opposite: false,
      },
      {
        title: { text: "Altitude (m)", style: { color: "#000000" } },
        labels: { format: "{value} m" },
        opposite: true,
      },
    ],
    xAxis: {
      type: "datetime",
      title: { text: "Time", style: { color: "#000000" } },
    },
    tooltip: { shared: true },
    series: [
      {
        name: "Air Pressure",
        type: "line",
        data: [],
        color: Highcharts.getOptions().colors[3],
      },
      {
        name: "Altitude",
        type: "line",
        data: [],
        yAxis: 1,
        color: Highcharts.getOptions().colors[4],
      },
    ],
  });

  // THIRD GRAPH: Frequency Distribution
  histo.value = Highcharts.chart("container1", {
      chart: { zoomType: "x" },
      title: { text: "Frequency Distribution Analysis", align: "left" },
      yAxis: {
        title: {
          text: "Frequency",
          style: { color: "#000000" },
        },
        labels: { format: "{value}" },
      },
  
      xAxis: {
        title: { text: "Value  % / °C ", style: { color: "#000000" } },
      },
      tooltip: { shared: true },
      series: [
        {
          name: "Temperature",
          type: "bar",
          data: [],
          turboThreshold: 0,
          color: Highcharts.getOptions().colors[0],
        },
        {
          name: "Humidity",
          type: "bar",
          data: [],
          turboThreshold: 0,
          color: Highcharts.getOptions().colors[1],
        },
      ],
    });
  };
  
  onMounted(() => {
    // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
    Mqtt.connect(); // Connect to Broker located on the backend
    setTimeout(() => {
      // Subscribe to each topic
      Mqtt.subscribe("620162297");
      Mqtt.subscribe("620162297_sub");
    }, 3000);
    CreateCharts();
  });
  
  onBeforeUnmount(() => {
    // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
    Mqtt.unsubcribeAll();
  });
  
  const updateLineCharts = async (rule) => {
    console.log("updateLineCharts function is being called!");
  if (!!start.value && !!end.value) {
      // Convert output from Textfield components to 10 digit timestamps
      let startDate = new Date(start.value).getTime() / 1000;
      let endDate = new Date(end.value).getTime() / 1000;
      // Fetch data from backend
      const data = await AppStore.getAllInRange(startDate, endDate);
      // if (data) {console.log(data);}
      // else {console.log("no data");}

      let heatIndex = [];
      let temperature = [];
      let humidity = [];
      let pressure = [];
      let altitude = [];

    data.forEach((row) => {
        let temp = rule ? celsiusToFahrenheit(row.temperature) : row.temperature;
        let heatIdx = rule ? celsiusToFahrenheit(row.heatIndex) : row.heatIndex;

        heatIndex.push({ x: row.timestamp * 1000, y: parseFloat(heatIdx.toFixed(2)) });
        temperature.push({ x: row.timestamp * 1000, y: parseFloat(temp.toFixed(2)) });
        humidity.push({ x: row.timestamp * 1000, y: parseFloat(row.humidity.toFixed(2)) });
        pressure.push({ x: row.timestamp * 1000, y: parseFloat(row.pressure.toFixed(2)) });
        altitude.push({ x: row.timestamp * 1000, y: parseFloat(row.altitude.toFixed(2)) });
    });
    

    heatIndexLine.value.series[0].setData(heatIndex);
    heatIndexLine.value.series[1].setData(temperature);
    heatIndexLine.value.series[2].setData(humidity);
    tempLine.value.series[0].setData(pressure);
    tempLine.value.series[1].setData(altitude);
  }
};
  
  const updateCards = async (rule) => {
    console.log("updateCards function is being called!");
    // Retrieve Min, Max, Avg, Spread/Range
    if (!!start.value && !!end.value) {
  
      // 1. Convert start and end dates collected fron TextFields to 10 digit timestamps
      let startDate = new Date(start.value).getTime() / 1000;
      let endDate = new Date(end.value).getTime() / 1000;
      // 2. Fetch data from backend by calling the API functions
      const temp = await AppStore.getTemperatureMMAR(startDate, endDate);
      const humid = await AppStore.getHumidityMMAR(startDate, endDate);
      const air = await AppStore.getPressureMMAR(startDate, endDate);
      const soil= await AppStore.getSoilMMAR(startDate, endDate);
      
      if(rule){
   
      temperature.max = celsiusToFahrenheit(temp[0].max).toFixed(1);
      temperature.min = celsiusToFahrenheit(temp[0].min).toFixed(1);
      temperature.avg = celsiusToFahrenheit(temp[0].avg).toFixed(1);
      temperature.range = (temperature.max - temperature.min).toFixed(1);
  
      }
      
      else{
      temperature.max = temp[0].max.toFixed(1);
      temperature.min = temp[0].min.toFixed(1);
      temperature.avg = temp[0].avg.toFixed(1);
      temperature.range = temp[0].range.toFixed(1);}
  
      humidity.max = humid[0].max.toFixed(1);
      humidity.min = humid[0].min.toFixed(1);
      humidity.avg = humid[0].avg.toFixed(1);
      humidity.range = humid[0].range.toFixed(1);
  
      pressure.min = air[0].min.toFixed(1);
      pressure.max = air[0].max.toFixed(1);
      pressure.avg = air[0].avg.toFixed(1);
      pressure.range = air[0].range.toFixed(1);
  
      moisture.min = soil[0].min.toFixed(1);
      moisture.max = soil[0].max.toFixed(1);
      moisture.avg = soil[0].avg.toFixed(1);
      moisture.range = soil[0].range.toFixed(1);
    }
  };
  
  const updateHistogramCharts = async (rule) => {
    console.log("updateHistogramCharts function is being called!");
    if (!!start.value && !!end.value) {
        let startDate = new Date(start.value).getTime() / 1000;
        let endDate = new Date(end.value).getTime() / 1000;

        const temp = await AppStore.getFreqDistro("temperature", startDate, endDate);
      temperature.max = celsiusToFahrenheit(temp[0].max).toFixed(1);
        const humid = await AppStore.getFreqDistro("humidity", startDate, endDate);

        let temperature = [];
        let humidity = [];

        temp.forEach((row) => {
            let tempVal = rule ? celsiusToFahrenheit(row["_id"]) : row["_id"];
            temperature.push({ x: tempVal, y: row["count"] });
        });
        humid.forEach((row) => {
            let humidVal = rule ? celsiusToFahrenheit(row["_id"]) : row["_id"];
            humidity.push({ x: humidVal, y: row["count"] });
        });

        histo.value.series[0].setData(temperature);
        histo.value.series[1].setData(humidity);
    }
  };
  
  
  const celsiusToFahrenheit = (celsius) => {
        return (celsius * 9) / 5 + 32;
      };
  
  const convertData = async () => {
    isFarenheit.value = !isFarenheit.value;
    updateLineCharts(isFarenheit.value);
    updateCards(isFarenheit.value);
    updateHistogramCharts(isFarenheit.value);

    histo.value.xAxis[0].update({
        title: { 
            text: isFarenheit.value ? "Value  % / °F" : "Value  % / °C",
            style: { color: '#000000' }
        }
    });

    heatIndexLine.value.yAxis[0].update({
        title: { 
            text: isFarenheit.value ? 'Temperature (°F)' : 'Temperature (°C)',
            style: { color: '#000000' }
        },
        labels: { 
            format: isFarenheit.value ? '{value} °F' : '{value} °C' 
        }
    });
  };
  </script>
  
  <style scoped>
  /** CSS STYLE HERE */
  
  .container {
    width: 1200px;
  }
  
  .row {
    max-width: 1200px;
  }
  
  .ma-5 {
    display: flex;
    align-items: center;
    justify-content: center;
    margin: 0 auto;
  
  }
  
  .col1 {
    border: black;
  }
  
  .sheet {
    display: flex;;
    align-items: center;
    justify-content: center;
    height: 100px;
    width: 100%;
  }
  
  Figure {
    border: 2px solid black;
  }
  </style>