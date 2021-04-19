/**
 * @file builtin_call.cpp
 * @brief The built in call objects for every client
 * 
 * This includes the 'set' and 'respond' commands.
 * 
 * @license{This project is released under the MIT License.}
 */


#include "rm/builtin_call.hpp"


/**
 * @brief Constructs with a client
 * 
 * @param cli The client
 */
rmBuiltinCallSet::rmBuiltinCallSet(rmClient* cli)
                 :rmCall("set", nullptr)
{
    client = cli;
}

/**
 * @brief Invokes the callback of the object
 * 
 * @param argc Argument count
 * @param argv Tokens
 */
void rmBuiltinCallSet::invoke(int argc, char* argv[]) {
    if(argc != 2)
        return;
    
    rmAttribute* attr = client->getAttribute(argv[0]);
    if(attr != nullptr) {
        rmAttributeData prev = attr->getValue();
        attr->setValue(argv[1]);
        if(attr->getValue().i != prev.i) {
            rmAttributeNotifier* noti = attr->getNotifier();
            if(noti != nullptr)
                noti->triggerCallback();
        }
    }
}




/**
 * @brief Constructs with a client
 * 
 * @param cli The client
 */
rmBuiltinCallEcho::rmBuiltinCallEcho(rmClient* cli)
                  :rmCall("echo", nullptr)
{
    client = cli;
}

/**
 * @brief Invokes the callback of the object
 * 
 * @param argc Argument count
 * @param argv Tokens
 */
void rmBuiltinCallEcho::invoke(int argc, char* argv[]) {
    char str[128];
    int i=0, j=0, k=0;
    while(i < argc) {
        char c;
        while((c = argv[i][j++]) != '\0')
            str[k++] = c;
        if(i < argc - 1)
            str[k++] = ' ';
        i++;
        j = 0;
        if(k == 127)
            break;
    }
    str[k] = '\0';
    str[127] = '\0';
    client->echo(str, 0);
}




/**
 * @brief Constructs with a client
 * 
 * @param cli The client
 */
rmBuiltinCallError::rmBuiltinCallError(rmClient* cli)
                  :rmCall("echo", nullptr)
{
    client = cli;
}

/**
 * @brief Invokes the callback of the object
 * 
 * @param argc Argument count
 * @param argv Tokens
 */
void rmBuiltinCallError::invoke(int argc, char* argv[]) {
    char str[128];
    int i=0, j=0, k=0;
    while(i < argc) {
        char c;
        while((c = argv[i][j++]) != '\0')
            str[k++] = c;
        if(i < argc - 1)
            str[k++] = ' ';
        i++;
        j = 0;
        if(k == 127)
            break;
    }
    str[k] = '\0';
    str[127] = '\0';
    client->echo(str, 2);
}
