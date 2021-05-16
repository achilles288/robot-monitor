#include <rm/attribute.hpp>

#include <gtest/gtest.h>


/**
 * @brief Sets the value
 * 
 * Setting boolean values for attributes of different data types.
 */
TEST(Attribute, setValue_bool) {
    rmAttribute attr1("attr1", RM_ATTRIBUTE_BOOL);
    rmAttribute attr2("attr2", RM_ATTRIBUTE_STRING);
    attr1.setValue(true);
    EXPECT_TRUE(attr1.getValue().b);
    attr2.setValue(false);
    EXPECT_STREQ(attr2.getValue().s, "0");
}

/**
 * @brief Sets the value
 * 
 * Setting character values for attributes of different data types.
 */
TEST(Attribute, setValue_char) {
    rmAttribute attr1("attr1", RM_ATTRIBUTE_CHAR);
    rmAttribute attr2("attr2", RM_ATTRIBUTE_INT, 0, 32);
    rmAttribute attr3("attr3", RM_ATTRIBUTE_STRING);
    attr1.setValue('f');
    EXPECT_EQ(attr1.getValue().c, 'f');
    attr2.setValue('A');
    EXPECT_EQ(attr2.getValue().i, 32);
    attr3.setValue('R');
    EXPECT_STREQ(attr3.getValue().s, "R");
}

/**
 * @brief Sets the value
 * 
 * Setting integer values for attributes of different data types.
 */
TEST(Attribute, setValue_int) {
    rmAttribute attr1("attr1", RM_ATTRIBUTE_CHAR);
    rmAttribute attr2("attr2", RM_ATTRIBUTE_INT, -1024, 1024);
    rmAttribute attr3("attr3", RM_ATTRIBUTE_FLOAT, 0.0f, 100.0f);
    rmAttribute attr4("attr4", RM_ATTRIBUTE_STRING);
    attr1.setValue(48);
    EXPECT_EQ(attr1.getValue().c, '0');
    attr2.setValue(674);
    EXPECT_EQ(attr2.getValue().i, 674);
    attr2.setValue(1402);
    EXPECT_EQ(attr2.getValue().i, 1024);
    attr3.setValue(43);
    EXPECT_NEAR(attr3.getValue().f, 43, 0.001f);
    attr3.setValue(-85);
    EXPECT_NEAR(attr3.getValue().f, 0.0f, 0.001f);
    attr4.setValue(-95332);
    EXPECT_STREQ(attr4.getValue().s, "-95332");
}

/**
 * @brief Sets the value
 * 
 * Setting floating point values for attributes of different data types.
 */
TEST(Attribute, setValue_float) {
    rmAttribute attr1("attr2", RM_ATTRIBUTE_INT, 100, 800);
    rmAttribute attr2("attr3", RM_ATTRIBUTE_FLOAT, -5.0f, 17.0f);
    rmAttribute attr3("attr4", RM_ATTRIBUTE_STRING);
    attr1.setValue(673.356f);
    EXPECT_EQ(attr1.getValue().i, 673);
    attr1.setValue(30.432f);
    EXPECT_EQ(attr1.getValue().i, 100);
    attr2.setValue(9.854f);
    EXPECT_NEAR(attr2.getValue().f, 9.854f, 0.001f);
    attr2.setValue(32.001f);
    EXPECT_NEAR(attr2.getValue().f, 17.0f, 0.001f);
    attr3.setValue(59.2301f);
    EXPECT_STREQ(attr3.getValue().s, "59.230099");
}

/**
 * @brief Sets the value
 * 
 * Setting string values for attributes of different data types.
 */
TEST(Attribute, setValue_string) {
    rmAttribute attr1("attr1", RM_ATTRIBUTE_BOOL);
    rmAttribute attr2("attr2", RM_ATTRIBUTE_CHAR);
    rmAttribute attr3("attr3", RM_ATTRIBUTE_INT);
    rmAttribute attr4("attr4", RM_ATTRIBUTE_FLOAT);
    rmAttribute attr5("attr5", RM_ATTRIBUTE_STRING);
    attr1.setValue("1");
    EXPECT_TRUE(attr1.getValue().b);
    attr2.setValue("c");
    EXPECT_EQ(attr2.getValue().c, 'c');
    attr3.setValue("135673");
    EXPECT_EQ(attr3.getValue().i, 135673);
    attr4.setValue("3.14159");
    EXPECT_NEAR(attr4.getValue().f, 3.14159f, 0.001f);
    attr5.setValue("Robotics");
    EXPECT_STREQ(attr5.getValue().s, "Robotics");
}

/**
 * @brief Gets string value
 * 
 * Getting string values from attributes of various data types
 */
TEST(Attribute, getValueString) {
    rmAttribute attr1("attr1", RM_ATTRIBUTE_BOOL);
    rmAttribute attr2("attr2", RM_ATTRIBUTE_CHAR);
    rmAttribute attr3("attr3", RM_ATTRIBUTE_INT);
    rmAttribute attr4("attr4", RM_ATTRIBUTE_FLOAT);
    rmAttribute attr5("attr5", RM_ATTRIBUTE_STRING);
    attr1.setValue(false);
    EXPECT_STREQ(attr1.getValueString().c_str(), "0");
    attr2.setValue('y');
    EXPECT_STREQ(attr2.getValueString().c_str(), "y");
    attr3.setValue(-535);
    EXPECT_STREQ(attr3.getValueString().c_str(), "-535");
    attr4.setValue(2.0034f);
    EXPECT_STREQ(attr4.getValueString().c_str(), "2.003400");
    attr5.setValue("Hello");
    EXPECT_STREQ(attr5.getValueString().c_str(), "Hello");
}
