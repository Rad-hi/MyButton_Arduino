/*
 * MyCountingButton Library example: Normal counting
 * -----------------------------------------------------------------------------
 * This example is meant to demonstrate the counting functionality 
 * of MyCountingButton (sub-lib of MyButton library)
 * -----------------------------------------------------------------------------
 * What's the example about?
 * A button wired between pins 12 and GND, and we preiodically print the count.
 * -----------------------------------------------------------------------------
 * Author: Radhi SGHAIER: https://github.com/Rad-hi
 * -----------------------------------------------------------------------------
 * Date: 23-01-2022 (23rd of January, 2022)
 * -----------------------------------------------------------------------------
 * License: Do whatever you want with the code ...
 *          If this was ever useful to you, and we happened to meet on 
 *          the street, I'll appreciate a cup of dark coffee, no sugar please.
 * -----------------------------------------------------------------------------
 */

#include <MyCountingButton.h>

#define BTN_PIN                       12    // Hardware interruption enabled pin (Arduino UNO)
#define PRINT_FREQ                    2000U // Print each two seconds
#define TRIGGER_COUNT                 10

MyCountingButton counting_btn;

/* Callback function for triggerOnCount() */
void callback();


void setup() {
  Serial.begin(115200);
  
  counting_btn.setupTriggerOnCount(TRIGGER_COUNT, callback);

  counting_btn.begin(BTN_PIN, NORMAL_UP); // Using default direction (ASCENDING) and default debounce time (5ms)

//  counting_btn.setDirection(DESCENDING); // We can change the counting direction
  counting_btn.setCountingProfile(ON_CHANGE); // Set on which edge we trigger --> both FALLING and RISING
}

void loop() {
  static unsigned long previous_print;
  
  counting_btn.loopCounter(); // This must be present in the loop in order for the button to catch the pressing events

  if(millis() - previous_print > PRINT_FREQ){
    Serial.println(counting_btn.getCount());
    previous_print = millis();
  }
}

void callback(){
  Serial.print(TRIGGER_COUNT); Serial.println(" ticks are reached!");
  counting_btn.resetCount();
}
