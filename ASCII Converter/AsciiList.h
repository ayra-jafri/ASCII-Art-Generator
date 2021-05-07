#pragma once
#ifndef ASCII_CONVERTER_ASCIILIST_H
#define ASCII_CONVERTER_ASCIILIST_H

#include <iostream>
#include <string>
#include <vector>
#include "AsciiChars.h"
using namespace std;

class AsciiList {
private:
  vector <AsciiChars> asciiList;

public:
  void getSet(int choice);

  void setWhich(int choice);
};
#endif //ASCII_CONVERTER_ASCIILIST_H