//#ifndef CONSTANTS_H
//#define CONSTANTS_H

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
#define UI_FONT_FACE "verdana.ttf"

#define UI_COLOR 255

// faders
#define FADER_RANGE 250

// buttons
#define BUTTON_COLOR 0x8DCACB

// touches
#define TOUCH_LINE_WIDTH 3
#define TOUCH_RADIUS 30
#define TOUCH_COLOR 128
#define TOUCH_MAX 20

// particle
#define PARTICLE_COLOR 0
#define PARTICLE_LIFE_COLOR 128
#define PARTICLE_LIMIT_COLOR 0xFF3300
#define PARTICLE_WIDTH 2

// console
#define CONSOLE_COLOR 0xA1DEDF
#define CONSOLE_BORDER_WIDTH 1
#define CONSOLE_BORDER_COLOR 0x717171

// grid
#define GRID_COLOR 213
#define GRID_HEIGHT 20

// poly
#define POLY_WIDTH 1
#define POLY_COLOR 128

#define LIMIT_PARTICLE 0.2 // percentage of the screen height below which it is not possible to add particles

// inactivity
#define INACTIVITY_TOUCH_MAX TOUCH_MAX
#define INACTIVITY_TOUCH_MIN TOUCH_MAX/5
#define INACTIVITY_THRESHOLD 10

//#endif