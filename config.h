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

#pragma once

#define OLED_BRIGHTNESS 128
#define OLED_TIMEOUT 30000

// Split Options
#define SPLIT_TRANSPORT_MIRROR
#define SPLIT_WPM_ENABLE
#define SPLIT_LED_STATE_ENABLE
#define SPLIT_OLED_ENABLE

#define AUDIO_PWM_DRIVER PWMD2
#define AUDIO_PWM_CHANNEL RP2040_PWM_CHANNEL_B
#define AUDIO_INIT_DELAY
#define AUDIO_PIN GP21


#define ZELDA_TREASURE \
    Q__NOTE(_A4), \
    Q__NOTE(_AS4), \
    Q__NOTE(_B4), \
    HD_NOTE(_C5), \

#define ZELDA_PUZZLE \
    Q__NOTE(_G5),     \
    Q__NOTE(_FS5),    \
    Q__NOTE(_DS5),     \
    Q__NOTE(_A4),    \
    Q__NOTE(_GS4),     \
    Q__NOTE(_E5),     \
    Q__NOTE(_GS5),     \
    HD_NOTE(_C6),

#define MARIO_MUSHROOM_LOST \
     S__NOTE(_C6 ), \
     S__NOTE(_GS5), \
     S__NOTE(_F5 ), \
     S__NOTE(_DS5), \
     S__NOTE(_C5 ), \
     S__NOTE(_AS4), \
     S__NOTE(_GS4), \
     S__NOTE(_F4 ), \
     S__NOTE(_DS4), \
     S__NOTE(_C4 ), \
     S__NOTE(_AS3), \
     S__NOTE(_GS3), \
     S__NOTE(_C6 ), \
     S__NOTE(_GS5), \
     S__NOTE(_F5 ), \
     S__NOTE(_DS5), \
     S__NOTE(_C5 ), \
     S__NOTE(_AS4), \
     S__NOTE(_GS4), \
     S__NOTE(_F4 ), \
     S__NOTE(_DS4), \
     S__NOTE(_C4 ), \
     S__NOTE(_AS3), \
     S__NOTE(_GS3), \
     S__NOTE(_C6 ), \
     S__NOTE(_GS5), \
     S__NOTE(_F5 ), \
     S__NOTE(_DS5), \
     S__NOTE(_C5 ), \
     S__NOTE(_AS4), \
     S__NOTE(_GS4), \
     S__NOTE(_F4 ), \
     S__NOTE(_DS4), \
     S__NOTE(_C4 ), \
     S__NOTE(_AS3), \
     S__NOTE(_GS3)

#define MARIO_MUSHROOM \
    S__NOTE(_C5 ), \
    S__NOTE(_G4 ), \
    S__NOTE(_C5 ), \
    S__NOTE(_E5 ), \
    S__NOTE(_G5 ), \
    S__NOTE(_C6 ), \
    S__NOTE(_G5 ), \
    S__NOTE(_GS4), \
    S__NOTE(_C5 ), \
    S__NOTE(_DS5), \
    S__NOTE(_GS5), \
    S__NOTE(_DS5), \
    S__NOTE(_GS5), \
    S__NOTE(_C6 ), \
    S__NOTE(_DS6), \
    S__NOTE(_GS6), \
    S__NOTE(_DS6), \
    S__NOTE(_AS4), \
    S__NOTE(_D5 ), \
    S__NOTE(_F5 ), \
    S__NOTE(_AS5), \
    S__NOTE(_D6 ), \
    S__NOTE(_F6 ), \
    S__NOTE(_AS6), \
    S__NOTE(_F6 )

#define MEGALOVANIA \
    Q__NOTE(_D4), Q__NOTE(_D4), H__NOTE(_D5), HD_NOTE(_A4), H__NOTE(_AF4), H__NOTE(_G4), H__NOTE(_F4), \
    Q__NOTE(_D4), Q__NOTE(_F4), Q__NOTE(_G4), Q__NOTE(_C4), Q__NOTE(_C4), H__NOTE(_D5), HD_NOTE(_A4), \
    H__NOTE(_AF4), H__NOTE(_G4), H__NOTE(_F4), Q__NOTE(_D4), Q__NOTE(_F4), Q__NOTE(_G4), \
    Q__NOTE(_B3), Q__NOTE(_B3), H__NOTE(_D5), HD_NOTE(_A4), H__NOTE(_AF4), H__NOTE(_G4), H__NOTE(_F4), \
    Q__NOTE(_D4), Q__NOTE(_F4), Q__NOTE(_G4), Q__NOTE(_BF3), Q__NOTE(_BF3), H__NOTE(_D5), \
    HD_NOTE(_A4), H__NOTE(_AF4), H__NOTE(_G4), H__NOTE(_F4), Q__NOTE(_D4), Q__NOTE(_F4), \
    Q__NOTE(_G4),

#define LAYER_ON_SOUND E__NOTE(_A3), E__NOTE(_B3),

#define LAYER_OFF_SOUND E__NOTE(_B3), E__NOTE(_A3),

#ifdef AUDIO_ENABLE
    #define STARTUP_SONG SONG(ZELDA_PUZZLE)
#endif


#define AUDIO_ENABLE_TONE_MULTIPLEXING
#define AUDIO_TONE_MULTIPLEXING_RATE_DEFAULT 10
