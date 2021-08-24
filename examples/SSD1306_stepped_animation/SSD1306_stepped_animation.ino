/*
 * This example is a showcase for the stepped button-read in the MyButton library
 * using the readInSteps() function.
 * ----------------------------------------------------------------------------
 * To better showcase the usefulness of such function, the SSD1306 display
 * is used to animate a progress bar of a push-button.
 * ----------------------------------------------------------------------------
 * A wrapper class for the Adafruit_SSD1306 library was created for clarity purposes
 * ----------------------------------------------------------------------------
 * Author: Radhi SGHAIER: https://github.com/Rad-hi
 * -----------------------------------------------------------------------------
 * Date: 24-08-2021 (24th of August, 2021)
 * -----------------------------------------------------------------------------
 * License: Do whatever you want with the code ...
 *          If this was ever useful to you, and we happened to meet on 
 *          the street, I'll appreciate a cup of dark coffee, no sugar please.
 * -----------------------------------------------------------------------------
 */

#include "Display.h"
#include <MyButton.h>

/*---------------------------------------------------------------------------------------*/

#define MY_BUTTON_PIN             32

/* Button object */
MyButton my_btn(MY_BUTTON_PIN, NORMAL_UP);

/*---------------------------------------------------------------------------------------*/

/* Display object; Created the wrapper just for clarity purposes */
TwoWire i2c_display_port = TwoWire(0); /* SDA 21, SCL 22 (On an ESP32)*/
MySSD1306 my_display(&i2c_display_port);

/*---------------------------------------------------------------------------------------*/

/* Message to be displayed when button is clicked for more than the specified period (in our case, 6s) */
static const char msg[] PROGMEM = "MyButton Library, \nreadInSteps() demo";

/* Duration and number of steps in the animation */
const uint32_t duration = 2000;
const uint8_t num_steps = 10;

/* Button animation locations, the PAIR_t type is simply a pair holder (consult printables.h) */
PAIR_t btn_locs[num_steps];

/*---------------------------------------------------------------------------------------*/

void setup() {  
  /* Initialize the screen */
  my_display.initial_display();
  
  /* Generate locations for the animations to follow */
  for(uint8_t i = 0; i < num_steps; i++){
    btn_locs[i] = {
                    .x = LEFT_MOST_POS + i * (BUTTON_BMP_WIDTH + CLEARANCE),
                    .y = (I2C_SCREEN_HEIGHT - BUTTON_BMP_HEIGHT)/ 2
                  };
  }
}

/*---------------------------------------------------------------------------------------*/

void loop() {

  /* Get the timed step */
  uint8_t step = my_btn.readInSteps(duration, num_steps);

  /* If in the middle of an animation, we visualize */
  if(step < num_steps){

    /* Visulaize the next step of the animation */
    my_display.update_button(step, btn_locs, false);

    /* Reached end of sequence */
    if(step == num_steps - 1)my_display.update_msg(msg); /* Show msg */
  }

  /* In case we stop clicking in the middle of the animation, we erase */
  else if(step == ABORTED_STEPS)my_display.update_button(step, btn_locs, true);

  /* Apply modifications to the screen */
  my_display.commit_update();
}
