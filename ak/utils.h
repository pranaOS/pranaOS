//
// Created by KrisnaPranav on 20/01/22.
//

#pragma once

namespace ak {
    class utils;

    class Utils {
      public:
        bool match(const utils& str);
        bool convert_uint(const utils&, bool& ok);
    };
}