#ifndef _CHUCKDUINO_H_
#define _CHUCKDUINO_H_

#include <Arduino.h>
#include <Arduino_OV767X.h>

class Chuckduino {
public:
    enum HeadRotation {
        Left,
        Right,
        None
    };

    enum Catchphrase {
        Bazinga
    };

    struct Precepts {
        uint16_t width;
        uint16_t height;
        uint16_t depth;
        uint8_t*** frame;
    };

    struct Actions {
        Chuckduino::HeadRotation headRotation;
        Chuckduino::Catchphrase catchphrase;
    };

public:
    static inline void setup() {
        Serial.begin(9600);
        while (!Serial);

        // configure camera
        if (!Camera.begin(QQVGA, GRAYSCALE, 1)) {
            Serial.println("Failed to initialize camera!");
            while (1);
        }
    }

    static inline Chuckduino::Precepts getPrecepts() {
        int bytesPerFrame = Camera.width() * Camera.height();
        byte* data = new byte[bytesPerFrame];
        Camera.readFrame(data);
        Serial.write(data, bytesPerFrame);

        // TODO: convert raw pixel data into formatted 'Chuckduino::Precepts' frame
    }

    static inline Chuckduino::Actions deliberate(Chuckduino::Precepts precepts) {
        // TODO: convert abstract agent precepts (sensor outputs) into abstract agent actions (actuator inputs)

        Chuckduino::HeadRotation headRotation = Chuckduino::HeadRotation::Left;
        Chuckduino::Catchphrase catchphrase = Chuckduino::Catchphrase::Bazinga;
        Chuckduino::Actions actions{headRotation,catchphrase};
        return actions;
    }

    static inline void takeActions(Chuckduino::Actions actions) {
        // TODO: convert abstract agent actions into actuator inputs
    }
};

#endif /* _CHUCKDUINO_H_ */
