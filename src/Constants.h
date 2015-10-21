//#define ACER // if active, will optimize for ACER screen, otherwise will work best in Macbook

#ifdef ACER
    #define FADER_SIZE 24
    #define BUTTON_SIZE 36
    #define CONSOLE_HEIGHT 200
    #define UI_FONT_SIZE 14
#else
    #define FADER_SIZE 20
    #define BUTTON_SIZE 20
    #define CONSOLE_HEIGHT 120
    #define UI_FONT_SIZE 14
#endif

// setup
#define FRAME_RATE 60
#define BACKGROUND_COLOR 255
#define CIRCLE_RESOLUTION 50
#define ARC_RESOLUTION 30

// network
#define HOST "localhost"
#define RECEIVE_PORT 7000
#define SEND_PORT 5000
#define OSC_ADDRESS "/GML/"

// app
#define MODULES 4
#define PARTICLES_PER_MODULE 10


// user interface
//#define UI_FONT_FACE "Raleway-Semibold.ttf"
#define UI_FONT_FACE "Raleway-Medium.ttf"
#define UI_COLOR 255 // color for buttons, faders, etc.

// faders
#define FADER_RANGE 250 // in pixels

// buttons
#define BUTTON_COLOR 0x8DCACB
#define BUTTON_REMOVE_COLOR 0x274848
#define BUTTON_WIDTH 1

// touches
#define TOUCH_COLOR 0x808080
#define TOUCH_ALPHA 150
#define TOUCH_MAX 20 // radius and in pixels

// particle
#define PARTICLE_COLOR_INNER 0xA1DEDF
#define PARTICLE_COLOR_OUTER 0
#define PARTICLE_LIMIT_COLOR 0xFF3300
#define PARTICLE_LIMIT_ALPHA TOUCH_ALPHA
#define PARTICLE_OFFSET 1.05

// console
#define CONSOLE_COLOR 0xA1DEDF
#define CONSOLE_BORDER_WIDTH 1
#define CONSOLE_BORDER_COLOR 0x717171

// grid
#define GRID_COLOR 213

// poly
#define POLY_WIDTH 1
#define POLY_COLOR 128

#define LIMIT_PARTICLE 0.2 // percentage of the screen height below which it is not possible to add particles

// inactivity
#define INACTIVITY_TOUCH_MAX TOUCH_MAX
#define INACTIVITY_TOUCH_MIN TOUCH_MAX/5
#define INACTIVITY_THRESHOLD 10