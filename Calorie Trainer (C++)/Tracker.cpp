#include "Tracker.h"
#include <ctime>
#include <iomanip>

Tracker::Tracker(const Customer& C) {
    //initializes members and reads the file.
    numberOfDates = 0;
    daily = NULL;
    fileName = getFileName(C.customerNum); //sets the file name. Each user has different files allocated.
    setDateToday();
    readFile();

    //if the user logins on a new day, the constructor adds the current day and initializes nutritional values consumed.
    if (!(daily[numberOfDates - 1].day == dateToday.day && daily[numberOfDates - 1].month == dateToday.month && daily[numberOfDates - 1].year == dateToday.year)) {
        Daily temp;
        temp.day = dateToday.day;
        temp.month = dateToday.month;
        temp.year = dateToday.year;
        temp.calories = 0;
        temp.carbs = 0;
        temp.fat = 0;
        temp.protein = 0;
        appendFile(temp); //current day is appended to the user file.
        readFile(); //reads again to update 'daily' array.
    }
}

Tracker::~Tracker() {
    if (daily != NULL)
        delete[] daily;
}

void Tracker::readFile() {
    if (daily != NULL)//readFile function is used everytime after write or append takes place.
        delete[] daily;

    //sets the number of dates by counting the number of lines in the file.
    string tmpr;
    ifstream file(fileName); //fileName is already set in the contstructor.
    if (file.fail())
        cout << "File openning has encountered an error";
    numberOfDates = 0;
    while (getline(file, tmpr)) {
        ++numberOfDates;
    }
    file.close();

    //opens the file again and reads the lines in the file into structs in the array.
    try{
        ifstream newfile(fileName);
        if (newfile.fail())
            cout << "File openning has encountered an error";
        daily = new Daily[numberOfDates * sizeof(Daily)];
        for (int i = 0; i < numberOfDates; i++) {
            newfile >> daily[i].day >> daily[i].month >> daily[i].year;
            newfile >> daily[i].calories >> daily[i].protein >> daily[i].carbs >> daily[i].fat;
        }
        newfile.close();
    }
    catch (bad_alloc&) {
        cout << "Memory allocation has encountered an error." << endl; // in case of bad memory allocation.
    }
}

void Tracker::writeFile() const {
    ofstream newfile(fileName);
    if (newfile.fail())
        cout << "File openning has encountered an error";
    for (int i = 0; i < numberOfDates; i++) {
        newfile << daily[i].day << " " << daily[i].month << " " << daily[i].year << " ";
        newfile << daily[i].calories << " " << daily[i].protein << " " << daily[i].carbs << " " << daily[i].fat << endl;
    }
    newfile.close();
}

void Tracker::appendFile(const Daily& temp) const {
    //adds the new date with initialized consumption values to the file.
    ofstream newfile(fileName, ios::app);
    if (newfile.fail())
        cout << "File openning has encountered an error";
    newfile << temp.day << " " << temp.month << " " << temp.year << " ";
    newfile << temp.calories << " " << temp.protein << " " << temp.carbs << " " << temp.fat << endl;
    newfile.close();
}

string Tracker::getFileName(int num) const {
    //users have different files for Tracker class, which are named in the format -> 0.txt 1.txt 2.txt etc depending on the customer No of the user in Customer class.
    string temp = "DATABASE/Tracker/";
    temp += to_string(num) + ".txt"; 
    ifstream tryfile(temp);
    if (tryfile.fail()) { //in case of new user, his or her file does not exist yet, hence creates a file in the folder.
        tryfile.close();
        ofstream newfile(temp);
       
        if (newfile.fail())
            cout << "File openning has encountered an error";
        newfile.close();
        return temp;
    }
    else
        return temp;
}

void Tracker::setDateToday() {
    char date[26];
    time_t now = time(NULL);
    ctime_s(date, sizeof(date), &now);

    string str = date; //converts to string which has the date of current day in the specific format.

    //sets the values of dateToday by retrieving them from str.
    dateToday.month = str.substr(4, 3);
    dateToday.day = stoi(str.substr(8, 2));
    dateToday.year = stoi(str.substr(20, 4));
}

int Tracker::getCurrentdayIndex() const {
    return numberOfDates - 1; //last member struct in the daily array is the current date.
}

void Tracker::displayHistory() const {
    //width is a struct that is inheritted from io class. two values max and min are used as a field width in designing the tabular format.
    width w;
    w.max = 25;
    w.min = 6;
    for (int i = 0; i < numberOfDates; i++)
        w = widthCalculator(daily[i]); //overloaded operator = sets min or max values to the largest width calculated from adjacent structs.
    w.min = w.min + 2;

    //after having the values for field width, prints the information in a tabular format.
    addNewline(1);
    for (int i = 0; i < numberOfDates; i++) {
        addLongline();
        cout << left << "Date: " << daily[i].day << "  " << daily[i].month << "  " << daily[i].year << endl;
        cout << setprecision(0) << fixed;
        cout << left << setw(w.max) << "Consumed calories" << "| " << right << setw(w.min) << daily[i].calories << " cal" << endl;
        cout << setprecision(1) << fixed;
        cout << left << setw(w.max) << "Consumed protein" << "| " << right << setw(w.min) << daily[i].protein << " gr" << endl;
        cout << left << setw(w.max) << "Consumed carbohydrates" << "| " << right << setw(w.min) << daily[i].carbs << " gr" << endl;
        cout << left << setw(w.max) << "Consumed fat" << "| " << right << setw(w.min) << daily[i].fat << " gr" << endl;
    }
}

void Tracker::displayIntakeGoal(const Customer& C) const {
    //sets field width values.
    int today = getCurrentdayIndex();
    width w;
    w.max = 45;
    w.min = (countDigits(C.customers[today].weightGainCalorie) > 6)? countDigits(C.customers[today].weightGainCalorie) : 6;
    double temp;

    //printing.
    addNewline(1);
    cout << setprecision(0) << fixed;
    //each remaining values can become negative values if the user consumer more than the goal amount.
    //therefore, if the remaining values are negative, informs the user has reached the goal amount.
    temp = C.customers[today].dailyCalorie - daily[today].calories;
    cout << left << setw(w.max) << "Remaining caloric intake for maintainance" << "| ";
    if(temp > 0)
        cout << right << setw(w.min) << temp << " cal" << endl;
    else
        cout << right << setw(15) << "Goal achieved!" << endl;

    temp = C.customers[today].weightLossCalorie - daily[today].calories;
    cout << left << setw(w.max) << "Remaining caloric intake for weight loss" << "| ";
    if (temp > 0)
        cout << right << setw(w.min) << temp << " cal" << endl;
    else
        cout << right << setw(15) << "Goal achieved!" << endl;

    temp = C.customers[today].weightGainCalorie - daily[today].calories;
    cout << left << setw(w.max) << "Remaining caloric intake for mass gain" << "| ";
    if (temp > 0)
        cout << right << setw(w.min) << temp << " cal" << endl;
    else
        cout << right << setw(15) << "Goal achieved!" << endl;

    temp = C.customers[today].dailyProtein - daily[today].protein;
    cout << setprecision(1) << fixed;
    cout << left << setw(w.max) << "Remaining protein intake for maintainance" << "| ";
    if (temp > 0)
        cout << right << setw(w.min) << temp << " cal" << endl;
    else
        cout << right << setw(15) << "Goal achieved!" << endl;
}

Tracker::width Tracker::widthCalculator(const Daily& temp) const{
    //similiar to widthCalculator of other two classes but slightly different.
    width x;
    x.max = 0;
    x.min = 6;
    x.min = (countDigits(temp.calories) > x.min) ? countDigits(temp.calories) : x.min;
    x.min = (countDigits(temp.protein) > x.min) ? countDigits(temp.protein) : x.min;
    x.min = (countDigits(temp.carbs) > x.min) ? countDigits(temp.carbs) : x.min;
    x.min = (countDigits(temp.fat) > x.min) ? countDigits(temp.fat) : x.min;
    return x;
}