// Copyright 2021 Joric (@joric)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

/* USB Device descriptor parameter */
//#define VENDOR_ID       0xFE1D
//#define PRODUCT_ID      0x6060
//#define DEVICE_VER      0x0100
//#define MANUFACTURER    Touchsensor
//#define PRODUCT         Ergoknife

#define I2C1_CLOCK_SPEED 50000

/* key matrix size */
/* Rows are doubled up */
#define MATRIX_ROWS 10
#define MATRIX_COLS 7

//#define MATRIX_ROW_PINS { D4, C6, D7, E6 }
//#define MATRIX_COL_PINS { F4, F5, F6, F7, B1, B3 }
#define MATRIX_ROW_PINS { C6, D7, E6, B4, B5 }
#define MATRIX_COL_PINS { F4, F5, F6, F7, B1, B3, B2 }
//#define UNUSED_PINS

/* COL2ROW, ROW2COL*/
#define DIODE_DIRECTION COL2ROW

/* define if matrix has ghost (lacks anti-ghosting diodes) */
//#define MATRIX_HAS_GHOST

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/*
 * Split Keyboard specific options, make sure you have 'SPLIT_KEYBOARD = yes' in your rules.mk, and define SOFT_SERIAL_PIN.
 */
//#define USE_I2C

#define SOFT_SERIAL_PIN D2
#define SELECT_SOFT_SERIAL_SPEED 1
/*Sets the protocol speed when using serial communication*/
//Speeds:
//0: about 189kbps (Experimental only)
//1: about 137kbps (default)
//2: about 75kbps
//3: about 39kbps
//4: about 26kbps
//5: about 20kbps

#define RGB_DI_PIN D3
#define RGBLED_SPLIT { 35, 35 }
#define RGBLED_NUM 70
#define RGBLIGHT_LIMIT_VAL 150

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE
