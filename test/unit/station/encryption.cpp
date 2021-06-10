#define RM_NO_WX

#include <rm/encryption.hpp>

#include <gtest/gtest.h>


/**
 * @brief Encrypts a message
 */
TEST(Encryption, encrypt) {
    bool keyGen = rmGenerateKey("mykey");
    ASSERT_TRUE(keyGen);
    
    const char* message = "I love Robotics";
    const char pubKey2[] = "3d05a119";
    char* encrypted = rmEncryptMessage(message, pubKey2);
    
    ASSERT_NE(nullptr, encrypted);
    size_t len = strlen(encrypted);
    EXPECT_GE(15, len);
    for(size_t i=0; i<15 && i<len; i++) {
        if(encrypted[i] == message[i]) {
            std::cout << message << std::endl;
            std::cout << encrypted << std::endl;
            free(encrypted);
            FAIL() << "bad encryption";
        }
    }
    std::cout << message << std::endl;
    std::cout << encrypted << std::endl;
    
    free(encrypted);
}


/**
 * @brief Encrypts and decrypts a message
 */
TEST(Encryption, decrypt) {
    bool keyGen = rmGenerateKey("mykey");
    ASSERT_TRUE(keyGen);
    
    const char* message = "I love Robotics";
    const char pubKey2[] = "3d05a119";
    
    char* encrypted = rmEncryptMessage(message, pubKey2);
    
    ASSERT_NE(nullptr, encrypted);
    
    char* decrypted = rmDecryptMessage(encrypted);
    
    free(encrypted);
    ASSERT_NE(nullptr, decrypted);
    EXPECT_STREQ(message, decrypted);
    free(decrypted);
}
