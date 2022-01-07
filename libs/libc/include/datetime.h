#pragma once

namespace pranaOSTime {
    class dateTime {
    public:
        signed char day = -1;
        signed char month = -1;
        int year = -1;

        signed char seconds = -1;
        signed char minutes = -1;
        signed char hours = -1;

        static dateTime current();
    };
}