#include <stdio.h>
#include <stdlib.h>

void generate_payload(const char *filename, const unsigned char *data, size_t size) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    size_t written = fwrite(data, sizeof(unsigned char), size, file);
    if (written != size) {
        perror("Error writing to file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    printf("Payload written to %s successfully!\n", filename);
    fclose(file);
}

int main() {
    // Define your payload here (example: shellcode or arbitrary data)
    unsigned char payload[] = {
        0x90, 0x90, 0x90, 0x90,  // NOP sled
        0xCC,                    // INT3 (breakpoint for debugging)
        0x41, 0x42, 0x43, 0x44   // Example data ("ABCD" in ASCII)
    };

    size_t payload_size = sizeof(payload) / sizeof(payload[0]);

    // Output file for the payload
    const char *output_filename = "payload.bin";

    // Generate the payload
    generate_payload(output_filename, payload, payload_size);

    return 0;
}
