/**
 * @file attribute.hpp
 * @brief Attributes shown by the client or manipulated by the station
 * 
 * A key value pair structure that can be used in a map structure. This type
 * of data is designed to be portable in communication between two devices.
 * The data here is the real-time data of the client device which can also be
 * overriden by the station. The value may be a string, a number or a blob.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_ATTRIBUTE_H__
#define __RM_ATTRIBUTE_H__ ///< Header guard

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


#include "client.hpp"

#include <cstdint>


#define RM_ATTRIBUTE_BOOL   0 ///< Boolean data type
#define RM_ATTRIBUTE_CHAR   1 ///< Character data type
#define RM_ATTRIBUTE_INT    2 ///< Integer data type
#define RM_ATTRIBUTE_FLOAT  3 ///< Floating point data type
#define RM_ATTRIBUTE_STRING 4 ///< String data type
#define RM_ATTRIBUTE_BLOB   5 ///< Binary large object


/**
 * @brief Attributes shown by the client or manipulated by the station
 * 
 * A key value pair structure that can be used in a map structure. This type
 * of data is designed to be portable in communication between two devices.
 * The data here is the real-time data of the client device which can also be
 * overriden by the station. The value may be a string, a number or a blob.
 */
class RM_API rmAttribute {
  private:
    rmClient* client = nullptr;
    char name[12] = {0};
    void* data = nullptr;
    int8_t type = RM_ATTRIBUTE_STRING;
    void* boundary = nullptr;
    
  public:
    /**
     * @brief Default constructor
     */
    rmAttribute() = default;
    
    /**
     * @brief Constructs an attribute with a name, type and boundaries
     * 
     * The constructor declares the new attribute on the specified client.
     * 
     * @param key Unique name of the attribute with maximum 11 characters
     * @param t Data type of the value stored
     * @param lower Lower bound value. The type of the lower and upper should
     *              be of the same type as t.
     * @param upper Upper bound value
     */
    rmAttribute(const char* key, int8_t t, void* lower=nullptr,
                void* upper=nullptr);
    
    /**
     * @brief Destructor
     */
    ~rmAttribute();
    
    /**
     * @brief Copy constructor (deleted)
     * 
     * @param attr Source
     */
    rmAttribute(const rmAttribute& attr) = delete;
    
    /**
     * @brief Move constructor
     * 
     * @param attr Source
     */
    rmAttribute(rmAttribute&& attr) noexcept = default;
    
    /**
     * @brief Copy assignment (deleted)
     * 
     * @param attr Source
     */
    rmAttribute& operator=(const rmAttribute& attr) = delete;
    
    /**
     * @brief Move assignment
     * 
     * @param attr Source
     */
    rmAttribute& operator=(rmAttribute&& attr) noexcept = default;
    
    /**
     * @brief Gets the attribute name
     * 
     * @return The name of the attribute
     */
    const char* getName() const;
    
    /**
     * @brief Sets the value
     * 
     * Calling this function triggers the set command to the client device.
     * 
     * @param value The boolean value
     */
    void setValue(bool value);
    
    /**
     * @brief Sets the value
     * 
     * Calling this function triggers the set command to the client device.
     * 
     * @param value The character value
     */
    void setValue(char value);
    
    /**
     * @brief Sets the value
     * 
     * Calling this function triggers the set command to the client device.
     * 
     * @param value The integer value
     */
    void setValue(int32_t value);
    
    /**
     * @brief Sets the value
     * 
     * Calling this function triggers the set command to the client device.
     * 
     * @param value The floating point value
     */
    void setValue(float value);
    
    /**
     * @brief Sets the value
     * 
     * Calling this function triggers the set command to the client device.
     * 
     * @param value The string value or blob. The array is copied.
     */
    void setValue(const char* value);
    
    /**
     * @brief Gets the value
     * 
     * @return The value in a pointer type which is to be casted to the
     *         appropriate type
     */
    void* getValue() const;
    
    /**
     * @brief Gets the data type of the value
     * 
     * @return The data type number
     */
    int8_t getType() const;
    
    /**
     * @brief Sets the boundary
     * 
     * @param lower Lower bound value
     * @param upper Upper bound value
     */
    void setBoundary(void* lower, void* upper);
    
    /**
     * @brief Gets the lower bound value
     * 
     * @return Lower bound value which is an integer or a float. If there
     *         is no boundary or the request is invalid, returns a null.
     */
    void* getLowerBound() const;
    
    /**
     * @brief Gets the upper bound value
     * 
     * @return Upper bound value which is an integer or a float. If there
     *         is no boundary or the request is invalid, returns a null.
     */
    void* getUpperBound() const;
    
    /**
     * @brief Sets the client who owns this attribute
     * 
     * @param cli The client
     */
    void setClient(rmClient* cli);
    
    /**
     * @brief Gets the client who owns this attribute
     * 
     * @return The client
     */
    rmClient* getClient() const;
};

#endif
