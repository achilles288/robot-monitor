#include <rm/client.hpp>

#include <gtest/gtest.h>


/**
 * @brief Gets the requested attribute
 * 
 * A list of attributes is created first. The function is validated by checking
 * if the name of the returned attribute has the same name as requested.
 */
TEST(Client, getAttribute) {
    rmClient client;
    rmAttribute* attr1 = client.createAttribute("pos", RM_ATTRIBUTE_FLOAT);
    rmAttribute* attr2 = client.createAttribute("speed", RM_ATTRIBUTE_FLOAT, 0,
                                                90);
    rmAttribute* attr3 = client.createAttribute("name", RM_ATTRIBUTE_STRING);
    rmAttribute* attr4 = client.createAttribute("index", RM_ATTRIBUTE_INT);
    rmAttribute* attr5 = client.createAttribute("char_a", RM_ATTRIBUTE_CHAR);
    rmAttribute* attr6 = client.createAttribute("option1", RM_ATTRIBUTE_BOOL);
    
    rmAttribute* attr;
    attr = client.getAttribute("voltage");
    EXPECT_EQ(nullptr, attr);
    attr = client.getAttribute("speed");
    ASSERT_NE(nullptr, attr);
    EXPECT_STREQ("speed", attr->getName());
    attr = client.getAttribute("index");
    ASSERT_NE(nullptr, attr);
    EXPECT_STREQ("index", attr->getName());
    attr = client.getAttribute("char_a");
    ASSERT_NE(nullptr, attr);
    EXPECT_STREQ("char_a", attr->getName());
}

/**
 * @brief Removes an attribute
 * 
 * Test the get before remove. After removal, test if the get returns null.
 */
TEST(Client, removeAttribute) {
    rmClient client;
    rmAttribute* attr1 = client.createAttribute("pos", RM_ATTRIBUTE_FLOAT);
    rmAttribute* attr2 = client.createAttribute("speed", RM_ATTRIBUTE_FLOAT, 0,
                                                90);
    rmAttribute* attr3 = client.createAttribute("name", RM_ATTRIBUTE_STRING);
    rmAttribute* attr4 = client.createAttribute("index", RM_ATTRIBUTE_INT);
    rmAttribute* attr5 = client.createAttribute("char_a", RM_ATTRIBUTE_CHAR);
    rmAttribute* attr6 = client.createAttribute("option1", RM_ATTRIBUTE_BOOL);
    
    rmAttribute* attr;
    attr = client.getAttribute("pos");
    ASSERT_NE(nullptr, attr);
    EXPECT_STREQ("pos", attr->getName());
    client.removeAttribute("pos");
    attr = client.getAttribute("pos");
    EXPECT_EQ(nullptr, attr);
    attr = client.getAttribute("option1");
    ASSERT_NE(nullptr, attr);
    EXPECT_STREQ("option1", attr->getName());
    client.removeAttribute("option");
    attr = client.getAttribute("option");
    EXPECT_EQ(nullptr, attr);
}

/**
 * @brief Gets the requested call
 * 
 * A list of calls is created first. The function is validated by checking if
 * the name of the returned call has the same name as requested.
 */
TEST(Client, getCall) {
    rmClient client;
    auto func = [](int argc, char** argv) {};
    rmCall* call1 = client.createCall("command1", func);
    rmCall* call2 = client.createCall("command7", func);
    rmCall* call3 = client.createCall("alert", func);
    rmCall* call4 = client.createCall("drive", func);
    rmCall* call5 = client.createCall("shutdown", func);
    
    rmCall* call;
    call = client.getCall("override");
    EXPECT_EQ(nullptr, call);
    call = client.getCall("alert");
    ASSERT_NE(nullptr, call);
    EXPECT_STREQ("alert", call->getName());
    call = client.getCall("drive");
    ASSERT_NE(nullptr, call);
    EXPECT_STREQ("drive", call->getName());
}

/**
 * @brief Removes a call
 * 
 * Test the get before remove. After removal, test if the get returns null.
 */
TEST(Client, removeCall) {
    rmClient client;
    auto func = [](int argc, char** argv) {};
    rmCall* call1 = client.createCall("command1", func);
    rmCall* call2 = client.createCall("command7", func);
    rmCall* call3 = client.createCall("alert", func);
    rmCall* call4 = client.createCall("drive", func);
    rmCall* call5 = client.createCall("shutdown", func);
    
    rmCall* call;
    call = client.getCall("drive");
    EXPECT_EQ(nullptr, call);
    EXPECT_STREQ("drive", call->getName());
    client.removeCall("drive");
    call = client.getCall("drive");
    EXPECT_EQ(nullptr, call);
}
