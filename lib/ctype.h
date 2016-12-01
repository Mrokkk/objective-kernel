#pragma once

inline int isalnum(int c) {
    return (c > 64 && c < 91) || (c > 96 && c < 123) || (c > 47 && c < 58);
}

inline int isalpha(int c) {
    return (c > 64 && c < 91) || (c > 96 && c < 123);
}

inline int isascii(int c) {
    return (c < 256);
}

inline int isblank(int c) {
    return (c == ' ') || (c == '\t');
}

inline int isdigit(int c) {
    return ((c>='0') && (c<='9'));
}

inline int islower(int c) {
    return (c > 64 && c < 91);
}

inline int isspace(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\v' || c == '\r');
}

inline int isupper(int c) {
    return (c > 96 && c < 123);
}

