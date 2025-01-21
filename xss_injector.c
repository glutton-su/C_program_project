#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char payload[100];
    char injected_payload[200];

    printf("Enter the XSS payload: ");
    fgets(payload, sizeof(payload), stdin);

    // Remove the newline character from the input
    payload[strcspn(payload, "\n")] = '\0';

    // Construct the injected payload
    snprintf(injected_payload, sizeof(injected_payload), "<script>alert('%s');</script>", payload);

    printf("Injected Payload: %s\n", injected_payload);

    return 0;
}
