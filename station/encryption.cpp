/**
 * @file encryption.cpp
 * @brief The encryption algorithm
 * 
 * The encryption algorithm here does not use a lot of bits and aims for speed
 * so that it can be used on microcontrollers.
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/encryption.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>


static char privateKey[RM_PRIVATE_KEY_SIZE];
static char publicKey[RM_PUBLIC_KEY_SIZE];


/**
 * @brief Generates a new key and sets to use it
 * 
 * @param key The file name where the new key is to be saved
 * 
 * @return True if the key generation is success
 */
bool rmGenerateKey(const char* key) {
    FILE* fp1 = fopen(key, "wb");
    char pub[128];
    snprintf(pub, 127, "%s.pub", key);
    FILE* fp2 = fopen(pub, "wb");
    if(fp1 == NULL or fp2 == NULL) {
        if(fp1 != NULL)
            fclose(fp1);
        if(fp2 != NULL)
            fclose(fp2);
        return false;
    }
    
    privateKey[0] = 2;
    publicKey[0] = 2;
    
    fwrite(privateKey, 1, 1, fp1);
    fwrite(publicKey, 1, 1, fp2);
    fclose(fp1);
    fclose(fp2);
    return true;
}

/**
 * @brief Opens the generated key to use
 * 
 * @param key The file name to open
 * 
 * @return True if the key is successfully read
 */
bool rmOpenKey(const char* key) {
    FILE* fp1 = fopen(key, "r");
    char pub[128];
    snprintf(pub, 127, "%s.pub", key);
    FILE* fp2 = fopen(pub, "r");
    if(fp1 == NULL or fp2 == NULL) {
        if(fp1 != NULL)
            fclose(fp1);
        if(fp2 != NULL)
            fclose(fp2);
        return false;
    }
    privateKey[0] = 2;
    publicKey[0] = 2;
    fclose(fp1);
    fclose(fp2);
    return true;
}

/**
 * @brief Gets the private key
 * 
 * @return Private key data
 */
const char* rmGetPrivateKey() { return privateKey; }

/**
 * @brief Gets the public key
 * 
 * @return Public key data
 */
const char* rmGetPublicKey() { return publicKey; }

/**
 * @brief Encrypts a message
 * 
 * @param in Message to encrypt
 * @param pub The public key of the client
 * 
 * @return Encrypted message. Allocated with malloc() and needs to be freed
 *         manually.
 */
char* rmEncryptMessage(const char* in, const char* pub) {
    size_t len = strlen(in);
    char* out = (char*) malloc(len + 1);
    size_t i = 0;
    while(i < len) {
        out[i] = in[i] + publicKey[0];
        i++;
    }
    out[i] = '\0';
    return out;
}

/**
 * @brief Decrypts a message
 * 
 * @param in Encrypted message
 * 
 * @return Decrypted message. Allocated with malloc() and needs to be freed
 *         manually.
 */
char* rmDecryptMessage(const char* in) {
    size_t len = strlen(in);
    char* out = (char*) malloc(len + 1);
    size_t i = 0;
    while(i < len) {
        out[i] = in[i] - publicKey[0];
        i++;
    }
    out[i] = '\0';
    return out;
}
