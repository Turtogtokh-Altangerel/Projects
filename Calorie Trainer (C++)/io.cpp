#include "io.h"
#include <cctype>
#include <math.h>

int io::selectUser() const {
    addLongline();
    addNewline(1);
    cout << "Welcome to Calorie Trainer!" << endl;
    addNewline(1);

    cout << "1)New user." << endl;
    cout << "2)Existing user." << endl;
    addNewline(1);
    cout << "Select: ";

    return cinput(1, 2);//the user input is repeatedly requested until 1 or 2 is input.
}

int io::menu() const {
    addNewline(1);
    addLongline();

    cout << "1)Add new food information." << endl;
    cout << "2)Add what I ate." << endl;
    cout << "3)Look up food information." << endl;
    cout << "4)Show my measurements." << endl;
    cout << "5)Change my measurement." << endl;
    cout << "6)Show my food intake." << endl;
    cout << "7)Track my daily goal." << endl;
    cout << "8)Show food list." << endl;
    cout << "9)Exit the program." << endl;
    addNewline(1);

    cout << "Select from the menu: ";
    return cinput(1, 9);//the user input is repeatedly requested until values between 1 and 9 is input.
}

bool io::repeat() const {
    addNewline(1);
    addLongline();

    cout << "1)Show menu." << endl;
    cout << "2)Exit." << endl;
    addNewline(1);

    cout << "Enter: ";
    if (cinput(1, 2) == 1)
        return true; //condition for show menu() again.
    return false;
}

int io::cinput(const int lower, const int upper) const {
    int x;
    bool error = true;

    while (error == true) { //the loop continues until a single numeric value in the rangeis read without anything left in the buffer.
        while (!(cin >> x) || (x < lower || x > upper)) { // in case unsuccessful integer read or out of range value.
            cout << "Please, enter only " << lower << " to " << upper << ": ";
            cin.clear();//clears the error flags. needed for while loop.
            cin.ignore(IGNORE_SIZE, '\n');//ignores all other character in the buffer until newline.
        }
        if (cin.eof() || cin.peek() == '\n')
            error = false; 
        else {
            cin.ignore(IGNORE_SIZE, '\n');//in case there are other characters after the input value.
            cout << "Please, enter only " << lower << " to " << upper << ": ";
        }
    }

    return x;
}

double io::cinput(const int lower) const {
    double x;
    bool error = true;

    while (error == true) { //the loop continues until a single numeric value in the range is read without anything left in the buffer.
        while (!(cin >> x) || x < lower) { // in case unsuccessful integer read or out of range value.
            cout << "Please, enter only one numeric (positive) value: ";
            cin.clear();//clears the error flags. needed for while loop.
            cin.ignore(IGNORE_SIZE, '\n');//ignores all other character in the buffer until newline.
        }
        if (cin.eof() || cin.peek() == '\n')
            error = false;
        else {
            cin.ignore(IGNORE_SIZE, '\n');//in case there are other characters after the input value.
            cout << "Please, enter only one numeric (positive) value: ";
        }
    }

    return x;
}

char io::cinput(const char ch1, const char ch2) const {
    char ch;
    bool error = true;

    while (error == true) { //the loop continues until a single char is read without anything left in the buffer.
        while (!(cin >> ch) || (toupper(ch) != ch1 && toupper(ch) != ch2)) {// in case unsuccessful character read or not desired character.
            cout << "Please, enter only single character" << "(M or F)" << ": ";
            cin.clear();
            cin.ignore(IGNORE_SIZE, '\n');
        }
        if (cin.eof() || cin.peek() == '\n')
            error = false;
        else {
            cin.ignore(IGNORE_SIZE, '\n');//in case there are other characters afther the input character.
            cout << "Please, enter only single character" << "(M or F)" << ": ";
        }
    }

    return toupper(ch);
}

string io::cinput() const {
    string str;
    bool error = true;

    while (error == true) { //the loop continues until a single string is read without anything left in the buffer.
        while (!(cin >> str)) {
            cout << "Please, enter only single name: ";
            cin.clear();
            cin.ignore(IGNORE_SIZE, '\n');
        }
        if (cin.eof() || cin.peek() == '\n')
            error = false;
        else {
            cin.ignore(IGNORE_SIZE, '\n');
            cout << "Please, enter only single name: ";
        }
    }

    return str;
}

string io::upperString(const string& str) const {
    string temp;
    for (size_t i = 0; i < str.size(); i++)
        temp += toupper(str[i]);
    return temp;
}

int io::countDigits(const double& num) const {

    return static_cast<int>(floor(log10(num) + 1));//cast is needed in order to return.
}

void io::addNewline(const int n) const {
    for (int i = 0; i < n; i++)
        cout << endl;
}


void io::addLongline() const {
    for (int i = 0; i < 40; i++)
        cout << "_";
    addNewline(2);
}

