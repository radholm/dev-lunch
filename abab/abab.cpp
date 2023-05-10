#include <emscripten/fetch.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <emscripten/emscripten.h>

typedef void (*ResponseCallback)(const char*);

void onResponse(emscripten_fetch_t *fetch)
{
    if (fetch->status == 200)
    {
        std::string response(fetch->data, fetch->numBytes);
        // Call the response callback with the received data
        if (fetch->userData != nullptr)
        {
            ResponseCallback callback = reinterpret_cast<ResponseCallback>(fetch->userData);
            callback(response.c_str());
        }
    }
    else
    {
        printf("HTTP request failed with status code: %d\n", fetch->status);
        // Call the response callback with an empty response
        if (fetch->userData != nullptr)
        {
            ResponseCallback callback = reinterpret_cast<ResponseCallback>(fetch->userData);
            callback("");
        }
    }

    // Clean up the fetch object
    emscripten_fetch_close(fetch);
}

#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

EXTERN EMSCRIPTEN_KEEPALIVE void makeApiCall(ResponseCallback callback)
{
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = onResponse;
    attr.userData = reinterpret_cast<void*>(callback);

    const char* url = "https://api.openai.com/v1/engines/davinci/completions";
    const char* headers[] = {"Content-Type", "application/json", "Authorization", "Bearer ", nullptr};
    attr.requestHeaders = headers;

    const char* data = R"({"prompt": "Hello!", "max_tokens": 256})";
    attr.requestData = data;
    attr.requestDataSize = strlen(data);
    std::strcpy(attr.requestMethod, "POST");

    emscripten_fetch(&attr, url);
}

int main()
{
    return 0;
}
