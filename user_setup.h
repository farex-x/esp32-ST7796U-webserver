// ##################################################################################
//
// Section 1. Driver and Display Settings
//
// ##################################################################################

// USE ST7796 DRIVER:
#define ST7796_DRIVER

// Display dimensions for your 3.5" 480x320 display, change dimensions if your ST7796U is bigger/smaller in resolution


//LANDSCAPE (use if esp is in landscape)
#define TFT_WIDTH  320
#define TFT_HEIGHT 480

//PORTRAIT (use if device is in portrait)
//#define TFT_WIDTH  480
//#define TFT_HEIGHT 320



// Color settings for ST7796:
#define TFT_RGB_ORDER TFT_BGR  // Most ST7796 use BGR order
#define TFT_INVERSION_ON

// Backlight control:
#define TFT_BACKLIGHT_ON HIGH

// ##################################################################################
//
// Section 2. Pin Definitions for ST7796
//
// ##################################################################################

// Keep your existing pins - they should work:
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15   // Chip select
#define TFT_DC   2    // Data/Command
#define TFT_RST  -1   // Set to -1 if using display's reset circuit, 

// Backlight control:
#define TFT_BL   27

// Touch screen:
#define TOUCH_CS 33

// SPI frequency - ST7796 can handle up to 27MHz:
#define SPI_FREQUENCY 26000000  // 26mhz, fast enough if 27mhz is too much
// #define SPI_FREQUENCY  20000000  //40000000 If 26MHz has issues

// Use HSPI port:
#define USE_HSPI_PORT

// ##################################################################################
//
// Section 3. Define the fonts that are to be used here
//
// ##################################################################################

// MAKE SURE THESE ARE UNCOMMENTED (no // in front):
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font
#define LOAD_FONT2  // Font 2. Small 16 pixel high font
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font
#define LOAD_FONT6  // Font 6. Large 48 pixel font
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font
#define LOAD_FONT8  // Font 8. Large 75 pixel font

#define LOAD_GFXFF  // FreeFonts

// Also make sure this is uncommented:
#define SMOOTH_FONT
