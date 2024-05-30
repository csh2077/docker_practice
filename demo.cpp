#include <openssl/aes.h>
#include <openssl/rand.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

void print_data(const char* title, const void* data, int len) {
    std::cout << title;
    const unsigned char* p = (const unsigned char*)data;
    for (int i = 0; i < len; ++i)
        printf("%02X ", *p++);
    printf("\n");
}

void print_info(std::string str, std::vector<uint8_t> &enc, std::vector<uint8_t> &dnc) {
    print_data("Plaintext: ", reinterpret_cast<const unsigned char*>(str.c_str()), str.size());
    print_data("Encrypted: ", enc.data(), enc.size());
    print_data("Decrypted: ", dnc.data(), dnc.size());
    printf("\n");
}

int main(int argc, char *argv[]) {
    int ret;
    uint8_t key[AES_BLOCK_SIZE];
    uint8_t iv[AES_BLOCK_SIZE];
    uint8_t tmp_iv[AES_BLOCK_SIZE];
    std::string text = "12345678901234567890";
    std::vector<uint8_t> encryptedData(text.size() + AES_BLOCK_SIZE);
    std::vector<uint8_t> decryptedData(text.size() + AES_BLOCK_SIZE);

    RAND_bytes(iv, sizeof(iv));
    RAND_bytes(key, sizeof(key));

    print_data("Key: ", key, sizeof(key));
    print_data("IV: ", iv, sizeof(iv));

    uint32_t len = text.size();
    if (len % AES_BLOCK_SIZE != 0) {
        uint32_t padding_len = AES_BLOCK_SIZE - len % AES_BLOCK_SIZE;
        text.append(padding_len, '\0');
    }

    AES_KEY aesKey;
    AES_set_encrypt_key(key, 128, &aesKey);
    AES_KEY desKey;
    AES_set_decrypt_key(key, 128, &desKey);

    memcpy(tmp_iv, iv, AES_BLOCK_SIZE);
    AES_cbc_encrypt(reinterpret_cast<const unsigned char*>(text.data()),
                    encryptedData.data(), text.size(), &aesKey, tmp_iv, AES_ENCRYPT);
    memcpy(tmp_iv, iv, AES_BLOCK_SIZE);
    AES_cbc_encrypt(encryptedData.data(), decryptedData.data(), text.size(), &desKey, tmp_iv, AES_DECRYPT);

    print_info(text, encryptedData, decryptedData);


    AES_ecb_encrypt(reinterpret_cast<const unsigned char*>(text.data()),
                    encryptedData.data(), &aesKey, AES_ENCRYPT);
    AES_ecb_encrypt(encryptedData.data(), decryptedData.data(), &desKey, AES_DECRYPT);

    print_info(text, encryptedData, decryptedData);


    int num = 0;
    memcpy(tmp_iv, iv, AES_BLOCK_SIZE);
    AES_cfb128_encrypt(reinterpret_cast<const unsigned char*>(text.data()), encryptedData.data(),
                        text.size(),  &aesKey,
                        tmp_iv, &num, AES_ENCRYPT);
    num = 0;
    memcpy(tmp_iv, iv, AES_BLOCK_SIZE);
    AES_cfb128_encrypt(encryptedData.data(), decryptedData.data(), text.size(),
                        &aesKey, tmp_iv, &num, AES_DECRYPT);
    print_info(text, encryptedData, decryptedData);


    num = 0;
    memcpy(tmp_iv, iv, AES_BLOCK_SIZE);
    AES_ofb128_encrypt(reinterpret_cast<const unsigned char*>(text.data()), encryptedData.data(),
                        text.size(),  &aesKey,
                        tmp_iv, &num);
    num = 0;
    memcpy(tmp_iv, iv, AES_BLOCK_SIZE);
    AES_ofb128_encrypt(encryptedData.data(), decryptedData.data(), text.size(),
                        &aesKey, tmp_iv, &num);
    print_info(text, encryptedData, decryptedData);

    return ret;
}