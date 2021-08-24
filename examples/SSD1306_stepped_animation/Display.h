#ifndef __MY_DISPLAY_H__
  #define __MY_DISPLAY_H__

  /* Dependencies */
  #include "printables.h"
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>

  /* Display address */
  #define I2C_SSD1306_ADDR       0x3C
  
  /* Custom display class to add the needed functionalities */
  class MySSD1306{
    
    private:

      /* The display object that'll be used to do everything */
      Adafruit_SSD1306 display;
      
      Adafruit_SSD1306 create_display(TwoWire * wire);
      
      
    public:

      /* Constructor */ 
      MySSD1306(TwoWire * wire);

      /* Draw initial screen with random values */
      void initial_display();

      /* This function shall update the buffer without making the change */
      void update_button(uint8_t step, PAIR_t * coordinates, bool erase);
      void update_msg(const char * msg);

      /* This function commits the changes needed in the display */
      void commit_update();
  };
  
#endif // __MY_DISPLAY_H__
