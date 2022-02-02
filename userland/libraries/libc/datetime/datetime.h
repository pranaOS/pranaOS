#pragma once

namespace LibC {

    class dateTime {
    public:
        /**
         * @brief day
         * 
         */
        signed char day = -1;
        /**
         * @brief month
         * 
         */
        signed char month = -1;

        /**
         * @brief year
         * 
         */
        int year = -1;

        /**
         * @brief seconds
         * 
         */
        signed char seconds = -1;

        /**
         * @brief minutes
         * 
         */
        signed char minutes = -1;

        /**
         * @brief hours
         * 
         */
        signed char hours = -1;

        /**
         * @brief current datetime
         * 
         * @return dateTime 
         */
        static dateTime current();

        /**
         * @brief convert time tostring()
         * 
         * @return char* 
         */
        char* toString();
    };
    
}