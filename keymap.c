/* Copyright 2021 Jay Greco
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
//#include "bongo.h"
#include <stdio.h>
#include "raw_hid.h"
#include <string.h>
#ifdef AUDIO_ENABLE
#include "audio.h"
#endif


// NOTE:
// In order to get the slave oled to receive keypresses:
// See: https://zenn.dev/teppeis/articles/2021-05-qmk-fire-process-record-in-slave

// clang-format off
enum layers {
    _BASE,
    _FUNC,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_ansi(
             KC_ESC,   KC_F1,   KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,    KC_F7,  KC_F8,  KC_F9,   KC_F10,    KC_F11,    KC_F12,   KC_HOME,  KC_DEL,
    KC_MPLY, KC_GRV,   KC_1,    KC_2,   KC_3,   KC_4,   KC_5,   KC_6,     KC_7,   KC_8,   KC_9,    KC_0,      KC_MINS,   KC_EQL,       KC_BSPC,       KC_MPLY,
    KC_NO,   KC_TAB,   KC_Q,    KC_W,   KC_E,   KC_R,           KC_T,     KC_Y,   KC_U,   KC_I,    KC_O,      KC_P,      KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_END,  
    KC_MUTE,  KC_CAPS,  KC_A,    KC_S,   KC_D,   KC_F,           KC_G,     KC_H,   KC_J,   KC_K,    KC_L,      KC_SCLN,   KC_QUOT,  KC_ENT,             KC_PGUP,
    KC_MNXT,  KC_LSFT,   KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,     KC_N,   KC_M,   KC_COMM, KC_DOT,    KC_SLSH,   KC_RSFT,            KC_UP,    KC_PGDN,
    KC_PSCR,  KC_LCTL,  KC_LGUI, KC_LALT,     MO(_FUNC),         KC_SPC,   KC_SPC,                  KC_RALT,    MO(_FUNC),   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
  ),
  [_FUNC] = LAYOUT_ansi(
             RGB_TOG, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, QK_BOOT,
    KC_TRNS, KC_TRNS, RGB_RMOD, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS,     KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
    KC_TRNS,     KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, KC_TRNS,
    QK_BOOT, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,          KC_TRNS, KC_TRNS,                   KC_TRNS, KC_TRNS, KC_TRNS, KC_MPRV, KC_MPLY, KC_MNXT
    ),
};
// clang-format on
uint8_t current_wpm = 0;

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [_FUNC] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD),     ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
};
#endif

#ifdef AUDIO_ENABLE
float caps_lock_on_song[][2] = SONG(MARIO_MUSHROOM);
float caps_lock_off_song[][2] = SONG(MARIO_MUSHROOM_LOST);
float layer_on_song[][2] = SONG(LAYER_ON_SOUND);
float layer_off_song[][2] = SONG(LAYER_OFF_SOUND);
float func_special_song[][2] = SONG(MEGALOVANIA);
#endif


oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_0;  // Master side (left) orientation
    } else {
        return OLED_ROTATION_180; // Slave side (right) flipped 180 degrees
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    #ifdef AUDIO_ENABLE
    switch (get_highest_layer(state)) {
        case _FUNC:
            PLAY_SONG(layer_on_song);
            break;
        default:
            break;
    }
    #endif
    return state;
}

static void render_status(void) {
    oled_set_cursor(0, 0);
    oled_write_P(PSTR("CHOPZ "), false);
    oled_write_P(PSTR("Layer "), false);
    switch (get_highest_layer(layer_state)) {
        case _FUNC:
            oled_write_P(PSTR("FN1 "), false);
            break;
        default: // use BASE case as default
            oled_write_P(PSTR("Base"), false);
            
    }

    // Host Keyboard LED Status
    oled_set_cursor(0, 1);
    static led_t persistent_led_state = {0};
    led_t led_state = host_keyboard_led_state();

    // Only update if the LED state has changed
    // Otherwise, the OLED will not turn off if an LED is on.
    if (persistent_led_state.raw != led_state.raw) {
        persistent_led_state = led_state;
        
        oled_write_ln_P(PSTR(""), false);

        if (led_state.caps_lock) {
            oled_set_cursor(0, 1);
            oled_write_P(PSTR("CAPS"), true);
        }

        //if (led_state.num_lock) {
        //    oled_set_cursor(5, 1);
        //    oled_write_P(PSTR("NUM"), false);
        //}

        if (led_state.scroll_lock) {
            oled_set_cursor(9, 1);
            oled_write_P(PSTR("SCR"), true);
        }
    }

    // WPM and max WPM
    oled_set_cursor(0, 2);
    oled_write_P(PSTR("WPM "), false);
    oled_write(get_u8_str(current_wpm, '0'), true);

    oled_set_cursor(8, 2);
    oled_write_P(PSTR("MAX "), false);
    static uint8_t max_wpm;
    max_wpm = MAX(max_wpm, current_wpm);
    oled_write(get_u8_str(max_wpm, '0'), true);
}


// HID data storage
char hid_data[32] = {0};
// Parsed values from HID data
char cpu_load[8] = "0%";
char cpu_temp[8] = "0C";
char gpu_temp[8] = "0C";
bool hid_data_received = false;

// Function to parse the HID data
void parse_hid_data(char* data, uint8_t length) {
    if (length == 0) return;
    
    // Format expected: "CPU_LOAD%|CPU_TEMP|GPU_TEMP"
    char* token;
    char temp_data[32];
    
    // Make a copy of the data for tokenization
    strncpy(temp_data, data, sizeof(temp_data) - 1);
    temp_data[sizeof(temp_data) - 1] = '\0';
    
    // Parse CPU load
    token = strtok(temp_data, "|");
    if (token != NULL) {
        strncpy(cpu_load, token, sizeof(cpu_load) - 1);
        cpu_load[sizeof(cpu_load) - 1] = '\0';
    }
    
    // Parse CPU temp
    token = strtok(NULL, "|");
    if (token != NULL) {
        strncpy(cpu_temp, token, sizeof(cpu_temp) - 1);
        cpu_temp[sizeof(cpu_temp) - 1] = '\0';
    }
    
    // Parse GPU temp
    token = strtok(NULL, "|");
    if (token != NULL) {
        strncpy(gpu_temp, token, sizeof(gpu_temp) - 1);
        gpu_temp[sizeof(gpu_temp) - 1] = '\0';
    }
    
    hid_data_received = true;
}
// Function to render the system info on the second OLED
static void render_system_info(void) {
    // Title
    oled_set_cursor(0, 0);
    oled_write_P(PSTR("SYSTEM INFO"), false);
    
    // CPU Load
    oled_set_cursor(0, 1);
    oled_write_P(PSTR("CPU: "), false);
    oled_write(cpu_load, false);
    
    // CPU Temperature
    oled_set_cursor(0, 2);
    oled_write_P(PSTR("CPU: "), false);
    oled_write(cpu_temp, false);
    
    // GPU Temperature
    oled_set_cursor(0, 3);
    oled_write_P(PSTR("GPU: "), false);
    oled_write(gpu_temp, false);
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
    // Copy the received data to our buffer, convert from uint8_t to char array
    memset(hid_data, 0, sizeof(hid_data));
    for (uint8_t i = 0; i < length && i < sizeof(hid_data) - 1; i++) {
        hid_data[i] = (char)data[i];
    }
    
    // Parse the data
    parse_hid_data(hid_data, strlen(hid_data));
    
    // Echo back to the host (optional for debugging)
    raw_hid_send(data, length);
}
//==============================
//==============================


bool oled_task_user(void) {
    // Update wpm
    current_wpm = get_current_wpm();

    if (is_keyboard_master()) {
        render_system_info();
    } else {
        render_status();

    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_CAPS:
            if (record->event.pressed) {
                #ifdef AUDIO_ENABLE
                    if (!(host_keyboard_led_state().caps_lock)) {
                        PLAY_SONG(caps_lock_on_song);
                    } else {
                        PLAY_SONG(caps_lock_off_song);
                    }
                #endif
            }
            return true;
        
        // New addition for unmapped key on _FUNC layer
        default:
            // Check if we're on the FUNC layer
            if (layer_state_is(_FUNC)) {
                // If an unmapped key is pressed
                if (keycode == KC_NO && record->event.pressed) {
                    #ifdef AUDIO_ENABLE
                    PLAY_SONG(func_special_song);
                    #endif
                    return false;
                }
            }
            return true;
    }
}
bool should_process_keypress(void) {
    return true;
}
