#pragma once

#include <types.h>

namespace LibC {
    // reference: https://www.geeksforgeeks.org/pi-in-c-with-examples/
    #define MATH_PI 3.14159265358979323846

    class math {
    public:     
        /**
         * @brief float values
         * 
         * @param a 
         * @param b 
         * @return float 
         */

        float fmaxf(float a, float b);
        float fminf(float a, float b);
        float clamp(float val, float min, float max);
        
        
        /**
         * @brief double functions 
         * 
         * @param a 
         * @param b 
         * @return double 
         */
        
        double fmax(double a, double b);
        double fmin(double a, double b);
        double fabs(double x);
        double ceil(double x);
        double exp(double x);
        double log(double y);
        double pow(double x, double y);


        /**
         * @brief integers
         * 
         * @param a 
         * @param b 
         * @return int 
         */

        int min(int a, int b);
        int max(int a, int b);
        int powi(int x, int y);
        
    };
}