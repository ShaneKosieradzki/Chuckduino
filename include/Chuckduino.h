#ifndef _CHUCKDUINO_H_
#define _CHUCKDUINO_H_

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
    static inline Chuckduino::Precepts getPrecepts() {
        int bytesPerFrame = Camera.width() * Camera.height() * Camera.bytesPerPixel();
        byte* data = new byte[bytesPerFrame];
        Camera.readFrame(data);

        // TODO: convert raw pixel data into formatted 'Chuckduino::Precepts' frame
    }

    static inline Chuckduino::Actions deliberate(Chuckduino::Precepts precepts) {
        // TODO: convert abstract agent precepts (sensor outputs) into abstract agent actions (actuator inputs)
    }

    static inline void takeActions(Chuckduino::Actions actions) {
        // TODO: convert abstract agent actions into actuator inputs
    }
};

#endif /* _CHUCKDUINO_H_ */
