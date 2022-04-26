// Copyright 2021 Joric (@joric)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#define OLED_DISPLAY_128X32

#ifdef RGBLIGHT_ENABLE
//#define RGBLIGHT_EFFECT_BREATHING
//#define RGBLIGHT_EFFECT_RAINBOW_MOOD
//#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
//#define RGBLIGHT_EFFECT_SNAKE
//#define RGBLIGHT_EFFECT_KNIGHT
//#define RGBLIGHT_EFFECT_CHRISTMAS
//#define RGBLIGHT_EFFECT_STATIC_GRADIENT
//#define RGBLIGHT_EFFECT_RGB_TEST
//#define RGBLIGHT_EFFECT_ALTERNATING
//#define RGBLIGHT_HUE_STEP 8
//#define RGBLIGHT_SAT_STEP 8
//#define RGBLIGHT_VAL_STEP 8
#endif

// If you are using an Elite C rev3 on the slave side, uncomment the lines below:
// #define SPLIT_USB_DETECT
// #define NO_USB_STARTUP_CHECK

#define USE_SERIAL

/* Select hand configuration */

#define MASTER_LEFT
//#define MASTER_RIGHT
//#define EE_HANDS
#define SPLIT_USB_DETECT

#define DYNAMIC_KEYMAP_LAYER_COUNT 4

//#define SERIAL_USE_MULTI_TRANSACTION
#define SPLIT_TRANSACTION_IDS_USER USER_SYNCXY

#define VIAL_KEYBOARD_UID {0x89, 0x10, 0x0B, 0x23, 0x91, 0xA6, 0x88, 0x5E}

#define VIAL_UNLOCK_COMBO_ROWS { 0, 2 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 3 }
//#define VIAL_COMBO_ENTRIES 1