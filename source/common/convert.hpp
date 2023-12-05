#include <cstring>

inline int ToInt(char* buffer) {
    int a = 0;
    memcpy(&a, buffer, sizeof(int));
    return a;
}