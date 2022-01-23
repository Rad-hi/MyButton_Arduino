/*
 * MyCountingButton Library example: Interruption counting
 * -----------------------------------------------------------------------------
 * This example is meant to demonstrate the counting through interruptions 
 * functionality of MyCountingButton (sub-lib of MyButton library)
 * -----------------------------------------------------------------------------
 * What's the example about?
 * We have a non-AB (cheap) encoder wired to VCC, GND, pin 2 (on its D0 pin)
 * and we calculate the RPM of the wheel it's attached to by measuring the time
 * it takes the wheel to finish a whole rotation (since we know the number of
 * expected ticks for each full rotation).
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

#define BTN_PIN                       2     // Hardware interruption enabled pin (Arduino UNO)
#define PRINT_FREQ                    2000U // Print each two seconds
#define MILLISECONDS_IN_MINUTE        (60U * 1000U)
#define TICKS_PER_ROTATION            25    // Number of "holes" in the encoder wheel

MyCountingButton isr_counting_btn;
float rpm;

/* Callback function for triggerOnCount() */
void calculate_rpm();


void setup() {
  Serial.begin(115200);
  
  isr_counting_btn.setupTriggerOnCount(TICKS_PER_ROTATION, calculate_rpm);
  
  isr_counting_btn.beginCountingInterrupter(BTN_PIN, GET_ISR(isr_counting_btn, countingInterruption), ASCENDING, RISING);
}

void loop() {
  static unsigned long previous_print;
  
  isr_counting_btn.loopCounter(); // This must be present in the loop in order for the button to catch the pressing events

  if(millis() - previous_print > PRINT_FREQ){
    Serial.println(rpm);
    rpm = 0.0;
    previous_print = millis();
  }
}

/*
 * This function is supposed to trigger once each 1 full rotation 
 * RPM =  1[rot] / time[minutes]
 * But we have our time in milliseconds
 * --> time[minutes] = time[milliseconds] / MILLISECONDS_IN_MINUTE
 * Thus, RPM = 1 / (time[milliseconds] / MILLISECONDS_IN_MINUTE)
 *           = MILLISECONDS_IN_MINUTE / time[milliseconds]
 */
void calculate_rpm(){
  static unsigned long previous_speed_measure;
  
  rpm = (float)(MILLISECONDS_IN_MINUTE / (millis() - previous_speed_measure));
  previous_speed_measure = millis();
  isr_counting_btn.resetCount();
}
