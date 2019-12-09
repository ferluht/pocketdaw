//
// Created by Admin on 06.09.2019.
//

#include "TextConversion.h"

namespace utils {

    std::wstring UTF8toUnicode(const std::string &s) {
        std::wstring ws;
        wchar_t wc;
        for (int i = 0; i < s.length();) {
            char c = s[i];
            if ((c & 0x80) == 0) {
                wc = c;
                ++i;
            } else if ((c & 0xE0) == 0xC0) {
                wc = (s[i] & 0x1F) << 6;
                wc |= (s[i + 1] & 0x3F);
                i += 2;
            } else if ((c & 0xF0) == 0xE0) {
                wc = (s[i] & 0xF) << 12;
                wc |= (s[i + 1] & 0x3F) << 6;
                wc |= (s[i + 2] & 0x3F);
                i += 3;
            } else if ((c & 0xF8) == 0xF0) {
                wc = (s[i] & 0x7) << 18;
                wc |= (s[i + 1] & 0x3F) << 12;
                wc |= (s[i + 2] & 0x3F) << 6;
                wc |= (s[i + 3] & 0x3F);
                i += 4;
            } else if ((c & 0xFC) == 0xF8) {
                wc = (s[i] & 0x3) << 24;
                wc |= (s[i] & 0x3F) << 18;
                wc |= (s[i] & 0x3F) << 12;
                wc |= (s[i] & 0x3F) << 6;
                wc |= (s[i] & 0x3F);
                i += 5;
            } else if ((c & 0xFE) == 0xFC) {
                wc = (s[i] & 0x1) << 30;
                wc |= (s[i] & 0x3F) << 24;
                wc |= (s[i] & 0x3F) << 18;
                wc |= (s[i] & 0x3F) << 12;
                wc |= (s[i] & 0x3F) << 6;
                wc |= (s[i] & 0x3F);
                i += 6;
            }
            ws += wc;
        }
        return ws;
    }

    std::string UnicodeToUTF8(const std::wstring &ws) {
        std::string s;
        for (int i = 0; i < ws.size(); ++i) {
            wchar_t wc = ws[i];
            if (0 <= wc && wc <= 0x7f) {
                s += (char) wc;
            } else if (0x80 <= wc && wc <= 0x7ff) {
                s += (0xc0 | (wc >> 6));
                s += (0x80 | (wc & 0x3f));
            } else if (0x800 <= wc && wc <= 0xffff) {
                s += (0xe0 | (wc >> 12));
                s += (0x80 | ((wc >> 6) & 0x3f));
                s += (0x80 | (wc & 0x3f));
            } else if (0x10000 <= wc && wc <= 0x1fffff) {
                s += (0xf0 | (wc >> 18));
                s += (0x80 | ((wc >> 12) & 0x3f));
                s += (0x80 | ((wc >> 6) & 0x3f));
                s += (0x80 | (wc & 0x3f));
            } else if (0x200000 <= wc && wc <= 0x3ffffff) {
                s += (0xf8 | (wc >> 24));
                s += (0x80 | ((wc >> 18) & 0x3f));
                s += (0x80 | ((wc >> 12) & 0x3f));
                s += (0x80 | ((wc >> 6) & 0x3f));
                s += (0x80 | (wc & 0x3f));
            } else if (0x4000000 <= wc && wc <= 0x7fffffff) {
                s += (0xfc | (wc >> 30));
                s += (0x80 | ((wc >> 24) & 0x3f));
                s += (0x80 | ((wc >> 18) & 0x3f));
                s += (0x80 | ((wc >> 12) & 0x3f));
                s += (0x80 | ((wc >> 6) & 0x3f));
                s += (0x80 | (wc & 0x3f));
            }
        }
        return s;
    }

}