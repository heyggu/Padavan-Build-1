#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void rc4_encrypt(const char *data, const char *key, char *output_hex) {
    unsigned char s_box[256];
    int i, j = 0;
    unsigned long key_len = strlen(key);
    unsigned long data_len = strlen(data);

    for (i = 0; i < 256; i++) {
        s_box[i] = i;
    }

    for (i = 0; i < 256; i++) {
        j = (j + s_box[i] + key[i % key_len]) % 256;
        unsigned char temp = s_box[i];
        s_box[i] = s_box[j];
        s_box[j] = temp;
    }

    i = 0;
    j = 0;
    for (int k = 0; k < data_len; k++) {
        i = (i + 1) % 256;
        j = (j + s_box[i]) % 256;
        unsigned char temp_s = s_box[i];
        s_box[i] = s_box[j];
        s_box[j] = temp_s;
        unsigned char keystream_byte = s_box[(s_box[i] + s_box[j]) % 256];
        unsigned char encrypted_byte = data[k] ^ keystream_byte;
        sprintf(output_hex + (k * 2), "%02x", encrypted_byte);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <plaintext_password> <key_auth_tag>\n", argv[0]);
        return 1;
    }
    const char *password = argv[1];
    const char *key = argv[2];
    char *hex_output = malloc(strlen(password) * 2 + 1);
    if (hex_output == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    rc4_encrypt(password, key, hex_output);
    printf("%s\n", hex_output);
    free(hex_output);
    return 0;
}