
#include <iostream>
#include <string>
#include "AsciiChars.h"

using namespace std;

AsciiChars::AsciiChars(string word, int bright) {
	ascii = word;
	brightness = bright;
}

void AsciiChars::setChar(string word) {
	ascii = word;
}

void AsciiChars::setBrightness(int num) {
	brightness = num;
}

string AsciiChars::getChar() const{
	return ascii;
}

int AsciiChars::getBrightness() const{
	return brightness;
}