//
// Created by Admin on 06.09.2019.
//

#ifndef PD_TEXTCONVERSION_H
#define PD_TEXTCONVERSION_H

#include <stdlib.h>
#include <string>

namespace utils {

    std::wstring UTF8toUnicode(const std::string &s);
    std::string UnicodeToUTF8(const std::wstring &ws);

}

#endif //PD_TEXTCONVERSION_H
