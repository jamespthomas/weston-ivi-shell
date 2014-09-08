/***************************************************************************
 *
 * Copyright (c) 2014 Codethink Limited
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/

#include <linux/input.h>
#include <ctype.h>
#include <string.h>

#include <weston/compositor.h>

#include "calibration-util.h"

void
calibration_strip_whitespace(char *str)
{
    char *pos, *end;

    pos = str;
    while (isspace(*pos))
        pos++;

    if (*pos == '\0') {
        weston_log("%s: string '%s' is all spaces\n", __FUNCTION__, str);
        return;
    }

    memmove(str, pos, strlen(str));

    end = str + strlen(str) - 1;
    while (end > str && isspace(*end))
        end--;
    *(end + 1) = '\0';
}

int
calibration_split_line(const char *line, char *key, char *val)
{
    char *delim = strchr(line, ':');
    if (delim == NULL) {
        weston_log("%s: Parsing error: Line %s has no ':'\n",
                   __FUNCTION__, line);
        return 0;
    }

    strncpy(key, line, delim - line);
    key[delim - line] = '\0';
    calibration_strip_whitespace(key);

    strcpy(val, delim + 1);
    calibration_strip_whitespace(val);

    return 1;
}

int
parse_key_code(const char *str)
{
    if (strcmp(str, "KEY_RESERVED") == 0) { return KEY_RESERVED; }
    if (strcmp(str, "KEY_ESC") == 0) { return KEY_ESC; }
    if (strcmp(str, "KEY_1") == 0) { return KEY_1; }
    if (strcmp(str, "KEY_2") == 0) { return KEY_2; }
    if (strcmp(str, "KEY_3") == 0) { return KEY_3; }
    if (strcmp(str, "KEY_4") == 0) { return KEY_4; }
    if (strcmp(str, "KEY_5") == 0) { return KEY_5; }
    if (strcmp(str, "KEY_6") == 0) { return KEY_6; }
    if (strcmp(str, "KEY_7") == 0) { return KEY_7; }
    if (strcmp(str, "KEY_8") == 0) { return KEY_8; }
    if (strcmp(str, "KEY_9") == 0) { return KEY_9; }
    if (strcmp(str, "KEY_0") == 0) { return KEY_0; }
    if (strcmp(str, "KEY_MINUS") == 0) { return KEY_MINUS; }
    if (strcmp(str, "KEY_EQUAL") == 0) { return KEY_EQUAL; }
    if (strcmp(str, "KEY_BACKSPACE") == 0) { return KEY_BACKSPACE; }
    if (strcmp(str, "KEY_TAB") == 0) { return KEY_TAB; }
    if (strcmp(str, "KEY_Q") == 0) { return KEY_Q; }
    if (strcmp(str, "KEY_W") == 0) { return KEY_W; }
    if (strcmp(str, "KEY_E") == 0) { return KEY_E; }
    if (strcmp(str, "KEY_R") == 0) { return KEY_R; }
    if (strcmp(str, "KEY_T") == 0) { return KEY_T; }
    if (strcmp(str, "KEY_Y") == 0) { return KEY_Y; }
    if (strcmp(str, "KEY_U") == 0) { return KEY_U; }
    if (strcmp(str, "KEY_I") == 0) { return KEY_I; }
    if (strcmp(str, "KEY_O") == 0) { return KEY_O; }
    if (strcmp(str, "KEY_P") == 0) { return KEY_P; }
    if (strcmp(str, "KEY_LEFTBRACE") == 0) { return KEY_LEFTBRACE; }
    if (strcmp(str, "KEY_RIGHTBRACE") == 0) { return KEY_RIGHTBRACE; }
    if (strcmp(str, "KEY_ENTER") == 0) { return KEY_ENTER; }
    if (strcmp(str, "KEY_LEFTCTRL") == 0) { return KEY_LEFTCTRL; }
    if (strcmp(str, "KEY_A") == 0) { return KEY_A; }
    if (strcmp(str, "KEY_S") == 0) { return KEY_S; }
    if (strcmp(str, "KEY_D") == 0) { return KEY_D; }
    if (strcmp(str, "KEY_F") == 0) { return KEY_F; }
    if (strcmp(str, "KEY_G") == 0) { return KEY_G; }
    if (strcmp(str, "KEY_H") == 0) { return KEY_H; }
    if (strcmp(str, "KEY_J") == 0) { return KEY_J; }
    if (strcmp(str, "KEY_K") == 0) { return KEY_K; }
    if (strcmp(str, "KEY_L") == 0) { return KEY_L; }
    if (strcmp(str, "KEY_SEMICOLON") == 0) { return KEY_SEMICOLON; }
    if (strcmp(str, "KEY_APOSTROPHE") == 0) { return KEY_APOSTROPHE; }
    if (strcmp(str, "KEY_GRAVE") == 0) { return KEY_GRAVE; }
    if (strcmp(str, "KEY_LEFTSHIFT") == 0) { return KEY_LEFTSHIFT; }
    if (strcmp(str, "KEY_BACKSLASH") == 0) { return KEY_BACKSLASH; }
    if (strcmp(str, "KEY_Z") == 0) { return KEY_Z; }
    if (strcmp(str, "KEY_X") == 0) { return KEY_X; }
    if (strcmp(str, "KEY_C") == 0) { return KEY_C; }
    if (strcmp(str, "KEY_V") == 0) { return KEY_V; }
    if (strcmp(str, "KEY_B") == 0) { return KEY_B; }
    if (strcmp(str, "KEY_N") == 0) { return KEY_N; }
    if (strcmp(str, "KEY_M") == 0) { return KEY_M; }
    if (strcmp(str, "KEY_COMMA") == 0) { return KEY_COMMA; }
    if (strcmp(str, "KEY_DOT") == 0) { return KEY_DOT; }
    if (strcmp(str, "KEY_SLASH") == 0) { return KEY_SLASH; }
    if (strcmp(str, "KEY_RIGHTSHIFT") == 0) { return KEY_RIGHTSHIFT; }
    if (strcmp(str, "KEY_KPASTERISK") == 0) { return KEY_KPASTERISK; }
    if (strcmp(str, "KEY_LEFTALT") == 0) { return KEY_LEFTALT; }
    if (strcmp(str, "KEY_SPACE") == 0) { return KEY_SPACE; }
    if (strcmp(str, "KEY_CAPSLOCK") == 0) { return KEY_CAPSLOCK; }
    if (strcmp(str, "KEY_F1") == 0) { return KEY_F1; }
    if (strcmp(str, "KEY_F2") == 0) { return KEY_F2; }
    if (strcmp(str, "KEY_F3") == 0) { return KEY_F3; }
    if (strcmp(str, "KEY_F4") == 0) { return KEY_F4; }
    if (strcmp(str, "KEY_F5") == 0) { return KEY_F5; }
    if (strcmp(str, "KEY_F6") == 0) { return KEY_F6; }
    if (strcmp(str, "KEY_F7") == 0) { return KEY_F7; }
    if (strcmp(str, "KEY_F8") == 0) { return KEY_F8; }
    if (strcmp(str, "KEY_F9") == 0) { return KEY_F9; }
    if (strcmp(str, "KEY_F10") == 0) { return KEY_F10; }
    if (strcmp(str, "KEY_NUMLOCK") == 0) { return KEY_NUMLOCK; }
    if (strcmp(str, "KEY_SCROLLLOCK") == 0) { return KEY_SCROLLLOCK; }
    if (strcmp(str, "KEY_KP7") == 0) { return KEY_KP7; }
    if (strcmp(str, "KEY_KP8") == 0) { return KEY_KP8; }
    if (strcmp(str, "KEY_KP9") == 0) { return KEY_KP9; }
    if (strcmp(str, "KEY_KPMINUS") == 0) { return KEY_KPMINUS; }
    if (strcmp(str, "KEY_KP4") == 0) { return KEY_KP4; }
    if (strcmp(str, "KEY_KP5") == 0) { return KEY_KP5; }
    if (strcmp(str, "KEY_KP6") == 0) { return KEY_KP6; }
    if (strcmp(str, "KEY_KPPLUS") == 0) { return KEY_KPPLUS; }
    if (strcmp(str, "KEY_KP1") == 0) { return KEY_KP1; }
    if (strcmp(str, "KEY_KP2") == 0) { return KEY_KP2; }
    if (strcmp(str, "KEY_KP3") == 0) { return KEY_KP3; }
    if (strcmp(str, "KEY_KP0") == 0) { return KEY_KP0; }
    if (strcmp(str, "KEY_KPDOT") == 0) { return KEY_KPDOT; }
    if (strcmp(str, "KEY_ZENKAKUHANKAKU") == 0) { return KEY_ZENKAKUHANKAKU; }
    if (strcmp(str, "KEY_102ND") == 0) { return KEY_102ND; }
    if (strcmp(str, "KEY_F11") == 0) { return KEY_F11; }
    if (strcmp(str, "KEY_F12") == 0) { return KEY_F12; }
    if (strcmp(str, "KEY_RO") == 0) { return KEY_RO; }
    if (strcmp(str, "KEY_KATAKANA") == 0) { return KEY_KATAKANA; }
    if (strcmp(str, "KEY_HIRAGANA") == 0) { return KEY_HIRAGANA; }
    if (strcmp(str, "KEY_HENKAN") == 0) { return KEY_HENKAN; }
    if (strcmp(str, "KEY_KATAKANAHIRAGANA") == 0) { return KEY_KATAKANAHIRAGANA; }
    if (strcmp(str, "KEY_MUHENKAN") == 0) { return KEY_MUHENKAN; }
    if (strcmp(str, "KEY_KPJPCOMMA") == 0) { return KEY_KPJPCOMMA; }
    if (strcmp(str, "KEY_KPENTER") == 0) { return KEY_KPENTER; }
    if (strcmp(str, "KEY_RIGHTCTRL") == 0) { return KEY_RIGHTCTRL; }
    if (strcmp(str, "KEY_KPSLASH") == 0) { return KEY_KPSLASH; }
    if (strcmp(str, "KEY_SYSRQ") == 0) { return KEY_SYSRQ; }
    if (strcmp(str, "KEY_RIGHTALT") == 0) { return KEY_RIGHTALT; }
    if (strcmp(str, "KEY_LINEFEED") == 0) { return KEY_LINEFEED; }
    if (strcmp(str, "KEY_HOME") == 0) { return KEY_HOME; }
    if (strcmp(str, "KEY_UP") == 0) { return KEY_UP; }
    if (strcmp(str, "KEY_PAGEUP") == 0) { return KEY_PAGEUP; }
    if (strcmp(str, "KEY_LEFT") == 0) { return KEY_LEFT; }
    if (strcmp(str, "KEY_RIGHT") == 0) { return KEY_RIGHT; }
    if (strcmp(str, "KEY_END") == 0) { return KEY_END; }
    if (strcmp(str, "KEY_DOWN") == 0) { return KEY_DOWN; }
    if (strcmp(str, "KEY_PAGEDOWN") == 0) { return KEY_PAGEDOWN; }
    if (strcmp(str, "KEY_INSERT") == 0) { return KEY_INSERT; }
    if (strcmp(str, "KEY_DELETE") == 0) { return KEY_DELETE; }
    if (strcmp(str, "KEY_MACRO") == 0) { return KEY_MACRO; }
    if (strcmp(str, "KEY_MUTE") == 0) { return KEY_MUTE; }
    if (strcmp(str, "KEY_VOLUMEDOWN") == 0) { return KEY_VOLUMEDOWN; }
    if (strcmp(str, "KEY_VOLUMEUP") == 0) { return KEY_VOLUMEUP; }
    if (strcmp(str, "KEY_POWER") == 0) { return KEY_POWER; }
    if (strcmp(str, "KEY_KPEQUAL") == 0) { return KEY_KPEQUAL; }
    if (strcmp(str, "KEY_KPPLUSMINUS") == 0) { return KEY_KPPLUSMINUS; }
    if (strcmp(str, "KEY_PAUSE") == 0) { return KEY_PAUSE; }
    if (strcmp(str, "KEY_KPCOMMA") == 0) { return KEY_KPCOMMA; }
    if (strcmp(str, "KEY_HANGUEL") == 0) { return KEY_HANGUEL; }
    if (strcmp(str, "KEY_HANJA") == 0) { return KEY_HANJA; }
    if (strcmp(str, "KEY_YEN") == 0) { return KEY_YEN; }
    if (strcmp(str, "KEY_LEFTMETA") == 0) { return KEY_LEFTMETA; }
    if (strcmp(str, "KEY_RIGHTMETA") == 0) { return KEY_RIGHTMETA; }
    if (strcmp(str, "KEY_COMPOSE") == 0) { return KEY_COMPOSE; }
    if (strcmp(str, "KEY_STOP") == 0) { return KEY_STOP; }
    if (strcmp(str, "KEY_AGAIN") == 0) { return KEY_AGAIN; }
    if (strcmp(str, "KEY_PROPS") == 0) { return KEY_PROPS; }
    if (strcmp(str, "KEY_UNDO") == 0) { return KEY_UNDO; }
    if (strcmp(str, "KEY_FRONT") == 0) { return KEY_FRONT; }
    if (strcmp(str, "KEY_COPY") == 0) { return KEY_COPY; }
    if (strcmp(str, "KEY_OPEN") == 0) { return KEY_OPEN; }
    if (strcmp(str, "KEY_PASTE") == 0) { return KEY_PASTE; }
    if (strcmp(str, "KEY_FIND") == 0) { return KEY_FIND; }
    if (strcmp(str, "KEY_CUT") == 0) { return KEY_CUT; }
    if (strcmp(str, "KEY_HELP") == 0) { return KEY_HELP; }
    if (strcmp(str, "KEY_MENU") == 0) { return KEY_MENU; }
    if (strcmp(str, "KEY_CALC") == 0) { return KEY_CALC; }
    if (strcmp(str, "KEY_SETUP") == 0) { return KEY_SETUP; }
    if (strcmp(str, "KEY_SLEEP") == 0) { return KEY_SLEEP; }
    if (strcmp(str, "KEY_WAKEUP") == 0) { return KEY_WAKEUP; }
    if (strcmp(str, "KEY_FILE") == 0) { return KEY_FILE; }
    if (strcmp(str, "KEY_SENDFILE") == 0) { return KEY_SENDFILE; }
    if (strcmp(str, "KEY_DELETEFILE") == 0) { return KEY_DELETEFILE; }
    if (strcmp(str, "KEY_XFER") == 0) { return KEY_XFER; }
    if (strcmp(str, "KEY_PROG1") == 0) { return KEY_PROG1; }
    if (strcmp(str, "KEY_PROG2") == 0) { return KEY_PROG2; }
    if (strcmp(str, "KEY_WWW") == 0) { return KEY_WWW; }
    if (strcmp(str, "KEY_MSDOS") == 0) { return KEY_MSDOS; }
    if (strcmp(str, "KEY_COFFEE") == 0) { return KEY_COFFEE; }
    if (strcmp(str, "KEY_DIRECTION") == 0) { return KEY_DIRECTION; }
    if (strcmp(str, "KEY_CYCLEWINDOWS") == 0) { return KEY_CYCLEWINDOWS; }
    if (strcmp(str, "KEY_MAIL") == 0) { return KEY_MAIL; }
    if (strcmp(str, "KEY_BOOKMARKS") == 0) { return KEY_BOOKMARKS; }
    if (strcmp(str, "KEY_COMPUTER") == 0) { return KEY_COMPUTER; }
    if (strcmp(str, "KEY_BACK") == 0) { return KEY_BACK; }
    if (strcmp(str, "KEY_FORWARD") == 0) { return KEY_FORWARD; }
    if (strcmp(str, "KEY_CLOSECD") == 0) { return KEY_CLOSECD; }
    if (strcmp(str, "KEY_EJECTCD") == 0) { return KEY_EJECTCD; }
    if (strcmp(str, "KEY_EJECTCLOSECD") == 0) { return KEY_EJECTCLOSECD; }
    if (strcmp(str, "KEY_NEXTSONG") == 0) { return KEY_NEXTSONG; }
    if (strcmp(str, "KEY_PLAYPAUSE") == 0) { return KEY_PLAYPAUSE; }
    if (strcmp(str, "KEY_PREVIOUSSONG") == 0) { return KEY_PREVIOUSSONG; }
    if (strcmp(str, "KEY_STOPCD") == 0) { return KEY_STOPCD; }
    if (strcmp(str, "KEY_RECORD") == 0) { return KEY_RECORD; }
    if (strcmp(str, "KEY_REWIND") == 0) { return KEY_REWIND; }
    if (strcmp(str, "KEY_PHONE") == 0) { return KEY_PHONE; }
    if (strcmp(str, "KEY_ISO") == 0) { return KEY_ISO; }
    if (strcmp(str, "KEY_CONFIG") == 0) { return KEY_CONFIG; }
    if (strcmp(str, "KEY_HOMEPAGE") == 0) { return KEY_HOMEPAGE; }
    if (strcmp(str, "KEY_REFRESH") == 0) { return KEY_REFRESH; }
    if (strcmp(str, "KEY_EXIT") == 0) { return KEY_EXIT; }
    if (strcmp(str, "KEY_MOVE") == 0) { return KEY_MOVE; }
    if (strcmp(str, "KEY_EDIT") == 0) { return KEY_EDIT; }
    if (strcmp(str, "KEY_SCROLLUP") == 0) { return KEY_SCROLLUP; }
    if (strcmp(str, "KEY_SCROLLDOWN") == 0) { return KEY_SCROLLDOWN; }
    if (strcmp(str, "KEY_KPLEFTPAREN") == 0) { return KEY_KPLEFTPAREN; }

    return -1;
}
