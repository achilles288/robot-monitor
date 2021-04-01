/**
 * @file encryption.hpp
 * @brief The encryption algorithm
 * 
 * The encryption algorithm here does not use a lot of bits and aims for speed
 * so that it can be used on microcontrollers.
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_ENCRYPTION_H__
#define __RM_ENCRYPTION_H__ ///< Header guard

#ifndef RM_API
#ifdef _WIN32
#ifdef RM_EXPORT
#define RM_API __declspec(dllexport) ///< API
#else
#define RM_API __declspec(dllimport) ///< API
#endif
#else
#define RM_API ///< API
#endif
#endif


#define RM_PRIVATE_KEY_SIZE 32
#define RM_PUBLIC_KEY_SIZE 8


/**
 * @brief Generates a new key and sets to use it
 * 
 * @param key The file name where the new key is to be saved
 * 
 * @return True if the key generation is success
 */
RM_API bool rmGenerateKey(const char* key);

/**
 * @brief Opens the generated key to use
 * 
 * @param key The file name to open
 * 
 * @return True if the key is successfully read
 */
RM_API bool rmOpenKey(const char* key);

/**
 * @brief Gets the private key
 * 
 * @return Private key data
 */
RM_API const char* rmGetPrivateKey();

/**
 * @brief Gets the public key
 * 
 * @return Public key data
 */
RM_API const char* rmGetPublicKey();

/**
 * @brief Encrypts a message
 * 
 * @param in Message to encrypt
 * @param pub The public key of the client
 * 
 * @return Encrypted message. Allocated with malloc() and needs to be freed
 *         manually.
 */
RM_API char* rmEncryptMessage(const char* in, const char* pub);

/**
 * @brief Decrypts a message
 * 
 * @param in Encrypted message
 * 
 * @return Decrypted message. Allocated with malloc() and needs to be freed
 *         manually.
 */
RM_API char* rmDecryptMessage(const char* in);

#endif
