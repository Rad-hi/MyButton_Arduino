/*
 * This example is a showcase for the stepped button-read in the MyButton library
 * using the readInSteps() function. WITH MYLTIPLE BUTTONS AND ANIMATIONS.
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

#define LEFT_TO_RIGHT_BUTTON_PIN      32
#define UP_TO_DOWN_BUTTON_PIN         33
#define DIAGONAL_BUTTON_PIN           25
#define RANDOM_BUTTON_PIN             26

/* Button object */
uint8_t debounce_time = 10;
MyButton left_to_right_btn(LEFT_TO_RIGHT_BUTTON_PIN, NORMAL_UP, debounce_time);
MyButton up_to_down_btn(UP_TO_DOWN_BUTTON_PIN, NORMAL_UP, debounce_time);
MyButton diagonal_btn(DIAGONAL_BUTTON_PIN, NORMAL_UP, debounce_time);
MyButton random_btn(RANDOM_BUTTON_PIN, NORMAL_UP, debounce_time);

/*---------------------------------------------------------------------------------------*/

/* Display object; Created the wrapper just for clarity purposes */
TwoWire i2c_display_port = TwoWire(0); /* SDA 21, SCL 22 (On the ESP32)*/
MySSD1306 my_display(&i2c_display_port);

/*---------------------------------------------------------------------------------------*/

/* Message to be displayed when button is clicked for more than the specified period (in our case, 6s) */
static const char msg[] PROGMEM = "MyButton Library, \nreadInSteps() demo";

/* Durations of the animations */
const uint32_t fast_duration = 1000;
const uint32_t slow_duration = 2000;

/* Number of steps */
const uint8_t num_steps = 10;
const uint8_t up_to_down_num_steps = 8;

/* Button animation locations, the PAIR_t type is simply a pair holder (consult printables.h) */
PAIR_t left_to_right_btn_locs[num_steps];
PAIR_t up_to_down_btn_locs[up_to_down_num_steps];
PAIR_t diagonal_btn_locs[num_steps];
PAIR_t random_btn_locs[num_steps];

/* Animation function prototype */
void animate_btn(uint8_t step, uint8_t num_steps, PAIR_t * btn_locs);

/*---------------------------------------------------------------------------------------*/

void setup() {  
  /* Initialize the screen */
  my_display.initial_display();

  /* Generate locations for the animations with the same number of steps */
  for(uint8_t i = 0; i < num_steps; i++){
    
    /* LEFT TO RIGHT :: Generate locations for the animations to follow */
    left_to_right_btn_locs[i] = {
                                  .x = LEFT_MOST_POS + i * (BUTTON_BMP_WIDTH + CLEARANCE),
                                  .y = (I2C_SCREEN_HEIGHT - BUTTON_BMP_HEIGHT)/ 2
                                };

    /* DIAGONAL :: Generate locations for the animations to follow */
    diagonal_btn_locs[i] = {
                             .x = LEFT_MOST_POS + i * (BUTTON_BMP_WIDTH + CLEARANCE),
                             .y = UP_MOST_POS + i * (BUTTON_BMP_HEIGHT + CLEARANCE) / 2
                           };

    /* RANDOM :: Generate locations for the animations to follow */
    random_btn_locs[i] = {
                           .x = random(LEFT_MOST_POS, I2C_SCREEN_WIDTH - BUTTON_BMP_WIDTH),
                           .y = random(UP_MOST_POS, I2C_SCREEN_HEIGHT - BUTTON_BMP_HEIGHT)
                         };
  }

  /* Generate locations for the animations with the same number of steps */
  for(uint8_t i = 0; i < up_to_down_num_steps; i++){
    
    /* UP TO DOWN :: Generate locations for the animations to follow */
    up_to_down_btn_locs[i] = {
                               .x = (I2C_SCREEN_WIDTH - BUTTON_BMP_WIDTH)/ 2,
                               .y = UP_MOST_POS + i * (BUTTON_BMP_HEIGHT + CLEARANCE)
                             };

  }
}

/*---------------------------------------------------------------------------------------*/

void loop() {

  /* LEFT TO RIGHT :: Get the timed step */
  uint8_t left_to_right_step = left_to_right_btn.readInSteps(slow_duration, num_steps);
  animate_btn(left_to_right_step, num_steps, left_to_right_btn_locs);
  

  /* UP TO DOWN :: Get the timed step */
  uint8_t up_to_down_step = up_to_down_btn.readInSteps(fast_duration, num_steps);
  animate_btn(up_to_down_step, up_to_down_num_steps, up_to_down_btn_locs);
  

  /* DIAGONAL :: Get the timed step */
  uint8_t diagonal_step = diagonal_btn.readInSteps(fast_duration, num_steps);
  animate_btn(diagonal_step, num_steps, diagonal_btn_locs);
  

  /* RANDOM :: Get the timed step */
  uint8_t random_step = random_btn.readInSteps(fast_duration, num_steps);
  animate_btn(random_step, num_steps, random_btn_locs);

}

/*---------------------------------------------------------------------------------------*/

/* Animation function */
void animate_btn(uint8_t step, uint8_t n_steps, PAIR_t * btn_locs){
  /* If in the middle of an animation, we visualize */
  if(step < n_steps - 1)my_display.update_button(step, btn_locs, false);
  
  /* In case we stop clicking in the middle of the animation OR we finish our steps, we erase */
  else if(step == ABORTED_STEPS || (step == n_steps - 1))my_display.update_button(step, btn_locs, true);

  /* Apply modifications to the screen */
  my_display.commit_update();
}
