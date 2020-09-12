#ifndef _CHUCKDUINO_H_
#define _CHUCKDUINO_H_

#include <Arduino.h>
#include <Arduino_OV767X.h>

#define BUFFER_LENGTH 19200
#define START Serial.begin

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
        // The number of elements in `leftBuffer` or `rightBuffer` (they need to be the same length)
        byte leftBuffer[BUFFER_LENGTH];
        byte rightBuffer[BUFFER_LENGTH];
    };

    struct Actions {
        Chuckduino::HeadRotation headRotation;
        Chuckduino::Catchphrase catchphrase;
    };

public:
    static inline void setup() {
        START(9600);
        while (!Serial);

        // configure camera
        if (!Camera.begin(QQVGA, GRAYSCALE, 1)) {
            Serial.println("Failed to initialize camera!");
            while (1);
        }
    }

    /* --------------------------------------------------Kate-------------------------------------------------- */

    static inline Chuckduino::Precepts getPrecepts() {

        // The number of bytes in a single camera frame
        int bytesPerFrame = Camera.width() * Camera.height();

        /* A buffer (aka C-Style list) of bytes that represent a single camera frame
         * where each byte represents the grayscale-color of a single pixel.
         */
        byte* data = (byte*) malloc(sizeof(byte)*bytesPerFrame);

        // `readFrame` fills `data` with the bytes from the camera of the current frame
        Camera.readFrame(data);

        // TODO: convert raw pixel data into formatted 'Chuckduino::Precepts' observation

        /* If we were to visualize `data` we could think of it as a long line of pixels.
         *
         *    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
         *    |     |     |     |     |     |     |     |     |     |     |     |     |     |     |  ...
         *    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
         *
         * Since `data` is representing a rectangular frame, it is more natural to think of `data` as a rectangular.
         * If you were to take `data` and create a new line every `Camera.width()` bytes
         * you will have the desired rectangular representation.
         *
         *    +-----+-----+-----+-----+-----+-----+
         *    |     |     |     |     |     |     |
         *    +-----+-----+-----+-----+-----+-----+
         *    |     |     |     |     |     |     |
         *    +-----+-----+-----+-----+-----+-----+
         *    |     |     |     |     |     |     |
         *    +-----+-----+-----+-----+-----+-----+
         *    |     |     |     |     |     |     |
         *    +-----+-----+-----+-----+-----+-----+
         *
         * You're job is to split `data` into to smaller byte buffers (aka C-Style list),
         * one buffer for the left and right half of the frame.
         * You can see a small example of how this division will go bellow:
         *
         *    +-----+-----+-----+-----+-----+-----+
         *    |  L  |  L  |  L  |  R  |  R  |  R  |
         *    +-----+-----+-----+-----+-----+-----+
         *    |  L  |  L  |  L  |  R  |  R  |  R  |
         *    +-----+-----+-----+-----+-----+-----+
         *    |  L  |  L  |  L  |  R  |  R  |  R  |
         *    +-----+-----+-----+-----+-----+-----+
         *    |  L  |  L  |  L  |  R  |  R  |  R  |
         *    +-----+-----+-----+-----+-----+-----+
         *
         *
         *
         * To accomplish this goal you will need to:
         *    1.) Dynamically allocate two buffers using `malloc` (https://rb.gy/nbtile)
         *    2.) Iterate over `data` pulling the appropriate bytes for the L & R buffers
         * */

        /* ---------------------Memory Allocation--------------------- */
        // Determine the the number of bytes each sub-buffer needs
        // based on the size of the original frame (aka `bytesPerFrame`)

        // Use `malloc` and `sizeof` to dynamically allocate
        // enough bytes for each buffer
        byte* leftBuffer = (byte*) malloc(sizeof(byte)*bytesPerFrame/2);
        byte* rightBuffer = (byte*) malloc(sizeof(byte)*bytesPerFrame/2);

        /* ----------------------Data Extraction---------------------- */

        // This for loop will iterate for 0-length(data).
        // Use the loop to iterate over `data` and
        // fill `leftBuffer` and `rightBuffer` with the appropriate bytes
        int l = 0;
        int r = 0;
        int remainder = 0;
        for(int i = 0; i < bytesPerFrame; i++) {
            remainder = i % Camera.width();
            byte current_byte = data[i];
            if (remainder < Camera.width()/2) {
                leftBuffer[l] = current_byte;
                l++;
            }
            else {
                rightBuffer[r] = current_byte;
                r++;
            }
        }

        /* ----------------------Data Formatting---------------------- */

        // In this section we take the variables we created and
        // organize them into a `Precepts` struct (No Action necessary).
        Precepts percepts {leftBuffer, rightBuffer};
        return percepts;
        
    }

    /* -------------------------------------------------------------------------------------------------------- */

    static inline Chuckduino::Actions deliberate(Chuckduino::Precepts precepts) {
        static Chuckduino::Precepts _prevPrecepts; // TODO: need to initialize and update
        auto euclidianDistance = [&](byte* p, byte* q) { // L2 norm
            int sum = 0;
            for(unsigned int i = 0; i < BUFFER_LENGTH; i++)
                sum = sum + sq(q[i] - p[i]);
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
