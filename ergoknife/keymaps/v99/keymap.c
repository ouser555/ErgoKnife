// Copyright 2021 Joric (@joric)
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H
#include "keyboards/jorne/keymaps/v99/i2cc.h"
#include "keyboards/jorne/keymaps/v99/v99i2chw.h"

#include "pointing_device.h"
#include "transactions.h"
#include "transport.h"
#include "transaction_id_define.h"

void process_mouse(report_mouse_t* mouse_report);
void process_mouse_user(report_mouse_t* mouse_report, int16_t x, int16_t y);
void process_wheel(report_mouse_t* mouse_report);
void process_wheel_user(report_mouse_t* mouse_report, int16_t h, int16_t v);

#define V99_DPI_OPTIONS { 1, 2, 3, 4, 5, 6 }
#define V99_DPI_MASTER_DEFAULT 1
#define V99_DPI_SLAVE_DEFAULT 1

#ifndef V99_DRAGSCROLL_DPI
#define V99_DRAGSCROLL_DPI 1 // Fixed-DPI Drag Scroll
#endif
uint16_t dpi_array[] = V99_DPI_OPTIONS;
#define DPI_OPTION_SIZE (sizeof(dpi_array) / sizeof(uint16_t))

static int8_t V99_X_TRANSFORM_M = -1;
static int8_t V99_Y_TRANSFORM_M =  1;
static int8_t V99_X_TRANSFORM_S = -1;
static int8_t V99_Y_TRANSFORM_S =  1;

typedef union {
  uint32_t raw;
  struct {
    uint8_t v99_dpi_master;
    uint8_t v99_dpi_slave;
  };
} user_config_t;

static user_config_t user_config;

static int8_t wheelh = 0;
static int8_t wheelv = 0;

#define V99_WHEEL_H_DIV 10
#define V99_WHEEL_V_DIV 20

static bool is_drag_scroll_m = 0;
static bool is_drag_scroll_s = 0;

#define V99MASTER 0x11
static int16_t s2m_x = 0;
static int16_t s2m_y = 0;

#if 1
//const static float V99_X_TRANSFORM = -1;//-2.5;
//const static float V99_Y_TRANSFORM = -1;//2.5;
//const static float V99_WHEEL_H_DIV = 10;
//const static float V99_WHEEL_V_DIV = 20;
//static bool is_drag_scroll = 0;
#endif

enum layers {
    _QWERTY = 0,
    _LOWER,
    _RAISE,
    _ADJUST,
};

enum custom_keycodes {
#ifdef VIA_ENABLE
  QWERTY = USER00,
#else
  QWERTY = SAFE_RANGE,
#endif
  LOWER,
  RAISE,
  ADJUST,
  DPI_CONFIG_M,
  DPI_CONFIG_S,
  DRAG_SCROLL_M,
  DRAG_SCROLL_S,
  RGBRST,
  V99_SAFE_RANGE,
};

#define EISU LALT(KC_GRV)

#define RBR_RGU MT(MOD_RGUI, KC_RBRC)
#define F12_RGU MT(MOD_RGUI, KC_F12)
#define PLS_LCT MT(MOD_LCTL, KC_PPLS)
#define EQL_LCT MT(MOD_LCTL, KC_PEQL)
#define APP_LCT MT(MOD_LCTL, KC_APP)
#define EQL_RCT MT(MOD_RCTL, KC_PEQL)
#define QUO_RCT MT(MOD_RCTL, KC_QUOT)
#define APP_RCT MT(MOD_RCTL, KC_APP)
#define MIN_RCT MT(MOD_RCTL, KC_MINS)
#define EQL_LAL MT(MOD_LALT, KC_EQL)
#define BSL_RAL MT(MOD_RALT, KC_BSLS)
#define BSP_LSH MT(MOD_LSFT, KC_BSPC)
#define SPC_RSH MT(MOD_RSFT, KC_SPC)
#define DEL_RSE LT(_RAISE, KC_DEL)
#define TAB_RSE LT(_RAISE, KC_TAB)
#define ENT_LWR LT(_LOWER, KC_ENT)
#define ESC_LWR LT(_LOWER, KC_ESC)


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* Qwerty
   * ,----------------------------------------------------------------------------------------------------------------------.
   * | ESC  |   1  |   2  |   3  |   4  |   5  |   [  |                    |   ]  |   6  |   7  |   8  |   9  |   0  |Pscree|
   * |------+------+------+------+------+------+------+--------------------+------+------+------+------+------+------+------|
   * |  `   |   Q  |   W  |   E  |   R  |   T  |   -  |                    |   =  |   Y  |   U  |   I  |   O  |   P  |  \   |
   * |------+------+------+------+------+------+------+--------------------+------+------+------+------+------+------+------|
   * | Tab  |   A  |   S  |   D  |   F  |   G  |  Del |                    | Bksp |   H  |   J  |   K  |   L  |   ;  |  "   |
   * |------+------+------+------+------+------+---------------------------+------+------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   B  | Space|                    | Enter|   N  |   M  |   ,  |   .  |   /  | Shift|
   * |-------------+------+------+------+------+------+------+------+------+------+------+------+------+------+-------------|
   * | Ctrl |  GUI |  ALt | EISU |||||||| Lower| Space|  Del |||||||| Bksp | Enter| Raise|||||||| Left | Down |  Up  | Right|
   * ,----------------------------------------------------------------------------------------------------------------------.
   */
  [_QWERTY] = LAYOUT( \
    KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_LBRC,                        KC_RBRC, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_PSCR, \
    KC_GRV,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_MINS,                        KC_EQL , KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS, \
    KC_TAB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_DEL ,                        KC_BSPC, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, \
    KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_SPC ,                        KC_ENT , KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, \
    KC_LCTL, KC_LGUI, KC_LALT, EISU,             LOWER,   KC_SPC ,KC_DEL,         KC_BSPC,KC_ENT , RAISE,            KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT  \
  ),

  /* Lower
   * ,----------------------------------------------------------------------------------------------------------------------.
   * |  F11 |  F1  |  F2  |  F3  |  F4  |  F5  |   {  |                    |   }  |  F6  |  F7  |  F8  |  F9  |  F10 |  F12 |
   * |------+------+------+------+------+------+------+--------------------+------+------+------+------+------+------+------|
   * |  ~   |   !  |   @  |   #  |   $  |   %  |   _  |                    |   +  |   ^  |   &  |   *  |   (  |   )  |  |   |
   * |------+------+------+------+------+------+------+--------------------+------+------+------+------+------+------+------|
   * | Tab  |   1  |   2  |   3  |   4  |   5  |  Del |                    | Bksp |   H  |   J  |   K  |   L  |   :  |  "   |
   * |------+------+------+------+------+------+---------------------------+------+------+------+------+------+------+------|
   * | Shift|   6  |   7  |   8  |   9  |   0  | Space|                    | Enter|   N  |   M  |   <  |   >  |   ?  | Shift|
   * |-------------+------+------+------+------+------+------+------+------+------+------+------+------+------+-------------|
   * | Ctrl |  GUI |  ALt | EISU |||||||| Lower| Space|  Del |||||||| Bksp | Enter| Raise|||||||| Home |PageDn|PageUp|  End |
   * ,----------------------------------------------------------------------------------------------------------------------.
   */
  [_LOWER] = LAYOUT(
    KC_F11,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_LCBR,                        KC_RCBR, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F12,  \
    KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_UNDS,                        KC_PLUS, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE, \
    KC_TAB,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_DEL ,                        KC_BSPC, KC_H,    KC_J,    KC_K,    KC_L,    KC_COLN, KC_DQT , \
    KC_LSFT, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_SPC ,                        KC_ENT , KC_N,    KC_M,    KC_LT,   KC_GT,   KC_QUES, KC_RSFT, \
    KC_LCTL, KC_LGUI, KC_LALT, EISU,             LOWER,   KC_SPC ,KC_DEL,         KC_BSPC,KC_ENT , RAISE,            KC_HOME, KC_PGDN, KC_PGUP, KC_END   \
  ),

  /* Raise
   * ,----------------------------------------------------------------------------------------------------------------------.
   * |  F11 |  F1  |  F2  |  F3  |  F4  |  F5  |   {  |                    |   }  |  F6  |  F7  |  F8  |  F9  |  F10 |  F12 |
   * |------+------+------+------+------+------+------+--------------------+------+------+------+------+------+------+------|
   * |  ~   |   !  |   @  |   #  |   $  |   %  |   _  |                    |   +  |   ^  |   &  |   *  |   (  |   )  |  |   |
   * |------+------+------+------+------+------+------+--------------------+------+------+------+------+------+------+------|
   * | Tab  |   1  |   2  |   3  |   4  |   5  |  Del |                    | Bksp |   H  |   J  |   K  |   L  |   :  |  "   |
   * |------+------+------+------+------+------+---------------------------+------+------+------+------+------+------+------|
   * | Shift|   6  |   7  |   8  |   9  |   0  | Space|                    | Enter|   N  |   M  |   <  |   >  |   ?  | Shift|
   * |-------------+------+------+------+------+------+------+------+------+------+------+------+------+------+-------------|
   * | Ctrl |  GUI |  ALt | EISU |||||||| Lower| Space|  Del |||||||| Bksp | Enter| Raise|||||||| Home |PageDn|PageUp|  End |
   * ,----------------------------------------------------------------------------------------------------------------------.
   */
  [_RAISE] = LAYOUT(
    KC_F11,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_LCBR,                        KC_RCBR, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F12,  \
    KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_UNDS,                        KC_PLUS, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE, \
    KC_TAB,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_DEL ,                        KC_BSPC, KC_H,    KC_J,    KC_K,    KC_L,    KC_COLN, KC_DQT , \
    KC_LSFT, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_SPC ,                        KC_ENT , KC_N,    KC_M,    KC_LT,   KC_GT,   KC_QUES, KC_RSFT, \
    KC_LCTL, KC_LGUI, KC_LALT, EISU,             LOWER,   KC_SPC ,KC_DEL,         KC_BSPC,KC_ENT , RAISE,            KC_HOME, KC_PGDN, DPI_CONFIG_S, DPI_CONFIG_M   \
  ),

  /* Adjust
   * ,----------------------------------------------------------------------------------------------------------------------.
   * |      |      |      |      |      |      |      |                    |      |      |      |      |      |      |      |
   * |------+------+------+------+------+------+---------------------------+------+------+------+------+------+------+------|
   * |      | Reset|RGB ON|  MODE|  HUE-|  HUE+|      |                    |      |  SAT-|  SAT+|  VAL-|  VAL+|      |      |
   * |------+------+------+------+------+------+---------------------------+------+------+------+------+------+------+------|
   * |      |      | BL ON|  BRTG|  INC|   DEC|      |                    |      |      |      |      |      |      |      |
   * |------+------+------+------+------+------+---------------------------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |                    |      |      |      |      |      |      |      |
   * |-------------+------+------+------+------+------+------+------+------+------+------+------+------+------+-------------|
   * |      |      |      |      ||||||||      |      |      ||||||||      |      |      ||||||||      |      |      |      |
   * ,----------------------------------------------------------------------------------------------------------------------.
   */
  [_ADJUST] = LAYOUT(
    _______, _______, _______, _______, _______, _______,_______,                       _______, _______, _______, _______, _______, _______, _______, \
    _______, RESET  , RGB_TOG, RGB_MOD, RGB_HUD, RGB_HUI,_______,                       _______, RGB_SAD, RGB_SAI, RGB_VAD, RGB_VAI, _______, _______, \
    _______, _______, BL_TOGG, BL_BRTG, BL_INC , BL_DEC ,_______,                       _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______, _______, _______,_______,                       _______, _______, _______, _______, _______, _______, _______, \
    _______, _______, _______, _______,          _______,_______,_______,       _______,_______, _______,          _______, _______, _______, _______  \
  )
};

#if 1
layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}
#else
void persistent_default_layer_set(uint16_t default_layer) {
  eeconfig_update_default_layer(default_layer);
  default_layer_set(default_layer);
}
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
         print("mode just switched to qwerty and this is a huge string\n");
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
        }
      #endif
      break;
    case DPI_CONFIG_M:
      if (record->event.pressed) {
          user_config.v99_dpi_master = (user_config.v99_dpi_master + 1) % DPI_OPTION_SIZE;
          eeconfig_update_kb(user_config.raw);
          //adns_set_cpi(dpi_array[user_config.v99_dpi_master]);
          V99_X_TRANSFORM_M = dpi_array[user_config.v99_dpi_master] * -1;
          V99_Y_TRANSFORM_M = dpi_array[user_config.v99_dpi_master] *  1;

          eeconfig_update_user(user_config.raw); // Write default value to EEPROM now
      }
      return false;
      //break;

    case DPI_CONFIG_S:
      if (record->event.pressed) {
          user_config.v99_dpi_slave = (user_config.v99_dpi_slave + 1) % DPI_OPTION_SIZE;
          eeconfig_update_kb(user_config.raw);
          //adns_set_cpi(dpi_array[user_config.v99_dpi_master]);
          V99_X_TRANSFORM_S = dpi_array[user_config.v99_dpi_slave] * -1;
          V99_Y_TRANSFORM_S = dpi_array[user_config.v99_dpi_slave] *  1;

          eeconfig_update_user(user_config.raw); // Write default value to EEPROM now
      }
      return false;
      //break;

	case DRAG_SCROLL_M:
#ifndef V99PAD_DRAGSCROLL_MOMENTARY
	  if (record->event.pressed)
#endif
	  {
		  is_drag_scroll_m ^= 1;
	  }
      wheelh = 0;
      wheelv = 0;
      return false;
	  //break;

    case DRAG_SCROLL_S:
#ifndef V99PAD_DRAGSCROLL_MOMENTARY
	  if (record->event.pressed)
#endif
	  {
		  is_drag_scroll_s ^= 1;
	  }
      wheelh = 0;
      wheelv = 0;
      return false;
	  //break;
  }
  return true;
}

#if 1

float xytrans(uint8_t data,int8_t ts){

  int8_t tmp = (int8_t) data;
  float odata = data * ts;

  if(abs(ts)>2){

    if(abs(tmp)<2){
      //odata = data * 2;
      if(ts > 0 ){
         odata = data *  3;
      }else{
         odata = data * -3;
      }
    }else if(abs(tmp)<5){
      if(ts > 0 ){
         odata = data *  4;
      }else{
         odata = data * -4;
      }
    }

  }
  return odata;
}

float htrans(uint8_t h){
  float i = 0;

  wheelh += h;
  if( wheelh != 0 ){
    i = wheelh / V99_WHEEL_H_DIV;
    wheelh = wheelh % V99_WHEEL_H_DIV;
    if(i != 0){
      wheelv = 0;
    }
  }
  return i;
}

float vtrans(uint8_t v){
  float i = 0;

  wheelv += v;
  if( wheelv != 0 ){
    i = wheelv / V99_WHEEL_V_DIV;
    wheelv = wheelv % V99_WHEEL_V_DIV;
    if(i != 0){
      wheelh = 0;
    }
  }
  return i;
}

__attribute__((weak)) void process_mouse_user(report_mouse_t* mouse_report, int16_t x, int16_t y) {
    //mouse_report->x = x/3;
    //mouse_report->y = y/3;
}

#if 0
__attribute__((weak)) void process_mouse(report_mouse_t* mouse_report) {

	  float xt = 0;
	  float yt = 0;

    //report_v99_t data = v99_slim_read_burst();
		report_v99_t data;
		//data.dx = v99_read(REG_PRODUCT_ID);
		data.dx = v99_read(REG_DELTA_X);
		data.dy = v99_read(REG_DELTA_Y);



    if (data.dx != 0 || data.dy != 0) {
        //if (debug_mouse)
        //    dprintf("Raw ] X: %d, Y: %d\n", data.dx, data.dy);

        // Apply delta-X and delta-Y transformations.
				if(abs(data.dx)>1 || abs(data.dy)>1){
				  xt = (float) data.dx * V99_X_TRANSFORM;
          yt = (float) data.dy * V99_Y_TRANSFORM;
			  }else{
					xt = (float) data.dx * -1;
          yt = (float) data.dy * -1;
				}

        int16_t xti = (int16_t)xt;
        int16_t yti = (int16_t)yt;

        process_mouse_user(mouse_report, xti, yti);
    }
}
#else
__attribute__((weak)) void process_mouse(report_mouse_t* mouse_report) {

	float xt = 0;
	float yt = 0;
    float ht = 0;
    float vt = 0;

    //report_v99_t data = v99_slim_read_burst();
	report_v99_t data;
	//data.dx = v99_read(REG_PRODUCT_ID);
	data.dx = v99_read(REG_DELTA_X);
	data.dy = v99_read(REG_DELTA_Y);
    data.dx = data.dx/3;
    data.dy = data.dy/3;

    if (is_keyboard_master()){  // keyboard master
      if(is_drag_scroll_m == is_drag_scroll_s){ // same mode

        if(!is_drag_scroll_m){ // xy mode
          if(data.dx == 0 && data.dy == 0){ // slave data

            xt = xytrans(s2m_x, V99_X_TRANSFORM_S);
            yt = xytrans(s2m_y, V99_Y_TRANSFORM_S);

          }else{  // master data

            xt = xytrans(data.dx, V99_X_TRANSFORM_M);
            yt = xytrans(data.dy, V99_Y_TRANSFORM_M);

          }
        }else{ // hv mode
          if(data.dx == 0 && data.dy == 0){ // slave data
            ht = htrans(s2m_x);
            vt = vtrans(s2m_y);
          }else{  // master data
            ht = htrans(data.dx);
            vt = vtrans(data.dy);
          }
        }

      }else{ // different mode

        if(!is_drag_scroll_m){ // m=xy, s=hv
          xt = xytrans(data.dx, V99_X_TRANSFORM_M);
          yt = xytrans(data.dy, V99_Y_TRANSFORM_M);

          ht = htrans(s2m_x);
          vt = vtrans(s2m_y);
        }else{  // m=hv, s=xy
          xt = xytrans(s2m_x, V99_X_TRANSFORM_S);
          yt = xytrans(s2m_y, V99_Y_TRANSFORM_S);

          ht = htrans(data.dx);
          vt = vtrans(data.dy);
        }

      }

      s2m_x = 0;
      s2m_y = 0;
      data.dx = 0;
      data.dy = 0;

    }else{ // keyboard slave
      // send xy value to keyboard master
      s2m_x = data.dx;
      s2m_y = data.dy;

      xt = data.dx;
      yt = data.dy;
      ht = 0;
      vt = 0;
    }

    //process_mouse_user(mouse_report, xti, yti);

    mouse_report->h = (int16_t)ht * -1;
    mouse_report->v = (int16_t)vt;
    mouse_report->x = (int16_t)xt;
    mouse_report->y = (int16_t)yt * -1;

}
#endif

#if 0
bool process_record_kb(uint16_t keycode, keyrecord_t* record) {

		if (keycode == KC_M) {
#ifndef V99PAD_DRAGSCROLL_MOMENTARY
        if (record->event.pressed)
#endif
        {
            is_drag_scroll ^= 1;
        }
    }
    return true;

}
#endif

void pointing_device_init(void) {

	v99_init();
    wait_ms(55);

}



void pointing_device_task(void) {

    report_mouse_t mouse_report = pointing_device_get_report();
    process_mouse(&mouse_report);
    pointing_device_set_report(mouse_report);
    pointing_device_send();

}

typedef struct _master_to_slave_t {
    //uint8_t m2s_id;
    bool m2s_id;
} master_to_slave_t;

typedef struct _slave_to_master_t {
    uint8_t slave_x;
    uint8_t slave_y;
} slave_to_master_t;

void user_sync_a_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
//void user_sync_a_slave_handler(uint8_t out_buflen, void* out_data) {
    //const master_to_slave_t *m2s = (const master_to_slave_t*)in_data;
    slave_to_master_t *s2m = (slave_to_master_t*)out_data;
    //if(m2s->m2s_id == V99MASTER){
      s2m->slave_x = s2m_x;
      s2m->slave_y = s2m_y;
    //}
}

void eeconfig_init_user(void) {  // EEPROM is getting reset!

  bool dpi_change_f = false;
  user_config.raw = 0;
  
  
  user_config.raw = eeconfig_read_user();
  if(user_config.v99_dpi_master >= DPI_OPTION_SIZE){
    user_config.v99_dpi_master = V99_DPI_MASTER_DEFAULT;
    dpi_change_f = true;
  }

  if(user_config.v99_dpi_slave >= DPI_OPTION_SIZE){
    user_config.v99_dpi_slave = V99_DPI_SLAVE_DEFAULT;
    dpi_change_f = true;
  }
  
  if(dpi_change_f){
    eeconfig_update_user(user_config.raw); // Write default value to EEPROM now
  }
  
  //user_config.v99_dpi_master = V99_DPI_MASTER_DEFAULT; // We want this enabled by default
  //user_config.v99_dpi_slave = V99_DPI_SLAVE_DEFAULT;
  //eeconfig_update_user(user_config.raw); // Write default value to EEPROM now  

}

void matrix_init_user(void){
  dprintf("init MS XY transform value \n");
  eeconfig_init_user();
}

void keyboard_post_init_user(void) {
    debug_enable=true;
    debug_matrix=true;
    transaction_register_rpc(USER_SYNCXY, user_sync_a_slave_handler);

    user_config.raw = eeconfig_read_user();

    V99_X_TRANSFORM_M = (float)dpi_array[user_config.v99_dpi_master] * (float)-1;
    V99_Y_TRANSFORM_M = (float)dpi_array[user_config.v99_dpi_master] * (float)1;
    V99_X_TRANSFORM_S = (float)dpi_array[user_config.v99_dpi_slave]  * (float)-1;
    V99_Y_TRANSFORM_S = (float)dpi_array[user_config.v99_dpi_slave]  * (float)1;

}

void housekeeping_task_user(void) {
    if (is_keyboard_master()) {
        // Interact with slave every 500ms
        static uint32_t last_sync = 0;
        //if (timer_elapsed32(last_sync) > 500) {
        if (timer_elapsed32(last_sync) > 10) { // 60hz
            //master_to_slave_t m2s = {V99MASTER};
            slave_to_master_t s2m = { 0, 0};

            //if(transaction_rpc_exec(USER_SYNCXY, sizeof(m2s), &m2s, sizeof(s2m), &s2m)) {
            if(transaction_rpc_recv(USER_SYNCXY, sizeof(s2m), &s2m)) {
                last_sync = timer_read32();
                //dprintf("Slave X value: %d\n", s2m.slave_x); // this will now be 11, as the slave adds 5
                //dprintf("Slave Y value: %d\n", s2m.slave_y); // this will now be 11, as the slave adds 5
                s2m_x = s2m.slave_x;
                s2m_y = s2m.slave_y;
            } else {
                dprint("Slave sync failed!\n");
            }

        }
    }
}
#endif