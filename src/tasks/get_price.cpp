#include "get_price.hpp"

const char *apiUrl = "https://api.coindesk.com/v1/bpi/currentprice/USD.json";

std::vector<EventCallbackWithNumber> priceEventCallbacks; // Define a vector to hold multiple event callbacks
TaskHandle_t getPriceTaskHandle;

#ifdef IS_3C
#define PRICE_WAIT_TIME 300000
#else
#define PRICE_WAIT_TIME 60000
#endif

void taskGetPrice(void *pvParameters)
{
    HTTPClient http;
    for (;;)
    {
        // Send HTTP request to CoinDesk API
        http.begin(apiUrl);
        int httpCode = http.GET();

        // Parse JSON response and extract average price
        float price;
        if (httpCode == 200)
        {
            String payload = http.getString();
            //Serial.println(payload);
            StaticJsonDocument<768> doc;
            deserializeJson(doc, payload);
            JsonObject bpi = doc["bpi"];
            price = bpi["USD"]["rate_float"];
            for(auto &callback : priceEventCallbacks) { // Loop through all the event callbacks and call them
                callback(price);
            }

            preferences.putFloat("btcPrice", price);
        }
        else
        {
            Serial.print("Error retrieving BTC/USD price. HTTP status code: ");
            Serial.println(httpCode);
        }

        // Disconnect from Wi-Fi network and wait for 60 seconds
        http.end();
        
        vTaskDelay(pdMS_TO_TICKS(PRICE_WAIT_TIME));
    }
}

void setupGetPriceTask()
{
    if (getPriceTaskHandle == nullptr) {
        xTaskCreate(taskGetPrice, "getPrice", 8192, NULL, 1, &getPriceTaskHandle);
        vTaskSuspend(getPriceTaskHandle);
    }
}

void registerNewPriceCallback(EventCallbackWithNumber cb)
{
  priceEventCallbacks.push_back(cb);
}