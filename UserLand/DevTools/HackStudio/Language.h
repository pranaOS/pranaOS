#pragma once

// includes
#include <AK/String.h>

namespace HackStudio {
enum class Language {
    Unknown,
    Cpp,
    JavaScript,
    GML,
    Ini,
    Shell,
};

Language language_from_file_extension(const String&);
Language language_from_name(const String&);

}
