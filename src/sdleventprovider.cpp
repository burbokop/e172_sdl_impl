#include "sdleventprovider.h"

#include <SDL2/SDL.h>

namespace {
e172::Scancode scancodeFromSDL(SDL_Scancode s)
{
    switch (s) {
    case SDL_SCANCODE_UNKNOWN:
        todo;
    case SDL_SCANCODE_A:
        return e172::ScancodeA;
    case SDL_SCANCODE_B:
        return e172::ScancodeB;
    case SDL_SCANCODE_C:
        todo;
    case SDL_SCANCODE_D:
        return e172::ScancodeD;
    case SDL_SCANCODE_E:
        return e172::ScancodeE;
    case SDL_SCANCODE_F:
        todo;
    case SDL_SCANCODE_G:
        todo;
    case SDL_SCANCODE_H:
        todo;
    case SDL_SCANCODE_I:
        todo;
    case SDL_SCANCODE_J:
        todo;
    case SDL_SCANCODE_K:
        todo;
    case SDL_SCANCODE_L:
        todo;
    case SDL_SCANCODE_M:
        todo;
    case SDL_SCANCODE_N:
        todo;
    case SDL_SCANCODE_O:
        todo;
    case SDL_SCANCODE_P:
        todo;
    case SDL_SCANCODE_Q:
        todo;
    case SDL_SCANCODE_R:
        todo;
    case SDL_SCANCODE_S:
        return e172::ScancodeS;
    case SDL_SCANCODE_T:
        todo;
    case SDL_SCANCODE_U:
        todo;
    case SDL_SCANCODE_V:
        todo;
    case SDL_SCANCODE_W:
        return e172::ScancodeW;
    case SDL_SCANCODE_X:
        todo;
    case SDL_SCANCODE_Y:
        todo;
    case SDL_SCANCODE_Z:
        todo;
    case SDL_SCANCODE_1:
        todo;
    case SDL_SCANCODE_2:
        todo;
    case SDL_SCANCODE_3:
        todo;
    case SDL_SCANCODE_4:
        todo;
    case SDL_SCANCODE_5:
        todo;
    case SDL_SCANCODE_6:
        todo;
    case SDL_SCANCODE_7:
        todo;
    case SDL_SCANCODE_8:
        todo;
    case SDL_SCANCODE_9:
        todo;
    case SDL_SCANCODE_0:
        todo;
    case SDL_SCANCODE_RETURN:
        todo;
    case SDL_SCANCODE_ESCAPE:
        todo;
    case SDL_SCANCODE_BACKSPACE:
        todo;
    case SDL_SCANCODE_TAB:
        todo;
    case SDL_SCANCODE_SPACE:
        return e172::ScancodeSpace;
    case SDL_SCANCODE_MINUS:
        todo;
    case SDL_SCANCODE_EQUALS:
        todo;
    case SDL_SCANCODE_LEFTBRACKET:
        todo;
    case SDL_SCANCODE_RIGHTBRACKET:
        todo;
    case SDL_SCANCODE_BACKSLASH:
        todo;
    case SDL_SCANCODE_NONUSHASH:
        todo;
    case SDL_SCANCODE_SEMICOLON:
        todo;
    case SDL_SCANCODE_APOSTROPHE:
        todo;
    case SDL_SCANCODE_GRAVE:
        todo;
    case SDL_SCANCODE_COMMA:
        todo;
    case SDL_SCANCODE_PERIOD:
        todo;
    case SDL_SCANCODE_SLASH:
        todo;
    case SDL_SCANCODE_CAPSLOCK:
        todo;
    case SDL_SCANCODE_F1:
        todo;
    case SDL_SCANCODE_F2:
        todo;
    case SDL_SCANCODE_F3:
        todo;
    case SDL_SCANCODE_F4:
        todo;
    case SDL_SCANCODE_F5:
        todo;
    case SDL_SCANCODE_F6:
        todo;
    case SDL_SCANCODE_F7:
        todo;
    case SDL_SCANCODE_F8:
        todo;
    case SDL_SCANCODE_F9:
        todo;
    case SDL_SCANCODE_F10:
        todo;
    case SDL_SCANCODE_F11:
        todo;
    case SDL_SCANCODE_F12:
        todo;
    case SDL_SCANCODE_PRINTSCREEN:
        todo;
    case SDL_SCANCODE_SCROLLLOCK:
        todo;
    case SDL_SCANCODE_PAUSE:
        todo;
    case SDL_SCANCODE_INSERT:
        todo;
    case SDL_SCANCODE_HOME:
        todo;
    case SDL_SCANCODE_PAGEUP:
        todo;
    case SDL_SCANCODE_DELETE:
        todo;
    case SDL_SCANCODE_END:
        todo;
    case SDL_SCANCODE_PAGEDOWN:
        todo;
    case SDL_SCANCODE_RIGHT:
        todo;
    case SDL_SCANCODE_LEFT:
        todo;
    case SDL_SCANCODE_DOWN:
        todo;
    case SDL_SCANCODE_UP:
        todo;
    case SDL_SCANCODE_NUMLOCKCLEAR:
        todo;
    case SDL_SCANCODE_KP_DIVIDE:
        todo;
    case SDL_SCANCODE_KP_MULTIPLY:
        todo;
    case SDL_SCANCODE_KP_MINUS:
        todo;
    case SDL_SCANCODE_KP_PLUS:
        todo;
    case SDL_SCANCODE_KP_ENTER:
        todo;
    case SDL_SCANCODE_KP_1:
        todo;
    case SDL_SCANCODE_KP_2:
        todo;
    case SDL_SCANCODE_KP_3:
        todo;
    case SDL_SCANCODE_KP_4:
        todo;
    case SDL_SCANCODE_KP_5:
        todo;
    case SDL_SCANCODE_KP_6:
        todo;
    case SDL_SCANCODE_KP_7:
        todo;
    case SDL_SCANCODE_KP_8:
        todo;
    case SDL_SCANCODE_KP_9:
        todo;
    case SDL_SCANCODE_KP_0:
        todo;
    case SDL_SCANCODE_KP_PERIOD:
        todo;
    case SDL_SCANCODE_NONUSBACKSLASH:
        todo;
    case SDL_SCANCODE_APPLICATION:
        todo;
    case SDL_SCANCODE_POWER:
        todo;
    case SDL_SCANCODE_KP_EQUALS:
        todo;
    case SDL_SCANCODE_F13:
        todo;
    case SDL_SCANCODE_F14:
        todo;
    case SDL_SCANCODE_F15:
        todo;
    case SDL_SCANCODE_F16:
        todo;
    case SDL_SCANCODE_F17:
        todo;
    case SDL_SCANCODE_F18:
        todo;
    case SDL_SCANCODE_F19:
        todo;
    case SDL_SCANCODE_F20:
        todo;
    case SDL_SCANCODE_F21:
        todo;
    case SDL_SCANCODE_F22:
        todo;
    case SDL_SCANCODE_F23:
        todo;
    case SDL_SCANCODE_F24:
        todo;
    case SDL_SCANCODE_EXECUTE:
        todo;
    case SDL_SCANCODE_HELP:
        todo;
    case SDL_SCANCODE_MENU:
        todo;
    case SDL_SCANCODE_SELECT:
        todo;
    case SDL_SCANCODE_STOP:
        todo;
    case SDL_SCANCODE_AGAIN:
        todo;
    case SDL_SCANCODE_UNDO:
        todo;
    case SDL_SCANCODE_CUT:
        todo;
    case SDL_SCANCODE_COPY:
        todo;
    case SDL_SCANCODE_PASTE:
        todo;
    case SDL_SCANCODE_FIND:
        todo;
    case SDL_SCANCODE_MUTE:
        todo;
    case SDL_SCANCODE_VOLUMEUP:
        todo;
    case SDL_SCANCODE_VOLUMEDOWN:
        todo;
    case SDL_SCANCODE_KP_COMMA:
        todo;
    case SDL_SCANCODE_KP_EQUALSAS400:
        todo;
    case SDL_SCANCODE_INTERNATIONAL1:
        todo;
    case SDL_SCANCODE_INTERNATIONAL2:
        todo;
    case SDL_SCANCODE_INTERNATIONAL3:
        todo;
    case SDL_SCANCODE_INTERNATIONAL4:
        todo;
    case SDL_SCANCODE_INTERNATIONAL5:
        todo;
    case SDL_SCANCODE_INTERNATIONAL6:
        todo;
    case SDL_SCANCODE_INTERNATIONAL7:
        todo;
    case SDL_SCANCODE_INTERNATIONAL8:
        todo;
    case SDL_SCANCODE_INTERNATIONAL9:
        todo;
    case SDL_SCANCODE_LANG1:
        todo;
    case SDL_SCANCODE_LANG2:
        todo;
    case SDL_SCANCODE_LANG3:
        todo;
    case SDL_SCANCODE_LANG4:
        todo;
    case SDL_SCANCODE_LANG5:
        todo;
    case SDL_SCANCODE_LANG6:
        todo;
    case SDL_SCANCODE_LANG7:
        todo;
    case SDL_SCANCODE_LANG8:
        todo;
    case SDL_SCANCODE_LANG9:
        todo;
    case SDL_SCANCODE_ALTERASE:
        todo;
    case SDL_SCANCODE_SYSREQ:
        todo;
    case SDL_SCANCODE_CANCEL:
        todo;
    case SDL_SCANCODE_CLEAR:
        todo;
    case SDL_SCANCODE_PRIOR:
        todo;
    case SDL_SCANCODE_RETURN2:
        todo;
    case SDL_SCANCODE_SEPARATOR:
        todo;
    case SDL_SCANCODE_OUT:
        todo;
    case SDL_SCANCODE_OPER:
        todo;
    case SDL_SCANCODE_CLEARAGAIN:
        todo;
    case SDL_SCANCODE_CRSEL:
        todo;
    case SDL_SCANCODE_EXSEL:
        todo;
    case SDL_SCANCODE_KP_00:
        todo;
    case SDL_SCANCODE_KP_000:
        todo;
    case SDL_SCANCODE_THOUSANDSSEPARATOR:
        todo;
    case SDL_SCANCODE_DECIMALSEPARATOR:
        todo;
    case SDL_SCANCODE_CURRENCYUNIT:
        todo;
    case SDL_SCANCODE_CURRENCYSUBUNIT:
        todo;
    case SDL_SCANCODE_KP_LEFTPAREN:
        todo;
    case SDL_SCANCODE_KP_RIGHTPAREN:
        todo;
    case SDL_SCANCODE_KP_LEFTBRACE:
        todo;
    case SDL_SCANCODE_KP_RIGHTBRACE:
        todo;
    case SDL_SCANCODE_KP_TAB:
        todo;
    case SDL_SCANCODE_KP_BACKSPACE:
        todo;
    case SDL_SCANCODE_KP_A:
        todo;
    case SDL_SCANCODE_KP_B:
        todo;
    case SDL_SCANCODE_KP_C:
        todo;
    case SDL_SCANCODE_KP_D:
        todo;
    case SDL_SCANCODE_KP_E:
        todo;
    case SDL_SCANCODE_KP_F:
        todo;
    case SDL_SCANCODE_KP_XOR:
        todo;
    case SDL_SCANCODE_KP_POWER:
        todo;
    case SDL_SCANCODE_KP_PERCENT:
        todo;
    case SDL_SCANCODE_KP_LESS:
        todo;
    case SDL_SCANCODE_KP_GREATER:
        todo;
    case SDL_SCANCODE_KP_AMPERSAND:
        todo;
    case SDL_SCANCODE_KP_DBLAMPERSAND:
        todo;
    case SDL_SCANCODE_KP_VERTICALBAR:
        todo;
    case SDL_SCANCODE_KP_DBLVERTICALBAR:
        todo;
    case SDL_SCANCODE_KP_COLON:
        todo;
    case SDL_SCANCODE_KP_HASH:
        todo;
    case SDL_SCANCODE_KP_SPACE:
        todo;
    case SDL_SCANCODE_KP_AT:
        todo;
    case SDL_SCANCODE_KP_EXCLAM:
        todo;
    case SDL_SCANCODE_KP_MEMSTORE:
        todo;
    case SDL_SCANCODE_KP_MEMRECALL:
        todo;
    case SDL_SCANCODE_KP_MEMCLEAR:
        todo;
    case SDL_SCANCODE_KP_MEMADD:
        todo;
    case SDL_SCANCODE_KP_MEMSUBTRACT:
        todo;
    case SDL_SCANCODE_KP_MEMMULTIPLY:
        todo;
    case SDL_SCANCODE_KP_MEMDIVIDE:
        todo;
    case SDL_SCANCODE_KP_PLUSMINUS:
        todo;
    case SDL_SCANCODE_KP_CLEAR:
        todo;
    case SDL_SCANCODE_KP_CLEARENTRY:
        todo;
    case SDL_SCANCODE_KP_BINARY:
        todo;
    case SDL_SCANCODE_KP_OCTAL:
        todo;
    case SDL_SCANCODE_KP_DECIMAL:
        todo;
    case SDL_SCANCODE_KP_HEXADECIMAL:
        todo;
    case SDL_SCANCODE_LCTRL:
        todo;
    case SDL_SCANCODE_LSHIFT:
        todo;
    case SDL_SCANCODE_LALT:
        todo;
    case SDL_SCANCODE_LGUI:
        todo;
    case SDL_SCANCODE_RCTRL:
        todo;
    case SDL_SCANCODE_RSHIFT:
        todo;
    case SDL_SCANCODE_RALT:
        todo;
    case SDL_SCANCODE_RGUI:
        todo;
    case SDL_SCANCODE_MODE:
        todo;
    case SDL_SCANCODE_AUDIONEXT:
        todo;
    case SDL_SCANCODE_AUDIOPREV:
        todo;
    case SDL_SCANCODE_AUDIOSTOP:
        todo;
    case SDL_SCANCODE_AUDIOPLAY:
        todo;
    case SDL_SCANCODE_AUDIOMUTE:
        todo;
    case SDL_SCANCODE_MEDIASELECT:
        todo;
    case SDL_SCANCODE_WWW:
        todo;
    case SDL_SCANCODE_MAIL:
        todo;
    case SDL_SCANCODE_CALCULATOR:
        todo;
    case SDL_SCANCODE_COMPUTER:
        todo;
    case SDL_SCANCODE_AC_SEARCH:
        todo;
    case SDL_SCANCODE_AC_HOME:
        todo;
    case SDL_SCANCODE_AC_BACK:
        todo;
    case SDL_SCANCODE_AC_FORWARD:
        todo;
    case SDL_SCANCODE_AC_STOP:
        todo;
    case SDL_SCANCODE_AC_REFRESH:
        todo;
    case SDL_SCANCODE_AC_BOOKMARKS:
        todo;
    case SDL_SCANCODE_BRIGHTNESSDOWN:
        todo;
    case SDL_SCANCODE_BRIGHTNESSUP:
        todo;
    case SDL_SCANCODE_DISPLAYSWITCH:
        todo;
    case SDL_SCANCODE_KBDILLUMTOGGLE:
        todo;
    case SDL_SCANCODE_KBDILLUMDOWN:
        todo;
    case SDL_SCANCODE_KBDILLUMUP:
        todo;
    case SDL_SCANCODE_EJECT:
        todo;
    case SDL_SCANCODE_SLEEP:
        todo;
    case SDL_SCANCODE_APP1:
        todo;
    case SDL_SCANCODE_APP2:
        todo;
    case SDL_SCANCODE_AUDIOREWIND:
        todo;
    case SDL_SCANCODE_AUDIOFASTFORWARD:
        todo;
    case SDL_SCANCODE_SOFTLEFT:
        todo;
    case SDL_SCANCODE_SOFTRIGHT:
        todo;
    case SDL_SCANCODE_CALL:
        todo;
    case SDL_SCANCODE_ENDCALL:
        todo;
    case SDL_NUM_SCANCODES:
        todo;
    }
    return e172::ScancodeUnknown;
}
} // namespace

std::optional<e172::Event> SDLEventProvider::pullEvent()
{
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            return e172::Event::keyDown(scancodeFromSDL(event.key.keysym.scancode));
        } else if (event.type == SDL_KEYUP) {
            return e172::Event::keyUp(scancodeFromSDL(event.key.keysym.scancode));
        } else if (event.type == SDL_MOUSEMOTION) {
            return e172::Event::mouseMotion(e172::Event::Pos(event.motion.x, event.motion.y));
        } else if (event.type == SDL_QUIT) {
            return e172::Event::quit();
        }
    }
    return std::nullopt;
}
