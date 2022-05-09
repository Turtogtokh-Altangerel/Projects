#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

//this exception states an error with 'gender' variable in Customers class.
class incorrectCharacter : public exception {
	virtual const char* what() const throw()
	{
		return "Input character for gender is incorrect.";
	}
} incorrectChar;

//this exception states an error with 'activity level' variable in Customers class.
class incorrectInteger : public exception {
	virtual const char* what() const throw()
	{
		return "Input selection for activity level is incorrect.";
	}
} incorrectInt;

#endif