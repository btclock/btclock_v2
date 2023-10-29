#include "get_price.hpp"

const PROGMEM char *apiUrl = "https://api.coindesk.com/v1/bpi/currentprice/USD.json";
const PROGMEM char *cgApiUrl = "https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd%2Ceur";

std::vector<EventCallbackWithNumber> priceEventCallbacks; // Define a vector to hold multiple event callbacks
TaskHandle_t getPriceTaskHandle;

#ifdef IS_3C
#define PRICE_WAIT_TIME 300000
#else
#define PRICE_WAIT_TIME 60000
#endif

void taskGetPrice(void *pvParameters)
{
    IPAddress result;

    int err = WiFi.hostByName("api.coingecko.com", result) ;

    if (err != 1) {
        flashTemporaryLights(255, 255, 0);
    }

    for (;;)
    {
        HTTPClient* http = new HTTPClient();
        http->setUserAgent(USER_AGENT);

        if (true)
        {
            // Send HTTP request to CoinGecko API
            http->begin(cgApiUrl);

            int httpCode = http->GET();

            // Parse JSON response and extract average price
            float usdPrice, eurPrice;
            if (httpCode == 200)
            {
                String payload = http->getString();
                SpiRamJsonDocument doc(768);
                deserializeJson(doc, payload);
                JsonObject bpi = doc["bitcoin"];
                usdPrice = bpi["usd"];
                eurPrice = bpi["eur"];
                for (EventCallbackWithNumber &callback : priceEventCallbacks)
                { // Loop through all the event callbacks and call them
                    callback(usdPrice);
                }

                preferences.putFloat("btcPrice", usdPrice);
                preferences.putFloat("btcPriceEur", eurPrice);
            }
            else
            {
                Serial.print(F("Error retrieving BTC/USD price (CoinGecko). HTTP status code: "));
                Serial.println(httpCode);
            }
        } else {

            // Send HTTP request to CoinDesk API
            http->begin(apiUrl);

            int httpCode = http->GET();

            // Parse JSON response and extract average price
            float usdPrice, eurPrice;
            if (httpCode == 200)
            {
                String payload = http->getString();
                SpiRamJsonDocument doc(768);
                deserializeJson(doc, payload);
                JsonObject bpi = doc["bpi"];
                usdPrice = bpi["USD"]["rate_float"];
                eurPrice = bpi["EUR"]["rate_float"];
                for (EventCallbackWithNumber &callback : priceEventCallbacks)
                { // Loop through all the event callbacks and call them
                    callback(usdPrice);
                }

                preferences.putFloat("btcPrice", usdPrice);
                preferences.putFloat("btcPriceEur", eurPrice);
            }
            else
            {
                Serial.print(F("Error retrieving BTC/USD price (CoinDesk). HTTP status code: "));
                Serial.println(httpCode);
            }
        }

        http->end();

        delete http;

        vTaskDelay(pdMS_TO_TICKS(PRICE_WAIT_TIME));
    }
}

void setupGetPriceTask()
{
    if (getPriceTaskHandle == nullptr)
    {
        xTaskCreate(taskGetPrice, "getPrice", 6144, NULL, 1, &getPriceTaskHandle);
        vTaskSuspend(getPriceTaskHandle);
    }
}

void registerNewPriceCallback(const EventCallbackWithNumber cb)
{
    priceEventCallbacks.push_back(cb);
}