#include "robotmonitor.h"

#include <gtest/gtest.h>


/**
 * @brief Gets the requested attribute
 * 
 * A list of attributes is created first. The function is validated by checking
 * if the name of the returned attribute has the same name as requested.
 */
TEST(Data, getAttribute) {
    rmClearData();
    rmCreateAttribute("pos", RM_ATTRIBUTE_FLOAT);
    rmCreateAttribute("speed", RM_ATTRIBUTE_FLOAT);
    rmCreateAttribute("name", RM_ATTRIBUTE_STRING);
    rmCreateAttribute("index", RM_ATTRIBUTE_INT);
    rmCreateAttribute("char_a", RM_ATTRIBUTE_CHAR);
    rmCreateAttribute("option1", RM_ATTRIBUTE_BOOL);
    
    rmAttribute* attr;
    attr = rmGetAttribute("voltage");
    EXPECT_EQ(nullptr, attr);
    attr = rmGetAttribute("speed");
    ASSERT_NE(nullptr, attr);
    EXPECT_STREQ("speed", attr->name);
    attr = rmGetAttribute("index");
    ASSERT_NE(nullptr, attr);
    EXPECT_STREQ("index", attr->name);
    attr = rmGetAttribute("char_a");
    ASSERT_NE(nullptr, attr);
    EXPECT_STREQ("char_a", attr->name);
}

/**
 * @brief Gets the requested call
 * 
 * A list of calls is created first. The function is validated by checking if
 * the name of the returned call has the same name as requested.
 */
TEST(Data, getCall) {
    rmClearData();
    auto func = [](int argc, char** argv) {};
    rmCreateCall("command1", func);
    rmCreateCall("command7", func);
    rmCreateCall("alert", func);
    rmCreateCall("drive", func);
    rmCreateCall("shutdown", func);
    
    rmCall* call;
    call = rmGetCall("override");
    EXPECT_EQ(nullptr, call);
    call = rmGetCall("alert");
    ASSERT_NE(nullptr, call);
    EXPECT_STREQ("alert", call->name);
    call = rmGetCall("drive");
    ASSERT_NE(nullptr, call);
    EXPECT_STREQ("drive", call->name);
}
