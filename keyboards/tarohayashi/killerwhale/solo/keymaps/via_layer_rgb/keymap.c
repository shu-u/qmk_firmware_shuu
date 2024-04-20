// Copyright 2021 Hayashi (@w_vwbw)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "add_keycodes.h"
#include "add_joystick.h"
#include "add_rgblayers.h"

// レイヤー名
enum layer_number {
    // 左手用
    LEFT_BASE = 0, LEFT_LOWER,  LEFT_UPPER,            
    // 右手用
    RIGHT_BASE, RIGHT_LOWER, RIGHT_UPPER,
    // 自動マウスレイヤー切り替えや設定用のレイヤー
    UTIL, MOUSE , BALL_SETTINGS, LIGHT_SETTINGS    
};
// キーマップ
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LEFT_BASE] = LAYOUT(
        // 天面スイッチ
        KC_ESC,  KC_1, KC_2,                 KC_3, LT(BALL_SETTINGS, KC_4), LT(LIGHT_SETTINGS, KC_5),
        KC_TAB,  KC_Q, KC_W,                 KC_E, KC_R,                    KC_T,
        CMD_CTL, KC_A, KC_S, KC_D, KC_F,                    KC_G,
                 LSFT_T(KC_Z), KC_X,                 KC_C, KC_V,                    KC_B,
                       MOD_SCRL,
        // 側面スイッチ
        KC_LNG2, KC_SPACE,                                  
        // 十字キーorジョイスティック                // ジョイスティックスイッチ
        KC_UP, KC_DOWN, KC_LEFT, KC_RIGHT,         KC_ENT,                 
        // 追加スイッチ                             // トグルスイッチ
        KC_MS_BTN2, KC_MS_BTN1,                    MO(RIGHT_BASE)
    ),    
    [RIGHT_BASE] = LAYOUT(
        KC_BSPC,    KC_0,       KC_9,            KC_8,            LT(BALL_SETTINGS, KC_7), LT(LIGHT_SETTINGS, KC_6),
        KC_ENT,     KC_P,       KC_O,            KC_I,            KC_U,                 KC_Y,
        KC_RSFT,    KC_SCLN,    LT(MOUSE, KC_L), KC_K,            KC_J,                 KC_H,
                    KC_SLSH,    KC_DOT,          KC_COMM,         KC_M,                 KC_N,
                                MOD_SCRL,
        KC_SPACE,KC_LNG1,
        KC_LEFT, KC_RIGHT, KC_DOWN, KC_UP,       KC_ENT,
        KC_MS_BTN2, KC_MS_BTN1,                  _______    
    ),
    [MOUSE] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,    _______,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX,
        _______, XXXXXXX, _______, KC_MS_BTN2, KC_MS_BTN1, MOD_SCRL,
                 XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
                          MOD_SCRL,
        XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, XXXXXXX,                            XXXXXXX

    ),
    [BALL_SETTINGS] = LAYOUT(
        XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, _______,  L_CHMOD,
        XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, CPI_I,    XXXXXXX,
        AUTO_MOUSE, XXXXXXX, XXXXXXX, L_ANG_D,   L_INV, L_ANG_I,
                    XXXXXXX, XXXXXXX, XXXXXXX, CPI_D,    XXXXXXX,
                          INV_SCRL,
        XXXXXXX, XXXXXXX,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, XXXXXXX,                            XXXXXXX
    ),
    [LIGHT_SETTINGS] = LAYOUT(
        XXXXXXX, XXXXXXX, XXXXXXX, RGB_MOD, RGB_MOD, _______,
        XXXXXXX, RGB_SPI, RGB_VAI, RGB_SAI, RGB_HUI, RGB_TOG,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                 RGB_SPD, RGB_VAD, RGB_SAD, RGB_HUD, XXXXXXX,
                          XXXXXXX,
        RGB_MOD, RGB_MOD,
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX,
        XXXXXXX, XXXXXXX,                            XXXXXXX
    )
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [LEFT_BASE] =   { 
        ENCODER_CCW_CW(KC_ESC, KC_TAB), 
        ENCODER_CCW_CW(REDO, UNDO), 
        ENCODER_CCW_CW(KC_WH_U, KC_WH_D),
        ENCODER_CCW_CW(KC_WH_U, KC_WH_D)
    },
    [RIGHT_BASE] =   { 
        ENCODER_CCW_CW(KC_DEL, KC_BSPC), 
        ENCODER_CCW_CW(KC_UP, KC_DOWN), 
        ENCODER_CCW_CW(KC_WH_U, KC_WH_D),
        ENCODER_CCW_CW(KC_WH_U, KC_WH_D)
    },
    [LIGHT_SETTINGS] = { 
        ENCODER_CCW_CW(RGB_SPI, RGB_SPD), 
        ENCODER_CCW_CW(RGB_VAI, RGB_VAD), 
        ENCODER_CCW_CW(RGB_SAI, RGB_SAD), 
        ENCODER_CCW_CW(RGB_HUI, RGB_HUD)
    }
};

// 初期化
void matrix_init_user(void) {
    matrix_init_addedjoystick();
}

void keyboard_post_init_user(void) {
    keyboard_post_init_rgblayers();
}

// キースキャン
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    process_record_addedkeycodes(keycode, record);
    return true;
}

// マトリックススキャン
void matrix_scan_user(void) {
    matrix_scan_addedjoystick();
}

