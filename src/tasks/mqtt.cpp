// #include "mqtt.hpp"

// // Define the event that will be emitted when the property is changed
// //EventHandle_t property_changed_event;

// // Define the MQTT client object
// AsyncMqttClient mqtt_client;

// // Define the FreeRTOS task that will handle the MQTT client
// void mqtt_task(void* pvParameters) {
//   // Connect to the MQTT broker
//   mqtt_client.setServer(MQTT_HOST, 1883);
//   #ifdef MQTT_PASSWORD
//   mqtt_client.setCredentials(MQTT_USERNAME, MQTT_PASSWORD);
//   #endif

//   mqtt_client.connect();

//   while (!mqtt_client.connected()) {
//     delay(1000);
//   }

//   // Subscribe to the MQTT topic
//   mqtt_client.subscribe(MQTT_TOPIC, 0);

//   while (1) {
//     // Check for incoming MQTT messages
//     vTaskDelay(pdMS_TO_TICKS(100));
//   }
// }