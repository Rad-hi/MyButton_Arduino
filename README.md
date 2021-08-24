# MyButton Library for Arduino IDE

<img src="images/demo.gif" width=640>

> Demo using the ```readInSteps``` function (check examples for demo)

## A push button debouncing and "enhancing" library for complex user interfaces

This library was made to make it easier to use push buttons, solve the debouncing problems, and allow makers to make more complex user interfaces through enhancing push-buttons with extra complex functionalities.

This library allows for: 

- Detection of pressing events on both the **RISING/FALLING** edges with debouncing.
- Association of multiple behaviors for a single button by creating multiple "checkpoints" for the button to report reaching **upon release**.
- Getting the progress of the pushed button **towards a target period** in a number of **specified steps**.
- Getting the time the button have been clicked for in 3 units (seconds, milliseconds, microseconds).

**All happening in a non-blocking manner.**

## How to use ?

### 1 - Instanciate the button object with the required options:

- Default debouncing time of **5ms**:

	```MyButton my_button(MY_BUTTON_PIN, NORMAL_UP);``` or ```MyButton my_button(MY_BUTTON_PIN, NORMAL_DOWN);```

- Custom debouncing time:
	
	```MyButton my_button(MY_BUTTON_PIN, NORMAL_UP, 25);```

The ```NORMAL_UP```, and ```NORMAL_DOWN``` keywords refer to whether the push button is normally UP or DOWN (pulled UP or DOWN).

### 2 - Then call a function following the examples, though:

One thing to take into consideration, is that for saving on resources, and since there's no apparent use-case where someone would configure one button to exert more than one of the behaviors possible through the functions, the time tracking and the state variables are **shared** between functions, so calling _ as an example _ ```readRisingClick()```, and ```readFallingClick``` back to back in the same loop would make the code behave unpredictibly.

## How to install ?

### 1 - Through the Arduino Library manager:

Go to **tools -> Manage Libraries... -> Type MyButton** and as shown in the picture below, you'll find the MyButton library.

<img src="images/install.png" width=640>

### 2 - Direct download/import: 

<img src="images/download.png" width=640>

Click on **Download ZIP** to download the library, place the unzipped folder into your libraries folder in your **arduinosketchfolder/libraries/PUT_HERE**(on Windows, this is likely to be under **Documents/Arduino/libraries**, on linux this is under **home/Arduino/libraries**). You may need to create the libraries subfolder if its your first library. Now, restart the IDE.

## Change LOG

### v1.0.1 (22nd August, 2021):

The ```readRisingClick()``` was repeatedly reporting a "rising edge" detection as long as the button was pressed, so fixed that, and now it's reporting only the detection of the rising edge, as it's supposed to do.

### v1.0.2 (24th August, 2021):

- Added debouncing to ```readInSteps()``` and made it return a **ABORTED_STEPS** value when the user stops pressing in the middle of the pressing period without reaching the intended stepped-period.
- Added multiple buttons driven animations example.
- Updated the Display API (wrapper class to the Adafruit_SSD1306 library) to make it more efficient and more intuitive, no change in how it works though.
