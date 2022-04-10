/**
 * @file echo.cpp
 * @brief To print regular messages for the client object
 * 
 * Intended to be used by the 'echo' command of the client device. This command
 * is usually to print normal output messages other than attribute changes and
 * functional callbacks.
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#define RM_EXPORT


#include "rm/echo.hpp"

#include "rm/client.hpp"


/**
 * @brief Echos the messages
 * 
 * Intended to be called from 'echo' command by the client device. This command
 * is usually to print normal output messages other than attribute changes and
 * functional callbacks.
 * 
 * @param msg The message
 * @param status The status code. Status code other than 0 may print red
 *               messages.
 */
void rmEcho::echo(const char* msg, int status) {}

/**
 * @brief Enables or disables the user input
 * 
 * @param en True for enable and false for otherwise
 */
void rmEcho::setEnabled(bool en) {}




static void echo(rmClient *client, int status, int argc, char *argv[]) {
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
    client->echo(str, status);
}


void rmCallbackEcho(int argc, char *argv[], rmClient* cli) {
    echo(client, 0, argc, argv);
}


void rmCallbackWarn(int argc, char *argv[], rmClient* cli) {
    echo(client, 2, argc, argv);
}


void rmCallbackErr(int argc, char *argv[], rmClient* cli) {
    echo(client, 1, argc, argv);
}
