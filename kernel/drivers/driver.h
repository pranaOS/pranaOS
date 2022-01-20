//
// Created by Krisna Pranav on 19/01/22.
//

#pragma once

namespace Kernel {
    class driver {
      public:
        driver(char* name = 0, char* description = 0);
        char* getDriverName();
        char* getDriverDescription();

        bool initialize();

      private:
        char driverName;
        char driverDescription;
    };
}
