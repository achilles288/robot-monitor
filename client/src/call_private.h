/**
 * @file call_private.h
 * @brief Calls that invoke on the station's request
 * 
 * The callback functions take the parameters only as an integer that
 * represents number of extra tokens and the array of strings which are those
 * tokens. This is similar to "int main(int argc, char* argv[])". The tokens
 * strings are to be converted into appropriate data types within the callback.
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#ifdef __cplusplus
extern "C" {
#endif


typedef struct _rmCall {
    char name[12];
    void (*callback)(int, char**);
} rmCall;


rmCall* _rmCallGet(const char* key);


#ifdef __cplusplus
}
#endif
