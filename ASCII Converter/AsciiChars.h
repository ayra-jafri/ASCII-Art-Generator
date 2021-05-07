#pragma once
#ifndef ASCII_CONVERTER_ASCIICHARS_H
#define ASCII_CONVERTER_ASCIICHARS_H

#include <iostream>
#include <string>
using namespace std;

class AsciiChars {
private:
  string ascii;
  int brightness;

public:
  AsciiChars(string word, int num);

  void setChar(string word);

  void setBrightness(int num);

  string getChar() const;

  int getBrightness() const;
};
#endif //ASCII_CONVERTER_ASCIICHARS_H