#include <cstring>

inline int ByteToInt(char* buffer) {
  int number = 0;
  memcpy(&number, buffer, sizeof(int));
  return number;
}

inline void IntToByte(int number, char* buffer) {
  memcpy(buffer, &number, sizeof(int));
}