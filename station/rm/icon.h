/**
 * @file icon.h
 * @brief Robot Monitor icon stored as program memory
 * 
 * @copyright Copyright (c) 2021 Khant Kyaw Khaung
 * 
 * @license{This project is released under the MIT License.}
 */


#pragma once
#ifndef __RM_ICON_H__
#define __RM_ICON_H__ ///< Header guard

#ifndef RM_WX_API
#ifdef _WIN32
#ifdef RM_WX_EXPORT
#define RM_WX_API __declspec(dllexport) ///< API
#else
#define RM_WX_API __declspec(dllimport) ///< API
#endif
#else
#define RM_WX_API ///< API
#endif
#endif


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Robot Monitor icon stored as program memory
 */
RM_WX_API extern unsigned char rmIconRGB[64 * 64 * 3];

/**
 * @brief Robot Monitor icon stored as program memory
 */
RM_WX_API extern unsigned char rmIconAlpha[64 * 64];


#ifdef __cplusplus
}

#include <wx/bitmap.h>

static wxBitmap rmGetIcon() {
    return wxBitmap(wxImage(64, 64, rmIconRGB, rmIconAlpha, true));
}

#endif

#endif
