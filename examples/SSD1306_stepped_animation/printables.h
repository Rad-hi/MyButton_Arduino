#ifndef __PRINTABLES_DISPLAY_H__
  #define __PRINTABLES_DISPLAY_H__

  #include"Arduino.h"
  
  /* Screen dimentions */
  #define I2C_SCREEN_WIDTH       128
  #define I2C_SCREEN_HEIGHT      64

  /* Message printing styles */
  #define MSG_CHAR_SIZE          1
  #define MSG_CHAR_HEIGHT        (7 * MSG_CHAR_SIZE)
  #define MSG_CHAR_WIDTH         (5 * MSG_CHAR_SIZE)
  #define MSG_Y_POS              ((I2C_SCREEN_HEIGHT - MSG_CHAR_HEIGHT) / 2)
  #define MSG_X_POS              MSG_CHAR_WIDTH
  #define MSG_VIZ_TIME           5000UL /* Milliseconds */

  /* Button pressed animation params */
  #define LEFT_MOST_POS          4 /* Pixels from bottom of the screen */
  #define CLEARANCE              4  /* Pixels between the blocks        */
  #define BUTTON_BMP_HEIGHT      16
  #define BUTTON_BMP_WIDTH       8
  
  
  typedef struct{
    uint8_t x;
    uint8_t y;
  }PAIR_t;

  /* The bitmap to be displayed when a button is preseed, white block of 8x16 pixels */
  static const uint8_t PROGMEM button_bmp[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
  
#endif // __PRINTABLES_DISPLAY_H__
