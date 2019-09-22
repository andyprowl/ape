#pragma once

enum class Key
{

    keySpace = 32,
    
    keyApostrophe = 39, /* ' */
    
    keyComma = 44, /* , */
    
    keyMinus = 45, /* - */
    
    keyPeriod = 46, /* . */
    
    keySlash = 47, /* / */
    
    key0 = 48,
    
    key1 = 49,
    
    key2 = 50,
    
    key3 = 51,
    
    key4 = 52,
    
    key5 = 53,
    
    key6 = 54,
    
    key7 = 55,
    
    key8 = 56,
    
    key9 = 57,
    
    keySemicolon = 59, /* ; */
    
    keyEqual = 61, /* = */
    
    keyA = 65,
    
    keyB = 66,
    
    keyC = 67,
    
    keyD = 68,
    
    keyE = 69,
    
    keyF = 70,
    
    keyG = 71,
    
    keyH = 72,
    
    keyI = 73,
    
    keyJ = 74,
    
    keyK = 75,
    
    keyL = 76,
    
    keyM = 77,
    
    keyN = 78,
    
    keyO = 79,
    
    keyP = 80,
    
    keyQ = 81,
    
    keyR = 82,
    
    keyS = 83,
    
    keyT = 84,
    
    keyU = 85,
    
    keyV = 86,
    
    keyW = 87,
    
    keyX = 88,
    
    keyY = 89,
    
    keyZ = 90,
    
    keyLeftBracket = 91, /* [ */
    
    keyBackslash = 92, /* \ */
    
    keyRightBracket = 93, /* ] */
    
    keyGraveAccent = 96,
    
    keyEscape = 256,
    
    keyEnter = 257,

    keyTab = 258,

    keyBackspace = 259,

    keyInsert = 260,

    keyDelete = 261,

    keyRight = 262,

    keyLeft = 263,

    keyDown = 264,

    keyUp = 265,

    keyPageUp = 266,

    keyPageDown = 267,

    keyHome = 268,

    keyEnd = 269,

    keyCapsLock = 280,

    keyScrollLock = 281,

    keyNumLock = 282,

    keyPrintScreen = 283,

    keyPause = 284,

    keyF1 = 290,

    keyF2 = 291,

    keyF3 = 292,

    keyF4 = 293,

    keyF5 = 294,

    keyF6 = 295,

    keyF7 = 296,

    keyF8 = 297,

    keyF9 = 298,

    keyF10 = 299,

    keyF11 = 300,

    keyF12 = 301,

    keyF13 = 302,

    keyF14 = 303,

    keyF15 = 304,

    keyF16 = 305,

    keyF17 = 306,

    keyF18 = 307,

    keyF19 = 308,

    keyF20 = 309,

    keyF21 = 310,

    keyF22 = 311,

    keyF23 = 312,

    keyF24 = 313,

    keyF25 = 314,

    keyKeypad0 = 320,

    keyKeypad1 = 321,

    keyKeypad2 = 322,

    keyKeypad3 = 323,

    keyKeypad4 = 324,

    keyKeypad5 = 325,

    keyKeypad6 = 326,

    keyKeypad7 = 327,

    keyKeypad8 = 328,

    keyKeypad9 = 329,

    keyKeypadDecimal = 330,

    keyKeypadDivide = 331,

    keyKeypadMultiply = 332,

    keyKeypadSubtract = 333,

    keyKeypadAdd = 334,

    keyKeypadEnter = 335,

    keyKeypadEqual = 336,

    keyLeftShift = 340,

    keyLeftControl = 341,

    keyLeftAlt = 342,

    keyLeftSuper = 343,

    keyRightShift = 344,

    keyRightControl = 345,

    keyRightAlt = 346,

    keyRightSuper = 347,

    keyMenu = 348

};

enum class KeyAction
{

    release = 0,

    press = 1,

    repeat = 2

};

enum class KeyModifier
{

    none = 0x0000,

    shift = 0x0001,
    
    control = 0x0002,
    
    alt = 0x0004,
    
    super = 0x0008,
    
    capsLock = 0x0010,
    
    numLock = 0x0020

};
