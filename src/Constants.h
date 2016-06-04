// #define TARGET_SEMIBREVE

#if defined TARGET_OF_IOS
    #define NMODULES 4
    #define UI_FONT_SIZE 13
    #define TOUCH_MAX 40
    #define TOUCH_MIN 10
    #define BUTTON_SIZE .20
    #define FADER_SIZE .16

 #elif defined TARGET_SEMIBREVE
    #define NMODULES 4
    #define UI_FONT_SIZE 14
    #define TOUCH_MAX 20
    #define TOUCH_MIN 0
    #define BUTTON_SIZE .18
    #define FADER_SIZE .12

#else
    #define NMODULES 4
    #define UI_FONT_SIZE 14
    #define TOUCH_MAX 20
    #define TOUCH_MIN 0
    #define BUTTON_SIZE .18
    #define FADER_SIZE .12
#endif

// setup
#define FRAME_RATE 60
#define BACKGROUND_COLOR 255
#define IMAGE_COLOR 47
#define ARROW_OFFSET 5
#define DEFAULT_ALPHA 150
#define CIRCLE_RESOLUTION 50
#define ARC_RESOLUTION 30

// network
#define HOST "localhost"
#define RECEIVE_PORT 7000
#define SEND_PORT 5000
#define OSC_ADDRESS "/GML/"

// app
#define PARTICLES_PER_MODULE 10

// user interface
#define UI_FONT_FACE "Raleway-Medium.ttf"
#define UI_COLOR 255 // color for buttons, faders, etc.

// buttons + fader
#define BUTTON_COLOR 0x8DCACB
#define BUTTON_REMOVE_COLOR 0x274848
#define BUTTON_WIDTH 1
#define BUTTON_CHANGE_INSTRUMENT_WIDTH .08
#define BUTTON_CHANGE_INSTRUMENT_HEIGHT .14
#define BUTTON_CHANGE_INSTRUMENT_COLOR 0xA1DEDF
#define BUTTON_CHANGE_INSTRUMENT_COLOR_ALPHA 200

// touches
#define TOUCH_COLOR 0x808080
#define TOUCH_ALPHA 150

// particle
#define PARTICLE_COLOR_INNER 0xA1DEDF
#define PARTICLE_COLOR_OUTER 0
#define PARTICLE_LIMIT_COLOR 0xFF3300
#define PARTICLE_LIMIT_ALPHA TOUCH_ALPHA
#define PARTICLE_OFFSET 1.05

// console
#define CONSOLE_HEIGHT 0.18
#define CONSOLE_SECTION_HEIGHT 0.75 // this is, in practice, the height at which the remove and clear buttons start
#define CONSOLE_COLOR 0xA1DEDF
#define CONSOLE_BORDER_WIDTH 1
#define CONSOLE_BORDER_COLOR 0x717171

// grid
#define GRID_COLOR 213

// poly
#define POLY_WIDTH 1
#define POLY_COLOR 128

#define LIMIT_PARTICLE 0.21 // percentage of the screen height below which it is not possible to add particles

// inactivity
#define INACTIVITY_TOUCH_MAX TOUCH_MAX
#define INACTIVITY_TOUCH_MIN TOUCH_MAX/5
#define INACTIVITY_THRESHOLD 120
