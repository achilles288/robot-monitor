/**
 * @file icon.h
 * @brief Robot Monitor icon stored as program memory
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


/**
 * @brief Robot Monitor icon stored as program memory
 */
extern unsigned char rmIconRGB[64 * 64 * 3];

/**
 * @brief Robot Monitor icon stored as program memory
 */
extern unsigned char rmIconAlpha[64 * 64];


#ifdef __cplusplus
#include <wx/bitmap.h>

static wxBitmap rmGetIcon() {
    return wxBitmap(wxImage(64, 64, rmIconRGB, rmIconAlpha, true));
}

#endif
