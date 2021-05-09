/*

Ayra Jafri

Final Lab / Semester Project

May 7, 2021

OS: Windows 10, Compiler/IDE: Visual Studio 2019

*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>	//included to read and write to and from files
#include <algorithm>	//included for use of the transform(), min(), and max() functions
#include <initializer_list> //used for allowing the min and max functions to take in more than two values
#include <Magick++.h>	//used to process the RGB values of each pixel of the image
#include "AsciiChars.h"	//used to convert the image to the ASCII text version

using namespace std;
using namespace Magick;

int grayScaleConvert(int convertType, int convert8Bit, Color pxlColor) {	//this converts RGB values to brightness / luma values, ranging from 0 - 255
	int redVal = pxlColor.quantumRed() / convert8Bit;	//converts the 16 bit RGB value to an 8 bit value (0 - 255)
	int greenVal = pxlColor.quantumGreen() / convert8Bit;
	int blueVal = pxlColor.quantumBlue() / convert8Bit;

	if (convertType == 1) {	//this calculates the brightness value / grayscale value based on the average
		return (redVal + greenVal + blueVal) / 3;
	}
	else if (convertType == 2) {	//this calculates the brightness value based on the "lightness" value
		return ((max({ redVal, greenVal, blueVal }) + min({ redVal, greenVal, blueVal })) / 2);
	}
	else {	//this calculates brightness values based on the HSP color model
		return sqrt((0.299 * redVal * redVal) + (0.587 * greenVal * greenVal) + (0.114 * blueVal * blueVal));
	}
}

string asciiToHTML(Color pxlColor, int convert8Bit) {	//this assigns the ASCII character at each pixel to the pixel's color; it colors the ASCII character in HTML
	int redVal = pxlColor.quantumRed() / convert8Bit;	//converts the 16 bit RGB value to an 8 bit value (0 - 255)
	int greenVal = pxlColor.quantumGreen() / convert8Bit;
	int blueVal = pxlColor.quantumBlue() / convert8Bit;
	return "<span style=\"color:rgb(" + to_string(redVal) + ", " + to_string(greenVal) + ", " + to_string(blueVal) + ");\">";
}

string asciiToHTML(double grayscale) {	//this assigns the ASCII character at each pixel to the pixel's grayscale color; it colors the ASCII character in HTML in grayscale
	return "<span style=\"color:rgb(" + to_string(grayscale) + ", " + to_string(grayscale) + ", " + to_string(grayscale) + ");\">";
}

int main(int argc, char **argv) {
	InitializeMagick(*argv);	//initializes the Magick++ library
	
	string userInput;
	int brightnessSetting = 1;	//used to keep track of what 
	int picIndex = 0;	//keeps track of which photo to use; especially important if the user chooses to downsize the image, as it allows the program to use the downsized image in calculations
	bool charMultiplier = false;	//keeps track of whether user wants to adjust the horizontal compression (squish) of the generated ASCII text image
	bool contrastAdj = false;	//tracks whether the image's contrast has been adjusted or not
	ifstream reader;	//imports the ASCII grayscale into the program
	ofstream writer;	//exports the ASCII image into a text and/or html file
	vector<Image> images;	//used to retrieve the newly created Image file
	vector<AsciiChars> asciiList;	//stores the ASCII grayscale, along with their respective brightness. This does NOT store the ASCII image
	vector<vector <int>> brightnessValues; //stores the brightness values of each pixel in the image
	vector<vector <string>> colorASCII; //stores rgb values of the image's pixels, to be used when creating a colored ASCII image through HTML
	vector<vector <string>> grayscaleASCII;
	vector<vector <string>> asciiImage;	//stores the converted ASCII image

	cin.clear();
	cin.sync();

	cout << "Hello! Welcome to the ASCII Converter." << endl;
	cout << "The following tool converts images to ASCII text, with help from ImageMagick's Magick++ library." << endl;
	cout << "Would you like to convert an image on your device, or an image from a url?" << endl;
	cout << "Type in \"local\" or \"url\" respectively." << endl;
	getline(cin, userInput);
	transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

	if (userInput.find("url") != string::npos) {
		cout << endl << "Enter the URL below: " << endl;
		getline(cin, userInput);
		Image pic(userInput);
		images.push_back(pic);
	}

	else if (userInput.find("local") != string::npos) {
		string fileExt = "png";
		cout << endl << "*Make sure that the image is in the same folder as the program." << endl;
		cout << "Enter in the image's name, INCLUDING the extension." << endl;
		cout << "For example, enter in \"myImage.png\" (without the quotes)." << endl;
		getline(cin, userInput);
		Image pic(userInput);
		images.push_back(pic);
	}

	int colVal = images.at(picIndex).columns(); //image WIDTH!!!
	int rowVal = images.at(picIndex).rows();	//image HEIGHT!!!

	cout << endl << "If your image is larger than 500x500, it is recommended to resize it to a smaller image." << endl;
	cout << "Would you like to resize your image to a smaller size?" << endl;
	cout << "Enter \"yes\" or \"no\"." << endl;
	getline(cin, userInput);
	transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

	if ((userInput.find("yes") != string::npos)) {	//if the user answers "yes" and one of the image dimensions is greater than 500px
		cout << endl << "Please enter the max width or max height of the ASCII art below." << endl;
		cout << "Enter in only ONE number. The ASCII image will then be resized, adjusting for the aspect ratio." << endl;
		getline(cin, userInput);
		int imgWidth = stoi(userInput);
		if(imgWidth < colVal || imgWidth < rowVal){	//if the entered dimension is less than one of the image's dimensions, resize it!
			Geometry newSize = Geometry(imgWidth, imgWidth);	//the image is resized (in the program ONLY) such that one of its dimensions matches 500px
			Image resizedPic = images.at(picIndex);
			resizedPic.modifyImage();	//prepares the image for modification
			resizedPic.resize(newSize);
			images.push_back(resizedPic);	//adds the resized image to the images vector
			picIndex++;
			colVal = images.at(picIndex).columns();	//updates the colVal and rowVal to match the newly resized image
			rowVal = images.at(picIndex).rows();
		}
	}

	cout << endl << "Would you like to raise the image contrast? This can improve readability of the final ASCII image." << endl;
	cout << "Enter \"yes\" if so. Otherwise enter \"no\"." << endl;
	getline(cin, userInput);
	transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);
	if (userInput.find("yes") != string::npos) {
		Image editedImage = images.at(picIndex);
		editedImage.modifyImage();	//prepares the image for modification
		editedImage.sigmoidalContrast(true, 10.0);	//raises the contrast of the image; the values range from 0.01 to 20.0, so a value in the middle was chosen
		images.push_back(editedImage);	//adds the edited image to the images vector
		picIndex++;
		contrastAdj = true;
	}


	cout << endl << "How would you like to calculate the brightness of each pixel?" << endl;
	cout << "Through the average, lightness, or luma values (as percieved)?" << endl;
	cout << "Enter 1, 2, or 3 respectively." << endl;
	cout << "If you are not sure, enter in 1." << endl;
	getline(cin, userInput);
	brightnessSetting = stoi(userInput);

	int convert8Bit = pow(2, images.at(0).modulusDepth()); //needed to convert 16 bit RGB values to 8 bit RGB values
	brightnessValues.resize(colVal, vector <int>(rowVal));	//resizes 2D vectossssr to dimensions of image
	asciiImage.resize(colVal, vector <string>(rowVal));	//resizes the generated ASCII image to the dimensions of the original image
	colorASCII.resize(colVal, vector <string>(rowVal));	//resizes the generated ASCII image to the dimensions of the original image
	grayscaleASCII.resize(colVal, vector <string>(rowVal));	//resizes the generated ASCII image to the dimensions of the original image

	for (int i = 0; i < rowVal; i++) {
		for (int j = 0; j < colVal; j++) {
			Color imgPixel;	//object that stores each individual pixel of an image
			if(contrastAdj){
				imgPixel = images.at((picIndex - 1)).pixelColor(j, i);	//switch to the previous image that DIDN'T have contrast adjusted, and get colors from that one
				colorASCII[j][i] = asciiToHTML(imgPixel, convert8Bit);	//this saves the RGB colors of each pixel in a matching 2D vector (same dimensions as the image) if the user wants to print to an HTML file
				imgPixel = images.at(picIndex).pixelColor(j, i);	//this sets imgPixel back to the edited image, so that the brightness values are calculated based on the adjusted (contrast) image
			}
			else {
				imgPixel = images.at(picIndex).pixelColor(j, i);
				colorASCII[j][i] = asciiToHTML(imgPixel, convert8Bit);	//this saves the RGB colors of each pixel in a matching 2D vector (same dimensions as the image) if the user wants to print to an HTML file
			}
			brightnessValues[j][i] = grayScaleConvert(brightnessSetting, convert8Bit, imgPixel);	//this converts the RGB values of each pixel, and saves it in a matching 2D matrix
			grayscaleASCII[j][i] = asciiToHTML(brightnessValues[j][i]); ////this saves the grayscale values of each pixel in a matching 2D vector (same dimensions as the image) if the user wants to print to an HTML file
		}
	}

	cout << endl << "Would you like the default range of characters, the extended range, or the experimental extended range of characters?" << endl;
	cout << "The extended range uses 90 characters, instead of the default 70 characters." << endl;
	cout << "The experimental range includes over 200 characters, some of which are not available on the US Keyboard, and thus may not display properly." << endl;
	cout << "The generated image may appear to be uneven and jagged as a result." << endl;
	cout << "Enter 1 for the default range, 2 for the extended range, and 3 for the experimental range." << endl;
	cout << "If you are not sure, enter in 1 for the default range." << endl;
	getline(cin, userInput);

	if (stoi(userInput) == 3) {
		reader.open("asciiLetters2.txt");
	}
	else if (stoi(userInput) == 2) {
		reader.open("asciiLetters3.txt");
	}
	else {
		reader.open("asciiLetters1.txt");
	}
	if (!(reader.is_open())) {
		cout << endl << "Sorry, but the file could not be opened for some reason." << endl;
		cout << "The file may not exist, be incorrectly named, or corrupted." << endl;
		cout << "The program will now end." << endl;
		return 0;
	}

	cout << endl << "One final question before the conversion begins: " << endl;
	cout << "The generated ASCII image may appear to be \"squished\" (horizontally compressed) . This can be somewhat adjusted." << endl;
	cout << "Note that this option is not recommended for large image sizes (past 500x500), or images that have been resized." << endl;
	cout << "Would you like to apply this adjustment? Enter \"yes\" if so. Otherwise enter \"no\"." << endl;
	getline(cin, userInput);
	transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

	if (userInput.find("yes") != string::npos) {
		charMultiplier = true;
	}

	userInput = "";
	while (!(reader.eof()) && !(reader.fail())) {
		int counter = 0;
		while (getline(reader, userInput)) {  //this block of code imports the ascii list into the program
			asciiList.push_back(AsciiChars(userInput, counter));	//and stores them in the asciiList vector as AsciiChars objects
			counter++;
		}
	}
	reader.close();

	for (int i = 0; i < rowVal; i++) {	//this block of code assigns the brightness value of each pixel to its respective ASCII character
		for (int j = 0; j < colVal; j++) {
			for (int n = 0; n < asciiList.size(); n++) {
				if (round(((asciiList.size()-1) / 255.0) * brightnessValues[j][i]) == asciiList.at(n).getBrightness()) {
					asciiImage[j][i] = asciiList.at(n).getChar();	//it then stores each ASCII character to a 2D vector (that matches the image's dimensions)
				}
			}
		}
	}

	cout << endl << "Result: " << endl;	//this block of code prints the generated ASCII image to the command window / terminal 
	for (int i = 0; i < rowVal; i++) {
		for (int j = 0; j < colVal; j++) {
			if (charMultiplier == false) {
				cout << asciiImage[j][i];
			}
			else {
				cout << asciiImage[j][i] << asciiImage[j][i] << asciiImage[j][i];
			}
		}
		cout << endl;
	}

	writer.open("art.txt");	//writes the ASCII image to a txt file
	if (writer.bad()) {    //if an error occurred when creating the file
		cout << "An error occurred in attempting to write the ASCII text to a file. Please try again." << endl; //displays the following
		return 0;																																																//and ends the program
	}

	for (int i = 0; i < rowVal; i++) {	//this block of code saves the generated ASCII image into a text file
		for (int j = 0; j < colVal; j++) {
			writer << asciiImage[j][i];
		}
		writer << endl;
	}
	writer.close();

	cout << endl << "The ASCII art has been exported into a text file." << endl;
	cout << "Look for \"art.txt\" in the program folder." << endl;
	cout << "For optimal viewing pleasure (especially for larger images), zoom out when viewing the text file." << endl;

	cout << endl << "Would you like to export the ASCII art to an HTML file?" << endl;
	cout << "Enter \"yes\" if so, or \"no\" to end the program." << endl;
	getline(cin, userInput);
	transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

	if (userInput.find("yes") != string::npos) {
		cout << endl << "Would you like to export it in color, black and white (just black, really), monochrome (grayscale), or all three?" << endl;
		cout << "Enter in 1, 2, 3, or 4 respectively." << endl;
		getline(cin, userInput);
		transform(userInput.begin(), userInput.end(), userInput.begin(), ::tolower);

		if (userInput.find("1") != string::npos || userInput.find("4") != string::npos) {
			writer.open("asciiColorHTML.html");
			if (writer.bad()) {    //if an error occurred when creating the file
				cout << "An error occurred. Please try again." << endl; //display the following
				return 0;																								//and end the program
			}
			else if (!writer.is_open()) {
				cout << "An error occurred. Please try again." << endl;
				return 0;
			}
			else {
				writer << "<div style=\"font-family: Courier New; font-size:0.5em; line-height:0.1em\"\">";	//this block of code saves the ASCII image into an HTML file, with color
				for (int i = 0; i < rowVal; i++) {
					writer << "<p>";
					for (int j = 0; j < colVal; j++) {
						if (charMultiplier == false) {
							writer << colorASCII[j][i] << asciiImage[j][i] << "</span>";
						}
						else {	//if the user wants to adjust for possible "squish", each character is printed three times per row, instead of just once
							writer << colorASCII[j][i] << asciiImage[j][i] << colorASCII[j][i] << asciiImage[j][i] << colorASCII[j][i] << asciiImage[j][i] << "</span>";
						}
					}
				}
				writer << "</p>";
			}
			writer << "</div>";
			cout << endl << "The colored HTML file has been generated! It will be in the same folder as the generated .txt file, named \"asciiColorHTML.html\"" << endl;
			writer.close();
		}

		if (userInput.find("2") != string::npos || userInput.find("4") != string::npos) {
			writer.open("asciiHTML.html");
			if (writer.bad()) {    //if an error occurred when creating the file
				cout << "An error occurred. Please try again." << endl; //display the following
				return 0;																								//and end the program
			}
			else if (!writer.is_open()) {
				cout << "An error occurred. Please try again." << endl;
				return 0;
			}
			else {
				writer << "<div style=\"font-family: Courier New; font-size:0.5em; line-height:0.1em\"\">";	//this block of color saves the ASCII image into an HTML file, without any color
				for (int i = 0; i < rowVal; i++) {
					writer << "<p>";
					for (int j = 0; j < colVal; j++) {
						if (charMultiplier == false) {
							writer << "<span>" << asciiImage[j][i] << "</span>";
						}
						else {	//if the user wants to adjust for possible "squish", each character is printed three times per row, instead of just once
							writer << "<span>" << asciiImage[j][i] << asciiImage[j][i] << asciiImage[j][i] << "</span>";
						}
					}
				}
				writer << "</p>";
			}
			writer << "</div>";
			cout << endl << "The HTML file has been generated! It will be in the same folder as the generated .txt file, named \"asciiHTML.html\"" << endl;
			writer.close();
		}
		if (userInput.find("3") != string::npos || userInput.find("4") != string::npos) {
			writer.open("asciiGrayscaleHTML.html");
			if (writer.bad()) {    //if an error occurred when creating the file
				cout << "An error occurred. Please try again." << endl; //display the following
				return 0;																								//and end the program
			}
			else if (!writer.is_open()) {
				cout << "An error occurred. Please try again." << endl;
				return 0;
			}
			else {
				writer << "<div style=\"font-family: Courier New; font-size:0.5em; line-height:0.1em\"\">";	//this block of color saves the ASCII image into an HTML file, without any color
				for (int i = 0; i < rowVal; i++) {
					writer << "<p>";
					for (int j = 0; j < colVal; j++) {
						if (charMultiplier == false) {
							writer << grayscaleASCII[j][i] << asciiImage[j][i] << "</span>";
						}
						else {	//if the user wants to adjust for possible "squish", each character is printed three times per row, instead of just once
							writer << grayscaleASCII[j][i] << asciiImage[j][i] << grayscaleASCII[j][i] << asciiImage[j][i] << grayscaleASCII[j][i] << asciiImage[j][i] << "</span>";
						}
					}
				}
				writer << "</p>";
			}
			writer << "</div>";
			cout << endl << "The HTML file has been generated! It will be in the same folder as the generated .txt file, named \"asciiGrayscaleHTML.html\"" << endl;
			writer.close();
		}
		else {
			cout << "Hmm, I didn't quite catch that. Please try running the program again." << endl;
		}
	}
	else {
		writer.close();
		cout << endl << "Goodbye!" << endl;
		return 0;	//the program ends
	}
	writer.close();
	cout << endl << "Goodbye!" << endl;
	return 0;	//the program ends
}