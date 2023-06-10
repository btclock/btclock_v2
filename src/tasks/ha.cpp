// #include "ha.hpp"

// WebSocketsClient webSocket;

// TaskHandle_t websocket_task_handle = NULL;
// const uint32_t WEBSOCKET_TASK_STACK_SIZE = 4096;
// const uint32_t WEBSOCKET_TASK_PRIORITY = 1;

// // WebSocket connection and subscription messages
// String auth_msg = String("{\"type\": \"auth\",\"access_token\": \"") + String(HA_AUTH_TOKEN) + String("\"}");
// String subscribe_msg = String("{\"id\": 1,\"type\": \"subscribe_events\",\"event_type\": \"state_changed\"}");

// void websocket_task(void *pvParameters)
// {
//     // Connect to WiFi network
//     // Connect to Home Assistant WebSocket API
//     webSocket.begin(HA_SERVER, 8123, "/api/websocket");
//     webSocket.setReconnectInterval(5000);

//     // Authenticate and subscribe to entity state changes
//     webSocket.onEvent([&](WStype_t type, uint8_t *payload, size_t length)
//                       {
//         switch (type)
//         {
//         case WStype_DISCONNECTED:
//             Serial.printf("[WebSocket] disconnected\n");
//             break;
//         case WStype_CONNECTED:
//             Serial.printf("[WebSocket] connected, sending authentication message...\n");
//             webSocket.sendTXT(auth_msg);
//             break;
//         case WStype_TEXT:
//             if (strstr((char *)payload, "auth_ok") != NULL)
//             {
//                 Serial.printf("[WebSocket] authentication successful, subscribing to entity state changes...\n");
//                 webSocket.sendTXT(subscribe_msg);
//             }
//             else if (strstr((char *)payload, "auth_invalid") != NULL)
//             {
//                 Serial.printf("[WebSocket] authentication failed, disconnecting...\n");
//                 webSocket.disconnect();
//             }
//             else
//             {
//                 StaticJsonDocument<1536> jsonDoc;
//                 DeserializationError err = deserializeJson(jsonDoc, (char *)payload);

//                 if (!err)
//                 {
//                     const char *event_type = jsonDoc["event"]["event_type"];
//                     const char *entity_id = jsonDoc["event"]["data"]["entity_id"];

//                     if (event_type && entity_id && strcmp(event_type, "state_changed") != 0)
//                         return;

//                     if (event_type && entity_id && strcmp(event_type, "state_changed") == 0 && strcmp(entity_id, HA_ENTITY_ID) == 0)
//                     {
//                         const char *new_state = jsonDoc["event"]["data"]["new_state"]["state"];
//                         if (new_state)
//                         {
//                             Serial.printf("[WebSocket] entity state changed: %s\n", new_state);
//                             /* Emit event for state change */
//                             // TODO: Add your custom event handling code here
//                         }
//                     }
//                 } else {
//                    Serial.printf("[WebSocket] error deserializing JSON: %s\n", err.c_str());
//                 }
//             } 
//             break;
//         default:
//             break;
//             } 
//         });

//     // Main event loop
//     while (true)
//     {
//         webSocket.loop();
//         vTaskDelay(pdMS_TO_TICKS(10));
//     }
// }
