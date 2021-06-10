#define RM_NO_WX

#include <rm/serial.hpp>

#include <gtest/gtest.h>


/**
 * @brief Appends the list and checks the element count
 */
TEST(SerialPortList, size) {
    rmSerialPortList ports;
    rmSerialPortInfo port;
    ASSERT_EQ(ports.size(), 0);
    strcpy(port.port, "/dev/ttyACM0");
    ports.push_back(port);
    ASSERT_EQ(ports.size(), 1);
    strcpy(port.port, "/dev/ttyACM1");
    ports.push_back(port);
    strcpy(port.port, "/dev/ttyS0");
    ports.push_back(port);
    ASSERT_EQ(ports.size(), 3);
    strcpy(port.port, "COM4");
    ports.push_back(port);
    strcpy(port.port, "/dev/ttyUSB0");
    ports.push_back(port);
    ASSERT_EQ(ports.size(), 5);
}

/**
 * @brief The array operator
 */
TEST(SerialPortList, get) {
    rmSerialPortList ports;
    rmSerialPortInfo port;
    strcpy(port.port, "/dev/ttyACM0");
    ports.push_back(port);
    strcpy(port.port, "/dev/ttyACM1");
    ports.push_back(port);
    strcpy(port.port, "/dev/ttyS0");
    ports.push_back(port);
    strcpy(port.port, "COM4");
    ports.push_back(port);
    strcpy(port.port, "/dev/ttyUSB0");
    ports.push_back(port);
    
    ASSERT_EQ(ports.size(), 5);
    port = ports[1];
    ASSERT_STREQ(port.port, "/dev/ttyACM1");
    port = ports[3];
    ASSERT_STREQ(port.port, "COM4");
}

/**
 * @brief The iterator
 */
TEST(SerialPortList, iterator) {
    rmSerialPortList ports;
    rmSerialPortInfo port;
    strcpy(port.port, "/dev/ttyACM0");
    ports.push_back(port);
    strcpy(port.port, "/dev/ttyACM1");
    ports.push_back(port);
    strcpy(port.port, "/dev/ttyS0");
    ports.push_back(port);
    strcpy(port.port, "COM4");
    ports.push_back(port);
    strcpy(port.port, "/dev/ttyUSB0");
    ports.push_back(port);
    
    ASSERT_EQ(ports.size(), 5);
    auto it = ports.begin();
    port = *it;
    EXPECT_STREQ(port.port, "/dev/ttyACM0");
    EXPECT_STREQ(it->port, "/dev/ttyACM0");
    it++;
    EXPECT_STREQ(it->port, "/dev/ttyACM1");
    EXPECT_STREQ((++it)->port, "/dev/ttyS0");
    it++;
    it++;
    it++;
    EXPECT_TRUE(ports.end() == it);
}
