#include <emscripten.h>
#include <emscripten/fetch.h>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>

// Callback function for handling the fetch response
void fetchCallback(emscripten_fetch_t *fetch) {
    if (fetch->status == 200) {
        EM_ASM(
            {
                const responseArray = new Uint8Array(Module.HEAPU8.buffer, $0, $1);
                const responseStr = new TextDecoder('utf-8').decode(responseArray);
                Module['handleResponse'](responseStr);
            },
            fetch->data,
            fetch->numBytes
        );
    } else {
        EM_ASM(
            {
                console.log("Fetch request failed with status code: " + $0);
            },
            fetch->status
        );
    }

    emscripten_fetch_close(fetch);  // Free the fetch data
}

// Function to send a fetch request to the OpenAI API
extern "C" {
    EMSCRIPTEN_KEEPALIVE void sendFetchRequest(const char* apiKey, const char* prompt) {
        std::string apiUrl = "https://api.openai.com/v1/engines/davinci/completions";
        std::string headers = "application/json";
        std::string data = "{\"prompt\": \"" + std::string(prompt) + "\", \"max_tokens\": 128}";

        EM_ASM(
            {
                const apiKeyStr = UTF8ToString($0);
                const promptStr = UTF8ToString($1);
                const apiUrlStr = UTF8ToString($2);
                const headersStr = UTF8ToString($3);
                const dataStr = UTF8ToString($4);

                const xhr = new XMLHttpRequest();
                xhr.open("POST", apiUrlStr);
                xhr.setRequestHeader("Content-Type", headersStr);
                xhr.setRequestHeader("Authorization", "Bearer " + apiKeyStr);

                xhr.onload = function() {
                    if (xhr.status === 200) {
                        Module['handleResponse'](xhr.responseText);
                    } else {
                        console.log("Fetch request failed with status code: " + xhr.status);
                    }
                };

                xhr.onerror = function() {
                    console.log("Fetch request failed");
                };

                xhr.send(dataStr);
            },
            apiKey,
            prompt,
            apiUrl.c_str(),
            headers.c_str(),
            data.c_str()
        );
    }
}

// Function called from JavaScript to handle the response
extern "C" {
    EMSCRIPTEN_KEEPALIVE void handleResponse(const char* response) {
        // Print the response to the console
        printf("Received response: %s\n", response);
    }
}
