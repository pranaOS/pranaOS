#pragma once 

#include <abi/audio.h>

namespace audio {
    struct PCMOutput {
        class audioController* c;
        void* output;

        /**
         * @brief Set the Master Volume object
         * 
         * @param percentage 
         * @return int 
         */
        virtual int setMasterVolume(int percentage) = 0;
    
        /**
         * @brief Get the Master Volume object
         * 
         * @return int 
         */
        virtual int getMasterVolume() const = 0;

        /**
         * @brief get the output default volume
         * 
         * @param output 
         * @param percentage 
         * @return int 
         */
        virtual int outputSetVolume(void* output, int percentage) = 0;

        /**
         * @brief get output volume
         * 
         * @param output 
         * @return int 
         */
        virtual int outputGetVolume(void* output) const = 0;

        /**
         * @brief object to write samples
         * 
         * @param output 
         * @param buffer 
         * @param size 
         * @param async 
         * @return int 
         */
        virtual int writeSamples(void* output, uint8_t buffer, size_t size, bool async) = 0;
    };  

    class audioController {
    public:
        audioController();
        virtual ~audioController();
    private:
        //
    };

    void initialize();

    void registerPCMOutput(PCMOutput* out);

}