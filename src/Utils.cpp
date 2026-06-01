#include "Utils.h"
#include <cctype>
#include <algorithm>

std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) { return ""; }
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

bool parseCoord(const std::string& raw, int& row, int& col) {
    std::string s = trim(raw);
    if (s.size() < 2) { return false; }

    char letter = static_cast<char>(std::toupper(static_cast<unsigned char>(s[0])));
    if (letter < 'A' || letter > 'J') { return false; }

    try {
        int num = std::stoi(s.substr(1));
        if (num < 1 || num > 10) { return false; }
        row = letter - 'A';
        col = num - 1;
        return true;
    } catch (...) {
        return false;
    }
}
