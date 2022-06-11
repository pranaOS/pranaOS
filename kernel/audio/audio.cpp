#include "audio.h"
#include <stdint.h>

namespace audio {
    PCMOutput* currentOutput;

    class mixerDevice {
    public:
        int ioctl(uint64_t cmd, uint64_t arg) override {
            switch(cmd) {
            default:
                return -EINVAL;
            }

            return -EINVAL;
        }
    };

    class soundFS {
    public: 
        soundFS() {

        }
    };

    audioController::~audioController(){
    }

    soundFS sfs;
    void initialize() {
        sfs = new soundFS();
    }
}