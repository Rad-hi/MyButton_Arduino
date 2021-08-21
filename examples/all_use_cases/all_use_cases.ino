/*
 * Examples on how to use all functions of the MyButton library
 * ------------------------------------------------------------------
 * Uncomment any example you want to use.
 * In case you choose to do a NORMAL_DOWN button, make sure to externally 
 * pull down the push-button, otherwise, an internal pull-up resistor is
 * used by default to the NORMAL_UP mode.
 */

#include <MyButton.h>

#define MY_BUTTON_PIN         32

/* Instanciate the button object */
MyButton my_btn(MY_BUTTON_PIN, NORMAL_UP);

/* Periods [milliseconds] for the multiple behaviors example, MUST BE IN INCREMENTAL ORDER */
uint32_t periods[] = {5, 1000, 2000, 4000};

/* Printing buffer */
char buf[255];

/* To decrease the printing frequency of the IN_LOOP proof that it's non blocking */
unsigned long last_print;

/* Steps' indexes for the steps example */
static const char steps_index PROGMEM [9][8] = {"first", "second", "third", "fourth", "fifth", "sixth", "seventh", "eighth", "nineth"};


void setup() {
  Serial.begin(9600);
  Serial.println("--- Click me! ---");
  last_print = millis();
}

void loop() {

  /*------------------------------------------------------------------------------*/
  /* ON CLICK (RISING EDGE) EXAMPLE                                               */
  /*------------------------------------------------------------------------------*/
  
  /* On the click moment, a boolean true is returned */
  if(my_btn.readRisingClick()){
    Serial.println("RISING :: Button clicked!");
  }

  /*------------------------------------------------------------------------------*/
  /* ON RELEASE (FALLING EDGE) EXAMPLE                                            */
  /*------------------------------------------------------------------------------*/
  
//  /* Once a button is released (after being clicked), a boolean true is returned */
//  if(my_btn.readFallingClick()){
//    Serial.println("FALLING :: Button clicked, and released!");
//  }

  /*------------------------------------------------------------------------------*/
  /* MULTIPLE BEHAVIOURS EXAMPLE                                                  */
  /*------------------------------------------------------------------------------*/
  
//  /* Get which of the periods the button have been clicked for */
//  uint8_t which = my_btn.readMultiple(periods, sizeof(periods)/sizeof(periods[0]));
//
//  /* If the button was clicked for one of the periods, we'll print which one was it */
//  if(which < sizeof(periods)/sizeof(periods[0]) + 1){
//    sprintf(buf, "Button clicked for: %dms", periods[which]);
//    Serial.println(buf);
//  }

  /*------------------------------------------------------------------------------*/
  /* STEPPED PERIOD EXAMPLE                                                       */
  /*------------------------------------------------------------------------------*/
  
//  /* Get which of the periods the button have been clicked for */
//  uint8_t step = my_btn.readInSteps(3000, 3);
//
//  /* If the button was clicked for one of the periods, we'll print which one was it */
//  if(step < 3){
//    sprintf(buf, "We're in the %s step", steps_index[step]);
//    Serial.println(buf);
//  }

  /*------------------------------------------------------------------------------*/
  /* TIMED READ EXAMPLE                                                           */
  /*------------------------------------------------------------------------------*/

//  uint32_t clicked_for = my_btn.readTimedPress(IN_SECONDS);
//  if(clicked_for > 0)Serial.println(clicked_for);

  /*------------------------------------------------------------------------------*/
  /* NON-BLOCKING PRINTING PROOF                                                  */
  /*------------------------------------------------------------------------------*/

  /* To prove that it's non blocking */
  if(millis() - last_print >= 1000){
    last_print = millis();
    Serial.println("IN LOOP");
  }
}
