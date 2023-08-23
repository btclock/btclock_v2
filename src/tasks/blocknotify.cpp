#include "blocknotify.hpp"

int currentBlockHeight = 789000;
QueueHandle_t bitcoinQueue;
BitcoinEvent bitcoinEvent;
const String NEW_BLOCK_MINED_EVENT = "new_block_mined";
std::vector<EventCallbackWithNumber> blockEventCallbacks;
TaskHandle_t blockNotifyTaskHandle;

#ifdef IS_3C
#define BLOCKNOTIFY_WAIT_TIME 300000
#else
#define BLOCKNOTIFY_WAIT_TIME 60000
#endif

bool useBitcoind = true;

void checkBitcoinBlock(void *pvParameters)
{
    int blockHeight = preferences.getUInt("blockHeight", currentBlockHeight);
 
    HTTPClient http;
    http.setReuse(true);
    useBitcoind = wifiClientInsecure.connect(preferences.getString("rpcHost", BITCOIND_HOST).c_str(), preferences.getUInt("rpcPort", BITCOIND_PORT));
    if (useBitcoind)
        Serial.println("bitcoind node is reachable, using this for blocks.");
    else
        Serial.println("bitcoind node is not reachable, using mempool API instead.");

    for (;;)
    {
        if (useBitcoind)
        {
            StaticJsonDocument<200> jsonDoc;

            http.begin(preferences.getString("rpcHost", BITCOIND_HOST).c_str(), preferences.getUInt("rpcPort", BITCOIND_PORT));
            http.addHeader("Content-Type", "application/json");
            http.addHeader("User-Agent", "BTClock/1.0");

            String payload = "{\"jsonrpc\":\"1.0\",\"id\":\"current_block_height\",\"method\":\"getblockcount\",\"params\":[]}";
            String auth = preferences.getString("rpcUser", BITCOIND_RPC_USER) + ":" + preferences.getString("rpcPass", BITCOIND_RPC_PASS);
            String authEncoded = base64::encode(auth);
            http.addHeader("Authorization", "Basic " + authEncoded);

            int httpCode = http.POST(payload);
            if (httpCode > 0 || httpCode != HTTP_CODE_UNAUTHORIZED)
            {
                String response = http.getString();
                deserializeJson(jsonDoc, response);
                blockHeight = jsonDoc["result"];
            }
            else
            {
                Serial.println("Error in HTTP request to bitcoind");
            }

            http.end();
        }
        else
        {
            http.begin("https://mempool.bitcoin.nl/api/blocks/tip/height");
            http.addHeader("User-Agent", "BTClock/1.0");
            int httpCode = http.GET();

            if (httpCode > 0 && httpCode == HTTP_CODE_OK)
            {
                String blockHeightStr = http.getString();
                blockHeight = blockHeightStr.toInt();
            }
            else
            {
                Serial.println("Error in HTTP request to mempool API");
            }

            http.end();
        }
        if (blockHeight > currentBlockHeight)
        {
            for (auto &callback : blockEventCallbacks)
            { // Loop through all the event callbacks and call them
                callback(blockHeight);
            }
            currentBlockHeight = blockHeight;
            preferences.putUInt("blockHeight", currentBlockHeight);
        }
        vTaskDelay(pdMS_TO_TICKS(BLOCKNOTIFY_WAIT_TIME)); // wait 1 minute before checking again
    }
}

// void bitcoinEventHandler(void *pvParameters)
// {
//     for (;;)
//     {
//         BitcoinEvent event;
//         xQueueReceive(bitcoinQueue, &event, portMAX_DELAY);
//         if (event.event == "new_block_mined")
//         {
//             Serial.println("New Bitcoin block mined at height: " + String(event.blockHeight));
//             Serial.println("Event: " + event.event);
//         }
//         // add your event handling logic here
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

void setupBlockNotify()
{
    // bitcoinQueue = xQueueCreate(10, sizeof(BitcoinEvent) * 2);

    if (blockNotifyTaskHandle == nullptr)
    {
        xTaskCreate(checkBitcoinBlock, "checkBitcoinBlock", 4096, NULL, 1, &blockNotifyTaskHandle);
        vTaskSuspend(blockNotifyTaskHandle);
    }
    //  xTaskCreate(bitcoinEventHandler, "bitcoinEventHandler", 10000, NULL, 110, NULL);
}

void registerNewBlockCallback(EventCallbackWithNumber cb)
{
    blockEventCallbacks.push_back(cb);
}