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
        unsigned int bufferLength;
        byte* leftBuffer;
        byte* rightBuffer;
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

        // TODO: convert raw pixel data into formatted 'Chuckduino::Precepts' observation
    }

    static inline Chuckduino::Actions deliberate(Chuckduino::Precepts precepts) {
        static Chuckduino::Precepts _prevPrecepts; // TODO: need to initialize and update
        auto euclidianDistance = [&](byte* p, byte* q) { // L2 norm
            int sum = 0;
            for(unsigned int i = 0; i < precepts.bufferLength; i++)
                sum = sq(q[i] - p[i]);
            return sqrt(sum);
        };

        const double distanceThreshold = 0; // adjustable parameter
        double leftDistance = euclidianDistance(_prevPrecepts.leftBuffer, precepts.rightBuffer);
        double rightDistance = euclidianDistance(_prevPrecepts.rightBuffer, precepts.rightBuffer);

        Chuckduino::HeadRotation headRotation =
                leftDistance >= rightDistance && leftDistance > distanceThreshold ? Chuckduino::HeadRotation::Left :
                rightDistance > distanceThreshold ? Chuckduino::HeadRotation::Right : Chuckduino::HeadRotation::None;
        Chuckduino::Catchphrase catchphrase = Chuckduino::Catchphrase::Bazinga; // TODO: rng catchphrase?
        Chuckduino::Actions actions{headRotation,catchphrase};
        return actions;
    }

    static inline void takeActions(Chuckduino::Actions actions) {
        // TODO: convert abstract agent actions into actuator inputs
    }
};

#endif /* _CHUCKDUINO_H_ */
