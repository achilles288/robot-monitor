/**
 * @file serial_list.cpp
 * @brief Serial ports to communicate with client devices
 * 
 * Function that lists the available serial ports.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT
#define RM_NO_WX


#include "rm/serial.hpp"

#include <utility>


/**
 * @brief Destructor
 */
rmSerialPortList::~rmSerialPortList() {
    if(ports != nullptr)
        delete ports;
}

/**
 * @brief Copy constructor
 * 
 * @param list Source
 */
rmSerialPortList::rmSerialPortList(const rmSerialPortList& list) {
    portCount = list.portCount;
    if(list.ports != nullptr) {
        ports = new rmSerialPortInfo[portCount];
        for(size_t i=0; i<portCount; i++)
            ports[i] = list.ports[i];
    }
}

/**
 * @brief Move constructor
 * 
 * @param list Source
 */
rmSerialPortList::rmSerialPortList(rmSerialPortList&& list) noexcept {
    ports = std::exchange(list.ports, nullptr);
    portCount = std::exchange(list.portCount, 0);
}

/**
 * @brief Copy assignment
 * 
 * @param list Source
 */
rmSerialPortList& rmSerialPortList::operator = (const rmSerialPortList& list) {
    rmSerialPortList tmp = rmSerialPortList(list);
    std::swap(ports, tmp.ports);
    std::swap(portCount, tmp.portCount);
    return *this;
}

/**
 * @brief Move assignment
 * 
 * @param list Source
 */
rmSerialPortList& rmSerialPortList::operator = (rmSerialPortList&& list) {
    rmSerialPortList tmp = rmSerialPortList(list);
    std::swap(ports, tmp.ports);
    std::swap(portCount, tmp.portCount);
    return *this;
}





/**
 * @brief Constructs from pointer
 * 
 * @param ptr Pointer value
 */
rmSerialPortList::iterator::iterator(rmSerialPortInfo* ptr) {
    pData = ptr;
}

/**
 * @brief Pointer dereferencing
 * 
 * @return The reference
 */
rmSerialPortInfo& rmSerialPortList::iterator::operator * () {
    return *pData;
}

/**
 * @brief Pointer dereferencing
 * 
 * @return The pointer
 */
rmSerialPortInfo* rmSerialPortList::iterator::operator -> () {
    return pData;
}

/**
 * @brief The prefix increment operator
 * 
 * @return Iterator to the next element
 */
rmSerialPortList::iterator& rmSerialPortList::iterator::operator ++ () {
    pData = &pData[1];
    return *this;
}

/**
 * @brief The postfix increment operator
 * 
 * @return Iterator to the next element
 */
rmSerialPortList::iterator rmSerialPortList::iterator::operator ++ (int) {
    iterator tmp = iterator(pData);
    pData = &pData[1];
    return tmp;
}

/**
 * @brief Compares the values
 * 
 * @param it Iterator 2
 * 
 * @return True if the pointers of the two are equal
 */
bool rmSerialPortList::iterator::operator == (const iterator& it) {
    return pData == it.pData;
}

/**
 * @brief Compares the values
 * 
 * @param it Iterator 2
 * 
 * @return True if the pointers of the two are not equal
 */
bool rmSerialPortList::iterator::operator != (const iterator& it) {
    return pData != it.pData;
}




/**
 * @brief Gets the beginning iterator
 * 
 * @return The iterator pointing to the first element of the list
 */
rmSerialPortList::iterator rmSerialPortList::begin() {
    return iterator(ports);
}

/**
 * @brief Gets the ending iterator
 * 
 * @return The iterator with the value that indicates the end of the list
 */
rmSerialPortList::iterator rmSerialPortList::end() {
    return iterator(&ports[portCount]);
}

/**
 * @brief Return the number of elements
 * 
 * @return This is the number of actual objects held in the vector, which
 *         is not necessarily equal to its storage capacity.
 */
size_t rmSerialPortList::size() const { return portCount; }

/**
 * @brief Returns a reference to the element at position n in the vector
 *        container
 * 
 * @param i Array index
 * 
 * @return The element at the specified position in the vector
 */
rmSerialPortInfo const& rmSerialPortList::operator [] (size_t i) const {
    return ports[i];
}

/**
 * @brief Adds a new element at the end of the vector, after its current
 *        last element
 * 
 * @param val Value to be copied or moved to the new element
 */
void rmSerialPortList::push_back(const rmSerialPortInfo& val) {
    rmSerialPortInfo* temp = new rmSerialPortInfo[portCount + 1];
    for(size_t i=0; i<portCount; i++)
        temp[i] = ports[i];
    temp[portCount++] = val;
    if(ports != nullptr)
        delete ports;
    ports = temp;
}
