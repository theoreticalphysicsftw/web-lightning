// MIT License
// 
// Copyright (c) 2023 Mihail Mladenov
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#pragma once

#include <SDL2/SDL_scancode.h>


namespace WL
{
    enum class EScanCode
    {
        Unknown = SDL_SCANCODE_UNKNOWN,
        A = SDL_SCANCODE_A,
        B = SDL_SCANCODE_B,
        C = SDL_SCANCODE_C,
        D = SDL_SCANCODE_D,
        E = SDL_SCANCODE_E,
        F = SDL_SCANCODE_F,
        G = SDL_SCANCODE_G,
        H = SDL_SCANCODE_H,
        I = SDL_SCANCODE_I,
        J = SDL_SCANCODE_J,
        K = SDL_SCANCODE_K,
        L = SDL_SCANCODE_L,
        M = SDL_SCANCODE_M,
        N = SDL_SCANCODE_N,
        O = SDL_SCANCODE_O,
        P = SDL_SCANCODE_P,
        Q = SDL_SCANCODE_Q,
        R = SDL_SCANCODE_R,
        S = SDL_SCANCODE_S,
        T = SDL_SCANCODE_T,
        U = SDL_SCANCODE_U,
        V = SDL_SCANCODE_V,
        W = SDL_SCANCODE_W,
        X = SDL_SCANCODE_X,
        Y = SDL_SCANCODE_Y,
        Z = SDL_SCANCODE_Z,

        One = SDL_SCANCODE_1,
        Two = SDL_SCANCODE_2,
        Three = SDL_SCANCODE_3,
        Four = SDL_SCANCODE_4,
        Five = SDL_SCANCODE_5,
        Six = SDL_SCANCODE_6,
        Seven = SDL_SCANCODE_7,
        Eight = SDL_SCANCODE_8,
        Nine = SDL_SCANCODE_9,
        Zero = SDL_SCANCODE_0,

        Return = SDL_SCANCODE_RETURN,
        Esc = SDL_SCANCODE_ESCAPE,
        Backspace = SDL_SCANCODE_BACKSPACE,
        Tab = SDL_SCANCODE_TAB,
        Space = SDL_SCANCODE_SPACE,

        Minus = SDL_SCANCODE_MINUS,
        Equals = SDL_SCANCODE_EQUALS,
        LeftBracket = SDL_SCANCODE_LEFTBRACKET,
        RightBracket = SDL_SCANCODE_RIGHTBRACKET,
        Backslash = SDL_SCANCODE_BACKSLASH,
        NonUSHash = SDL_SCANCODE_NONUSHASH, 
        Semicolon = SDL_SCANCODE_SEMICOLON,
        Apostrophe = SDL_SCANCODE_APOSTROPHE,
        Grave = SDL_SCANCODE_GRAVE, 
        Comma = SDL_SCANCODE_COMMA,
        Period = SDL_SCANCODE_PERIOD,
        Slash = SDL_SCANCODE_SLASH,

        CapsLock = SDL_SCANCODE_CAPSLOCK,

        F1 = SDL_SCANCODE_F1,
        F2 = SDL_SCANCODE_F2,
        F3 = SDL_SCANCODE_F3,
        F4 = SDL_SCANCODE_F4,
        F5 = SDL_SCANCODE_F5,
        F6 = SDL_SCANCODE_F6,
        F7 = SDL_SCANCODE_F7,
        F8 = SDL_SCANCODE_F8,
        F9 = SDL_SCANCODE_F9,
        F10 = SDL_SCANCODE_F10,
        F11 = SDL_SCANCODE_F11,
        F12 = SDL_SCANCODE_F12,

        PrintScreen = SDL_SCANCODE_PRINTSCREEN,
        ScrollLock = SDL_SCANCODE_SCROLLLOCK,
        Pause = SDL_SCANCODE_PAUSE,
        Insert = SDL_SCANCODE_INSERT,
        Home = SDL_SCANCODE_HOME,
        PageUp = SDL_SCANCODE_PAGEUP,
        Delete = SDL_SCANCODE_DELETE,
        End = SDL_SCANCODE_END,
        PageDown = SDL_SCANCODE_PAGEDOWN,
        Right = SDL_SCANCODE_RIGHT,
        Left = SDL_SCANCODE_LEFT,
        Down = SDL_SCANCODE_DOWN,
        Up = SDL_SCANCODE_UP,

        NumLockClear = SDL_SCANCODE_NUMLOCKCLEAR,
        KeypadDivide = SDL_SCANCODE_KP_DIVIDE,
        KeypadMultiply = SDL_SCANCODE_KP_MULTIPLY,
        KeypadMinus = SDL_SCANCODE_KP_MINUS,
        KeypadPlus = SDL_SCANCODE_KP_PLUS,
        KeypadEnter = SDL_SCANCODE_KP_ENTER,
        KeypadOne = SDL_SCANCODE_KP_1,
        KeypadTwo = SDL_SCANCODE_KP_2,
        KeypadThree = SDL_SCANCODE_KP_3,
        KeypadFour = SDL_SCANCODE_KP_4,
        KeypadFive = SDL_SCANCODE_KP_5,
        KeypadSix = SDL_SCANCODE_KP_6,
        KeypadSeven = SDL_SCANCODE_KP_7,
        KeypadEight = SDL_SCANCODE_KP_8,
        KeypadNine = SDL_SCANCODE_KP_9,
        KeypadZero = SDL_SCANCODE_KP_0,
        KeypadPeriod = SDL_SCANCODE_KP_PERIOD,

        NonUSBackslash = SDL_SCANCODE_NONUSBACKSLASH, 
        Application = SDL_SCANCODE_APPLICATION, 
        Power = SDL_SCANCODE_POWER,
        KeypadEquals = SDL_SCANCODE_KP_EQUALS,
        F13 = SDL_SCANCODE_F13,
        F14 = SDL_SCANCODE_F14,
        F15 = SDL_SCANCODE_F15,
        F16 = SDL_SCANCODE_F16,
        F17 = SDL_SCANCODE_F17,
        F18 = SDL_SCANCODE_F18,
        F19 = SDL_SCANCODE_F19,
        F20 = SDL_SCANCODE_F20,
        F21 = SDL_SCANCODE_F21,
        F22 = SDL_SCANCODE_F22,
        F23 = SDL_SCANCODE_F23,
        F24 = SDL_SCANCODE_F24,
        Execute =SDL_SCANCODE_EXECUTE,
        Help = SDL_SCANCODE_HELP,    
        Menu = SDL_SCANCODE_MENU,   
        Select = SDL_SCANCODE_SELECT,
        Stop = SDL_SCANCODE_STOP,  
        Again = SDL_SCANCODE_AGAIN, 
        Undo = SDL_SCANCODE_UNDO,  
        Cut = SDL_SCANCODE_CUT,   
        Copy = SDL_SCANCODE_COPY,   
        Paste = SDL_SCANCODE_PASTE,  
        Find = SDL_SCANCODE_FIND,    
        Mute = SDL_SCANCODE_MUTE,
        VolumeUp = SDL_SCANCODE_VOLUMEUP,
        VolumeDown = SDL_SCANCODE_VOLUMEDOWN,

        KeypadComma = SDL_SCANCODE_KP_COMMA,

        LeftControl = SDL_SCANCODE_LCTRL,
        LeftShift = SDL_SCANCODE_LSHIFT,
        LeftAlt = SDL_SCANCODE_LALT,
        LeftGui = SDL_SCANCODE_LGUI,
        RightControl = SDL_SCANCODE_RCTRL,
        RightShift = SDL_SCANCODE_RSHIFT,
        RightAlt = SDL_SCANCODE_RALT, 
        RightGui = SDL_SCANCODE_RGUI,

        SDL_NUM_SCANCODES,
    };
}