#pragma once

#include "ESPAsyncWebServer.h"

class OneParamRewrite : public AsyncWebRewrite
{
protected:
    String _urlPrefix;
    int _paramIndex;
    String _paramsBackup;

public:
    OneParamRewrite(const char *from, const char *to);
    bool match(AsyncWebServerRequest *request) override;
};