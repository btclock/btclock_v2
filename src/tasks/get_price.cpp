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
        http.addHeader("User-Agent", "BTClock/1.0");

        int httpCode = http.GET();

        // Parse JSON response and extract average price
        float usdPrice, eurPrice;
        if (httpCode == 200)
        {
            String payload = http.getString();
            StaticJsonDocument<768> doc;
            deserializeJson(doc, payload);
            JsonObject bpi = doc["bpi"];
            usdPrice = bpi["USD"]["rate_float"];
            eurPrice = bpi["EUR"]["rate_float"];
            for(auto &callback : priceEventCallbacks) { // Loop through all the event callbacks and call them
                callback(usdPrice);
            }

            preferences.putFloat("btcPrice", usdPrice);
            preferences.putFloat("btcPriceEur", eurPrice);
        }
        else
        {
            Serial.print("Error retrieving BTC/USD price. HTTP status code: ");
            Serial.println(httpCode);
        }

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