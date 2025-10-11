#ifndef __COLORS_H__
#define __COLORS_H__

//Reset
#define C_RESET      "\x1b[0m"

//Regular Colors
#define C_BLACK      "\x1b[30m"
#define C_RED        "\x1b[31m"
#define C_GREEN      "\x1b[32m"
#define C_YELLOW     "\x1b[33m"
#define C_BLUE       "\x1b[34m"
#define C_MAGENTA    "\x1b[35m"
#define C_CYAN       "\x1b[36m"
#define C_WHITE      "\x1b[37m"

//Bright Colors
#define C_BBLACK     "\x1b[90m"
#define C_BRED       "\x1b[91m"
#define C_BGREEN     "\x1b[92m"
#define C_BYELLOW    "\x1b[93m"
#define C_BBLUE      "\x1b[94m"
#define C_BMAGENTA   "\x1b[95m"
#define C_BCYAN      "\x1b[96m"
#define C_BWHITE     "\x1b[97m"

//Background colors
#define C_BG_BLACK   "\x1b[40m"
#define C_BG_RED     "\x1b[41m"
#define C_BG_GREEN   "\x1b[42m"
#define C_BG_YELLOW  "\x1b[43m"
#define C_BG_BLUE    "\x1b[44m"
#define C_BG_MAGENTA "\x1b[45m"
#define C_BG_CYAN    "\x1b[46m"
#define C_BG_WHITE   "\x1b[47m"

//Bright background
#define C_BG_BBLACK   "\x1b[100m"
#define C_BG_BRED     "\x1b[101m"
#define C_BG_BGREEN   "\x1b[102m"
#define C_BG_BYELLOW  "\x1b[103m"
#define C_BG_BBLUE    "\x1b[104m"
#define C_BG_BMAGENTA "\x1b[105m"
#define C_BG_BCYAN    "\x1b[106m"
#define C_BG_BWHITE   "\x1b[107m"

//Bold versions (can be combined)
#define C_BOLD       "\x1b[1m"

//Italic
#define C_ITALIC  "\x1b[3m"

//Underline
#define C_UNDERLINE  "\x1b[4m"

//Blink
#define C_BLINK  "\x1b[5m"

//Standout
#define C_STANDOUT  "\x1b[7m"

//True color
#define C_RGB_FG(r,g,b)  "\x1b[38;2;" #r ";" #g ";" #b "m"
#define C_RGB_BG(r,g,b)  "\x1b[48;2;" #r ";" #g ";" #b "m"

#endif //COLORS.H