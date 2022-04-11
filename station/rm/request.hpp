/**
 * @file request.hpp
 * @brief Sending a message while expecting a response from the client
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_REQUEST_H__
#define __RM_REQUEST_H__ ///< Header guard

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


class rmClient;


/**
 * @brief Waits for the response and triggers the callback function
 */
class RM_API rmRequest {
  private:
    char message[128] = {0};
    void (*callback)(const char*) = nullptr;
    long timeout = 1000;
    
  public:
    /**
     * @brief Default constructor
     */
    rmRequest() = default;
    
    /**
     * @brief Constructs a request instance
     * 
     * @param msg The request message
     * @param func The callback function
     * @param t Timeout in milliseconds
     */
    rmRequest(const char* msg, void (*func)(const char*), long t);
    
    /**
     * @brief Gets the request message
     * 
     * @return The string of the request
     */
    const char* getMessage() const;
    
    /**
     * @brief Gets the timeout value
     * 
     * @return Timeout in milliseconds
     */
    long getTimeout() const;
    
    /**
     * @brief Triggers when a response message is recieved from the client
     */
    virtual void onResponse(const char* msg);
};


/**
 * @breif The built in request objects with a privilege to access the client
 */
class RM_API rmBuiltinRequest: public rmRequest {
  private:
    rmClient* client = nullptr;
    void (*callback2)(const char*, rmClient*) = nullptr;
    
  public:
    /**
     * @brief Constructs a request instance
     * 
     * @param msg The request message
     * @param func The callback function
     * @param t Timeout in milliseconds
     * @param cli The client instance which the callback has access to
     */
    rmBuiltinRequest(const char* msg, void (*func)(const char*), long t,
                     rmClient* cli);
    
    /**
     * @brief Triggers when a response message is recieved from the client
     */
    void onResponse(const char* msg) override;
};

#endif
