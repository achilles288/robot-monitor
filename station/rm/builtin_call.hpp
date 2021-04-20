/**
 * @file builtin_call.hpp
 * @brief The built in call objects for every client
 * 
 * This includes the 'set' and 'echo' commands.
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_BUILTIN_CALL_H__
#define __RM_BUILTIN_CALL_H__ ///< Header guard


#include "client.hpp"


/**
 * @brief The built in call 'set'
 */
class rmBuiltinCallSet: public rmCall {
  private:
    rmClient* client = nullptr;
    
  public:
    /**
     * @brief Constructs with a client
     * 
     * @param cli The client
     */
    rmBuiltinCallSet(rmClient* cli);
    
    /**
     * @brief Invokes the callback of the object
     * 
     * @param argc Argument count
     * @param argv Tokens
     */
    void invoke(int argc, char *argv[]) override;
};


/**
 * @brief The built in call 'echo'
 */
class rmBuiltinCallEcho: public rmCall {
  private:
    rmClient* client = nullptr;
    
  public:
    /**
     * @brief Constructs with a client
     * 
     * @param cli The client
     */
    rmBuiltinCallEcho(rmClient* cli);
    
    /**
     * @brief Invokes the callback of the object
     * 
     * @param argc Argument count
     * @param argv Tokens
     */
    void invoke(int argc, char *argv[]) override;
};


/**
 * @brief The built in call 'warn'
 */
class rmBuiltinCallWarn: public rmCall {
  private:
    rmClient* client = nullptr;
    
  public:
    /**
     * @brief Constructs with a client
     * 
     * @param cli The client
     */
    rmBuiltinCallWarn(rmClient* cli);
    
    /**
     * @brief Invokes the callback of the object
     * 
     * @param argc Argument count
     * @param argv Tokens
     */
    void invoke(int argc, char *argv[]) override;
};


/**
 * @brief The built in call 'error'
 */
class rmBuiltinCallError: public rmCall {
  private:
    rmClient* client = nullptr;
    
  public:
    /**
     * @brief Constructs with a client
     * 
     * @param cli The client
     */
    rmBuiltinCallError(rmClient* cli);
    
    /**
     * @brief Invokes the callback of the object
     * 
     * @param argc Argument count
     * @param argv Tokens
     */
    void invoke(int argc, char *argv[]) override;
};

#endif
