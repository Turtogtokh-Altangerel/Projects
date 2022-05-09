#include "Food.h"
#include <iomanip>

Food::Food() {
    records = NULL;
    fileName = "DATABASE/Food_Data.txt"; //file containing food information.
    readFile(); //constructor calls readFile, sets number of food and dynamic array of structs.
}

Food::~Food() {
    if (records != NULL) //frees the dynamic allocation
        delete[] records;
}

void Food::readFile() {

    if (records != NULL) //readFile function is used everytime after new food is added, hence freeing the previous memory is necessary.
        delete[] records;
    numberOfFood = 0;

    try {
        //opens the file and sets the number of food by checking the number of lines, then allocates a memory dynamically.
        ifstream file(fileName); 
        if (file.fail())
            cout << "File openning has encountered an error";
        string tmp;
        while (getline(file, tmp))
            ++numberOfFood;
        records = new nutrition[numberOfFood * sizeof(nutrition)];
        file.close();

        //opens the file again and reads the lines in the file into structs in the array.
        ifstream newfile(fileName);
        if (newfile.fail())
            cout << "File openning has encountered an error";
        for (int i = 0; i < numberOfFood; i++) {
            newfile >> records[i].name >> records[i].calories >> records[i].protein >> records[i].carbs >> records[i].fat;
        }
        newfile.close();
    }
    catch (bad_alloc&) { //in case of bad memory allocation.
        cout << "Memory allocation has encountered an error." << endl;
    }
}

void Food::appendFile (const nutrition& temp) const {
    //opens the file and appends a new struct received in the function parameter list.
    ofstream newfile(fileName, ios::app);
    if (newfile.fail())
        cout << "File openning has encountered an error";
    newfile << temp.name << " " << temp.calories << " " << temp.protein << " " << temp.carbs << " " << temp.fat << endl;
    newfile.close();
}

int Food::searchFood() const {
    int index;
    bool found = false;
    string str;
    addNewline(1);

    //searches food by comparing the food names with user input, returns the index of the found member of the array, otherwise repeats the user input.
    cout << "Search food: ";
    while (true) {
        str = cinput(); //cinput() assures only single string name is input by the user.
        for (index = 0; index < numberOfFood; index++) {
            if (upperString(str) == upperString(records[index].name)) {
                found = true;
                break;
            }
        }
        if (found == false) {
            addNewline(1);
            cout << str << " is not found." << endl;
            addNewline(1);
            cout << "Enter again: ";
        }
        else
            return index;
    }
}

void Food::addFood() {
    nutrition temp;
    addNewline(1);
    bool repeat = true;

    //cinput() overloaded functions assure the user input values in the correct format and correct range.
    cout << "Enter food name: ";
    while (repeat == true) { //the loop continues until the user inputs a string that does not match any of the foods in the list.
        temp.name = cinput();
        for (int i = 0; i < numberOfFood; i++) {
            if (upperString(temp.name) == upperString(records[i].name)) {
                addNewline(1);
                cout << temp.name << " is already in the food list." << endl;
                addNewline(1);
                cout << "Please, enter a new food: ";
                repeat = true;
                break;
            }
            else
                repeat = false; //in case no matching, the input is allowed and the loop is broken.
        }
    }
    cout << "Enter calories: ";
    temp.calories = cinput(0);
    cout << "Enter protein: ";
    temp.protein = cinput(0);
    cout << "Enter carbs: ";
    temp.carbs = cinput(0);
    cout << "Enter fat: ";
    temp.fat = cinput(0);

    addNewline(1);
    cout << "You added " << temp.name << endl;

    //writes the new food onto the database and reads it again to update the dynamic array.
    appendFile(temp);
    readFile();
}

void Food::displayFood() const {
    int j = searchFood(); //returns the index of the food.
    addNewline(1);

    //width is a struct that is inheritted from io class. two values max and min are used as a field width in designing the tabular format.
    width w;
    w.max = 6;
    w.min = 6;
    for (int i = 0; i < numberOfFood; i++) 
        w = widthCalculator(records[i]); //overloaded operator = sets min or max values to the largest width calculated from adjacent structs.
    w.max = 10;

    //printing in a tabular format.
    cout << left << setw(w.max) << "Food name" << " | " << right << setw(w.min + 4) << records[j].name << endl;
    cout << setprecision(0) << fixed;
    cout << left << setw(w.max) << "Calories" << " | " << right << setw(w.min) << records[j].calories << " cal" << endl;
    cout << setprecision(1) << fixed;
    cout << left << setw(w.max) << "Protein" << " | " << right << setw(w.min) << records[j].protein << "  gr" << endl;
    cout << left << setw(w.max) << "Carbs" << " | " << right << setw(w.min) << records[j].carbs << "  gr" << endl;
    cout << left << setw(w.max) << "Fat" << " | " << right << setw(w.min) << records[j].fat << "  gr" << endl;
}

void Food::displayAllFood() const {
    addNewline(1);
    addLongline();

    //sets field width values for setw.
    width w;
    w.max = 0;
    w.min = 8;
    for (int i = 0; i < numberOfFood; i++)
        w = widthCalculator(records[i]);

    //printing all food nutritional values in a tabular format.
    cout << left << setw(w.max) << "Food name" << " | " << right << setw(w.min) << "Calories" << "     | ";
    cout << setw(w.min) << "Protein" << "    | " << setw(w.min) << "Carbs" << "    | " << setw(w.min) << "Fat" << "    | " << endl;
    for (int j = 0; j < numberOfFood; j++) {
        cout << left << setw(w.max) << records[j].name << " | ";
        cout << setprecision(0) << fixed;
        cout << right << setw(w.min) << records[j].calories << " cal" << " | ";
        cout << setprecision(1) << fixed;
        cout << right << setw(w.min) << records[j].protein << " gr" << " | "
             << right << setw(w.min) << records[j].carbs << " gr" << " | "
             << right << setw(w.min) << records[j].fat << " gr" << " | "
             << endl;
    }
}

Food::width Food::widthCalculator(const nutrition& temp) const {
    width x;
    x.max = 0;
    x.min = 8;

    //sets min or max values to the largest width calculated from adjacent structs.
    x.max = (temp.name.size() > (size_t)x.max) ? temp.name.size() : x.max;
    x.min = (countDigits(temp.calories) > x.min) ? countDigits(temp.calories) : x.min;
    x.min = (countDigits(temp.protein) > x.min) ? countDigits(temp.protein) : x.min;
    x.min = (countDigits(temp.carbs) > x.min) ? countDigits(temp.carbs) : x.min;
    x.min = (countDigits(temp.fat) > x.min) ? countDigits(temp.fat) : x.min;

    return x;
}
