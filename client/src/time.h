/**
 * @file time.h
 * @brief Platform dependant time functions for the library
 * 
 * @copyright Copyright (c) 2022 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#ifdef __cplusplus
extern "C" {
#endif


#if defined(__arm__)
uint32_t HAL_GetTick();

#define _rmGetTime() HAL_GetTick()

#elif defined(Arduino_h)
long millis();

#define _rmGetTime() millis()

#else
#define _rmGetTime() 0

#endif


#ifdef __cplusplus
}
#endif
