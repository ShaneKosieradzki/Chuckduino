#include <Chuckduino.h>

void setup() {
    Chuckduino::setup();
}

void loop() {
    Chuckduino::Precepts precepts = Chuckduino::getPrecepts();
    Chuckduino::Actions actions = Chuckduino::deliberate(precepts);
    Chuckduino::takeActions(actions);
}