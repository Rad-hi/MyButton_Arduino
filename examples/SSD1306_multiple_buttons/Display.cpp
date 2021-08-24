#include "Display.h"

/* Init utils */

MySSD1306 :: MySSD1306(TwoWire * wire){
  display = create_display(wire);
}

Adafruit_SSD1306 MySSD1306 :: create_display(TwoWire * wire){
  Adafruit_SSD1306 display(I2C_SCREEN_WIDTH, I2C_SCREEN_HEIGHT, wire, -1);
  return display;
}

/* Init display */
void MySSD1306 :: initial_display(){
  display.begin(SSD1306_SWITCHCAPVCC, I2C_SSD1306_ADDR);
  display.clearDisplay();
}

/* Animation during the button pressing */
void MySSD1306 :: update_button(uint8_t step, PAIR_t * coordinates, bool erase){

  if(erase){
    for(uint8_t i = 0; i < step; i++){
      display.drawBitmap(coordinates[i].x, coordinates[i].y, 
                         button_bmp, 
                         BUTTON_BMP_WIDTH, BUTTON_BMP_HEIGHT, 
                         SSD1306_BLACK); 
    }
  }
  else{
    display.drawBitmap(coordinates[step].x, coordinates[step].y, 
                       button_bmp, 
                       BUTTON_BMP_WIDTH, BUTTON_BMP_HEIGHT, 
                       SSD1306_WHITE);
  }
}

/* Text displaying function */
void MySSD1306 :: update_msg(const char * msg){
  
  display.clearDisplay();
  display.setTextSize(MSG_CHAR_SIZE);             
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(MSG_X_POS, MSG_Y_POS);
  display.print(F(msg));
  
  display.display();

  display.startscrollleft(0x00, 0x0F);  
    
  delay(MSG_VIZ_TIME);
  
  display.stopscroll();
  display.clearDisplay();
}

/* Refresh the screen ; commit changes */
void MySSD1306 :: commit_update(){
  display.display();
}
