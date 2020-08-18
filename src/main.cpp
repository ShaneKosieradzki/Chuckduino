#include <Arduino.h>
#include <Arduino_OV767X.h>
#include <Chuckduino.h>

void setup() {
    Serial.begin(9600);
    while (!Serial);

    // configure camera
    if (!Camera.begin(QVGA, RGB565, 1)) {
        Serial.println("Failed to initialize camera!"); // assumes serial port is configured
        while (1);
    }
}

void loop() {
    Chuckduino::Precepts precepts = Chuckduino::getPrecepts();
    Chuckduino::Actions actions = Chuckduino::deliberate(precepts);
    Chuckduino::takeActions(actions);
}