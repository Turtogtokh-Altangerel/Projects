#ifndef IO_H
#define IO_H
#include <iostream>
#include <string>

using namespace std;

class io {
private:
	const int IGNORE_SIZE = 512; //used in cin.ignore();
public:
	struct width {
		int max;
		int min;
		//struct operator that compares max and min of one width variable with max and min of another width variable.
		//and changes max and min to be the larger values for left hand side width.
		width& operator =(const width& other) {
			max = (max > other.max) ? max : other.max;
			min = (min > other.min) ? min : other.min;
			return *this; //returns a copy as a reference.
		}
	};
	//starts the menu. chooses new or existing user.
	int selectUser() const;

	//main menu.
	int menu() const;

	//returns true if the user repeats menu(), otherwise false.
	bool repeat() const;

	//overloaded functions for cin.
	int cinput(const int lower, const int upper) const;//receives lower and upper limit, asks the user to enter a value between them and returns only value in the desired range.

	double cinput(const int lower) const;//receives lower limit, asks the user to enter a value larger than it and returns only value in the desired range.

	char cinput(const char ch1, const char ch2) const;//receives two character from which the user chooses and returns only one of the characters.

	string cinput() const;//asks the user to enter a single string without space and returns only if such string is input.

	string upperString(const string& str) const;//receives a string and returns an uppercase version of the string.

	//returns the number of digits in a double variable excluding its decimal part.
	int countDigits(const double& num) const;

	//adds endlines as many times as requested.
	void addNewline(const int n) const;
	
	//adds a dashed long line.
	void addLongline() const;

};
#endif