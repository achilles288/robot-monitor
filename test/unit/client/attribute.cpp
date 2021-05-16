#include "robotmonitor.h"

#include <gtest/gtest.h>


/**
 * @brief Sets the value
 */
TEST(Attribute, setBool) {
    rmAttribute attr1;
    strcpy(attr1.name, "attr1");
    attr1.type = RM_ATTRIBUTE_BOOL;
    
    rmAttributeSetBool(&attr1, true);
    EXPECT_TRUE(attr1.data.b);
    rmAttributeSetBool(&attr1, false);
    EXPECT_FALSE(attr1.data.b);
    rmAttributeSetValue(&attr1, "1");
    EXPECT_TRUE(attr1.data.b);
    rmAttributeSetValue(&attr1, "0");
    EXPECT_FALSE(attr1.data.b);
}

/**
 * @brief Sets the value
 */
TEST(Attribute, setChar) {
    rmAttribute attr1;
    strcpy(attr1.name, "attr1");
    attr1.type = RM_ATTRIBUTE_CHAR;
    
    rmAttributeSetChar(&attr1, 'f');
    EXPECT_EQ(attr1.data.c, 'f');
    rmAttributeSetChar(&attr1, 'A');
    EXPECT_EQ(attr1.data.c, 'A');
    rmAttributeSetValue(&attr1, "@");
    EXPECT_EQ(attr1.data.c, '@');
    rmAttributeSetValue(&attr1, "g");
    EXPECT_EQ(attr1.data.c, 'g');
}

/**
 * @brief Sets the value
 */
TEST(Attribute, setInt) {
    rmAttribute attr1 = {"attr1", {.i=0}, RM_ATTRIBUTE_INT, {.i=100},
                         {.i=600}, NULL};
    
    rmAttributeSetInt(&attr1, 126);
    EXPECT_EQ(attr1.data.i, 126);
    rmAttributeSetInt(&attr1, 785);
    EXPECT_EQ(attr1.data.i, 600);
    rmAttributeSetInt(&attr1, 43);
    EXPECT_EQ(attr1.data.i, 100);
    rmAttributeSetValue(&attr1, "352");
    EXPECT_EQ(attr1.data.i, 352);
    rmAttributeSetValue(&attr1, "905");
    EXPECT_EQ(attr1.data.i, 600);
    rmAttributeSetValue(&attr1, "32");
    EXPECT_EQ(attr1.data.i, 100);
}

/**
 * @brief Sets the value
 */
TEST(Attribute, setFloat) {
    rmAttribute attr1 = {"attr1", {.f=0}, RM_ATTRIBUTE_FLOAT, {.f=-50.08f},
                         {.f=124.87f}, NULL};
    
    rmAttributeSetFloat(&attr1, 32.053f);
    EXPECT_NEAR(attr1.data.f, 32.053f, 0.001f);
    rmAttributeSetFloat(&attr1, 302.053f);
    EXPECT_NEAR(attr1.data.f, 124.87f, 0.001f);
    rmAttributeSetFloat(&attr1, -82.402f);
    EXPECT_NEAR(attr1.data.f, -50.08f, 0.001f);
    rmAttributeSetValue(&attr1, "74.023");
    EXPECT_NEAR(attr1.data.f, 74.023f, 0.001f);
    rmAttributeSetValue(&attr1, "202.053");
    EXPECT_NEAR(attr1.data.f, 124.87f, 0.001f);
    rmAttributeSetValue(&attr1, "-82.402");
    EXPECT_NEAR(attr1.data.f, -50.08f, 0.001f);
}

/**
 * @brief Sets the value
 */
TEST(Attribute, setString) {
    rmAttribute attr1;
    strcpy(attr1.name, "attr1");
    attr1.data.s = NULL;
    attr1.type = RM_ATTRIBUTE_STRING;
    
    rmAttributeSetString(&attr1, "Hello");
    EXPECT_STREQ(attr1.data.s, "Hello");
    rmAttributeSetString(&attr1, "Blah blah");
    EXPECT_STREQ(attr1.data.s, "Blah blah");
    rmAttributeSetValue(&attr1, "Robotics");
    EXPECT_STREQ(attr1.data.s, "Robotics");
    free(attr1.data.s);
}
