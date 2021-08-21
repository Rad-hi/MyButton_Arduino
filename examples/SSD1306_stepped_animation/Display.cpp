#include "Display.h"

/* Init utils */

MySSD1306 :: MySSD1306(TwoWire * wire){
  display = create_display(wire);
}

Adafruit_SSD1306 MySSD1306 :: create_display(TwoWire * wire){
  Adafruit_SSD1306 display(I2C_SCREEN_WIDTH, I2C_SCREEN_HEIGHT, wire, -1);
  return display;
}

/*  
 * (X0, Y0) +---------------------+
 *          |     AREA TO BE      |
 *          |       CLEARED       |
 *          +---------------------+ (X1, Y1)
 */
void MySSD1306 :: clear_partial(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2){
  display.fillRect(x1, y1, x2, y2, SSD1306_BLACK);
  display.display();
}

/* Init display */
void MySSD1306 :: initial_display(){
  display.begin(SSD1306_SWITCHCAPVCC, I2C_SSD1306_ADDR);
  display.clearDisplay();
}

/* Animation during the button pressing */
void MySSD1306 :: update_button(uint8_t step, uint8_t len, PAIR_t * coordinates){

  if(step == len - 1){
    for(uint8_t i = 0; i < len-1; i++){
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
  
  display.display();
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
