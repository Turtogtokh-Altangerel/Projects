#include "Customer.h"
#include "Exception.h"
#include <iomanip>

Customer::Customer() {
    customers = NULL;
    fileName = "DATABASE/Customer_Data.txt"; //file containing all users with their measurements.
    readFile();
    customerNum = -1;
}

Customer::~Customer() {
    if (customers != NULL)
        delete[] customers;
}

void Customer::readFile() {
    if (customers != NULL) // readFile function is used everytime after write or append takes place
        delete[] customers;

    //sets the number of users by reading the number of lines in the customer database.
    string tmpr;
    numberOfCustomers = 0;
    ifstream file(fileName);
    if (file.fail())
        cout << "File openning has encountered an error";
    while (getline(file, tmpr)) {
        ++numberOfCustomers;
    }
    file.close();

    //opens the file again and reads the lines in the file into structs in the array.
    try{
        ifstream newfile(fileName);
        if (newfile.fail())
            cout << "File openning has encountered an error";

        customers = new Info[numberOfCustomers * sizeof(Info)];

        for (int i = 0; i < numberOfCustomers; i++) {
            newfile >> customers[i].name >> customers[i].weight >> customers[i].height;
            newfile >> customers[i].age >> customers[i].gender >> customers[i].activityLevel >> customers[i].bmr;
            newfile >> customers[i].dailyCalorie >> customers[i].weightLossCalorie >> customers[i].weightGainCalorie >> customers[i].dailyProtein;
        }
        newfile.close();
    }
    catch (bad_alloc&) {
        cout << "Memory allocation has encountered an error." << endl;//in case of bad memory allocation.
    }
}

void Customer::writeFile() const {
    ofstream newfile(fileName);
    if (newfile.fail())
        cout << "File openning has encountered an error";

    for (int i = 0; i < numberOfCustomers; i++) {
        newfile << customers[i].name << " " << customers[i].weight << " " << customers[i].height << " ";
        newfile << customers[i].age << " " << customers[i].gender << " " << customers[i].activityLevel << " " << customers[i].bmr << " ";
        newfile << customers[i].dailyCalorie << " " << customers[i].weightLossCalorie << " " << customers[i].weightGainCalorie << " " << customers[i].dailyProtein << endl;
    }
    newfile.close();
}

void Customer::appendFile(const Info& temp) const {
    //adds the new user information with initialized consumption values to the file.
    ofstream newfile(fileName, ios::app);
    if (newfile.fail())
        cout << "File openning has encountered an error";
    newfile << temp.name << " " << temp.weight << " " << temp.height << " " << temp.age << " " << temp.gender << " ";
    newfile << temp.activityLevel << " " << temp.bmr << " " << temp.dailyCalorie << " ";
    newfile << temp.weightLossCalorie << " " << temp.weightGainCalorie << " " << temp.dailyProtein << "\n";
    newfile.close();
}

char Customer::genderSelector() const {
    char temp;
    addNewline(1);
    cout << "Select your gender(M or F)?: ";
    temp = cinput('M', 'F'); //only characters M or F are returned.
    return temp;
}

string Customer::genderSelector(const char& ch) const {
    switch (ch) {
    case 'M':
        return "Male";
    case 'F':
        return "Female";
    default:
        throw incorrectChar; // in case of an unfavourable char.
    }
}

double Customer::activitySelector() const {
    //selections.
    addNewline(1);
    cout << "Select your activity level." << endl;
    cout << "1)Sedentary(little or no exercise)" << endl;
    cout << "2)Lightly active(light exercise 1-3 days per week)" << endl;
    cout << "3)Moderately active(moderate exercise 3-5 days per week)" << endl;
    cout << "4)Very active(hard exercise 6-7 days a week)" << endl;
    cout << "5)Extra active(daily intense exercise, sports or physical job)" << endl;

    addNewline(1);
    cout << "Select: ";

    //cinput() overloaded functions assure the user input values in the correct format and correct range.
    switch (cinput(1, 5)) {
    case 1: return 1.2;
    case 2: return 1.375;
    case 3: return 1.55;
    case 4: return 1.725;
    case 5: return 1.9;
    default: throw incorrectInt; //to ensure returning for all control paths.
    }
}

string Customer::activitySelector(const double& num) const {
    if (num == 1.2)
        return "Sedentary";
    if (num == 1.375)
        return "Lightly active";
    if (num == 1.55)
        return "Moderately active";
    if (num == 1.725)
        return "Very active";
    if (num == 1.9)
        return "Extra active";
    else
        throw incorrectInt; // in case of an unfavourable value.
}

double Customer::calculateBmr(const char& gender, const double& weight, const double& height, const double& age) const {
    //calculation of BMR according to known formula.
    if (gender == 'M')
        return 10 * weight + 6.25 * height - 5 * age + 5;
    else if (gender == 'F')
        return 10 * weight + 6.25 * height - 5 * age - 161;
    else
        throw incorrectChar; //to ensure returning for all control paths.
}

void Customer::changeValues() {
    int tmp;
    double newvalue;

    addNewline(1);
    cout << "1)Weight\n2)Height\n3)Age\n4)Gender\n5)ActivityLevel" << endl;
    cout << "Which measurement would you like to modify?" << endl;

    addNewline(1);
    cout << "Enter: ";

    //asks the user input between 1 and 5.
    tmp = cinput(1, 5);
    if (tmp <= 3) {
        addNewline(1);
        cout << "Enter the new value for ";
        switch (tmp) {
        case 1: cout << "weight: ";
        case 2: cout << "height: ";
        case 3: cout << "age: ";
        }
        newvalue = cinput(0);
    }
    switch (tmp) { //changes the measurement value of the user according to switch case.
    case 1: customers[customerNum].weight = newvalue;
        break;
    case 2: customers[customerNum].height = newvalue;
        break;
    case 3: customers[customerNum].age = newvalue;
        break;
    case 4: customers[customerNum].gender = genderSelector();
        break;
    case 5: customers[customerNum].activityLevel = activitySelector();
        break;
    }

    //after a change in user info, other calculated information has to updated in the dynamic array as well as in the database.
    customers[customerNum].bmr = calculateBmr(customers[customerNum].gender, customers[customerNum].weight, customers[customerNum].height, customers[customerNum].age);
    customers[customerNum].dailyCalorie = customers[customerNum].bmr * customers[customerNum].activityLevel;
    customers[customerNum].weightLossCalorie = customers[customerNum].dailyCalorie - 400;
    customers[customerNum].weightGainCalorie = customers[customerNum].dailyCalorie + 400;
    customers[customerNum].dailyProtein = customers[customerNum].weight * 1.2;
    writeFile();
}

void Customer::setCustomerNum(const int& i) {
    customerNum = i; //sets the current user's index in the customers array equal to the int received.
}

void Customer::addCustomer() {
    //cinput() overloaded functions assure the user input values in the correct format and correct range.
    //loads the input into a temporary struct.
    Info temp;
    bool repeat = true;
    addNewline(1);
    cout << "Enter your nickname: ";
    while (repeat == true) { //the loop continues until the user inputs a string that does not match any of the names of existing users.
        temp.name = cinput();
        for (int i = 0; i < numberOfCustomers; i++) {
            if (upperString(temp.name) == upperString(customers[i].name)) {
                addNewline(1);
                cout << "A user with the same name already exists." << endl;
                addNewline(1);
                cout << "Please, enter an unique name: ";
                repeat = true;
                break;
            }
            else
                repeat = false; //in case no matching, the input name is allowed and the loop is broken.
        }
    }
    cout << "Enter your weight: ";
    temp.weight = cinput(0);
    cout << "Enter your height: ";
    temp.height = cinput(0);
    cout << "Enter your age: ";
    temp.age = cinput(0);
    temp.gender = genderSelector();

    //computes and sets the remaining measurement values.
    try {
        temp.activityLevel = activitySelector();
        temp.bmr = calculateBmr(temp.gender, temp.weight, temp.height, temp.age);
        temp.dailyCalorie = temp.bmr * temp.activityLevel;
        temp.weightLossCalorie = temp.dailyCalorie - 400;
        temp.weightGainCalorie = temp.dailyCalorie + 400;
        temp.dailyProtein = temp.weight * 1.2;

        appendFile(temp); //appends the new user's information onto the customers file.
        readFile(); //updates the customers array.
        setCustomerNum(numberOfCustomers - 1); //in case of a new user, the last user in the customers array is the current user.
    }
    catch (exception& ex) {
        cout << ex.what() << endl; //in case activitySelector() or calculateBmr() functions throw an error.
    }
}

int Customer::findCustomer() const {
    string temp;
    addNewline(1);

    //the loop repeats request for an user input until the name of a user that exists in the database is found.
    cout << "Enter your name: ";
    while (true) { //the loop continues until the strings match.
        temp = cinput();
        //checks every name with temp and returns the index in case of matching.
        for (int index = 0; index < numberOfCustomers; index++) {
            if (upperString(temp) == upperString(customers[index].name))
                return index;
        }
        addNewline(1);
        cout << "Sorry. Could not find your name." << endl;
        addNewline(1);
        cout << "Enter again: ";
    }
}

void Customer::displayCustomer() const {
    int i = customerNum;
    addNewline(1);
    addLongline();

    //sets field width values for setw().
    width w;
    w.max = 9;
    w.min = 6;
    for(int i = 0; i < numberOfCustomers; i++)
        w = widthCalculator(customers[i]);
    w.min = (w.min < 6) ? 6 : w.max - 3; //setting field value for w.min.

    //displays the measurements of the current user in a tabular format.
    cout << left << setw(30) << "Name" << "| " << right << setw(w.max) << customers[i].name << endl;
    cout << left << setw(30) << "Weight" << "| " << right << setw(w.min) << customers[i].weight << " kg" << endl;
    cout << left << setw(30) << "Height" << "| " << right << setw(w.min) << customers[i].height << " cm" << endl;
    cout << left << setw(30) << "Age" << "| " << right << setw(w.max) << customers[i].age << endl;
    try {
        cout << left << setw(30) << "Gender" << "| " << right << setw(w.max) << genderSelector(customers[i].gender) << endl;
        cout << left << setw(30) << "Activity level" << "| " << right << setw(w.max) << activitySelector(customers[i].activityLevel) << endl;
    }
    catch (exception& ex) { //in case of any exceptions thrown.
        cout << ex.what() << endl;
    }
    cout << setprecision(0) << fixed;
    cout << left << setw(30) << "BMR: " << "| " << right << setw(w.min) << customers[i].bmr << "cal" << endl;
    cout << left << setw(30) << "Maintainance Caloric intake" << "| " << right << setw(w.min) << customers[i].dailyCalorie << "cal" << endl;
    cout << left << setw(30) << "Weight loss Caloric intake" << "| " << right << setw(w.min) << customers[i].weightLossCalorie << "cal" << endl;
    cout << left << setw(30) << "Weight gain Caloric intake" << "| " << right << setw(w.min) << customers[i].weightGainCalorie << "cal" << endl;
    cout << left << setw(30) << "Maintainance Protein intake" << "| " << right << setw(w.min) << customers[i].dailyProtein << "cal" << endl;
}

Customer::width Customer::widthCalculator(const Info& temp) const {
    width x;
    x.max = 0;
    x.min = 6;

    //sets x.max value equal to the largest width out of widths of some members of the received struct.
    x.max = (temp.name.size() > (size_t)x.max) ? temp.name.size() : x.max;
    x.max = (genderSelector(temp.gender).size() > (size_t)x.max) ? genderSelector(temp.gender).size() : x.max;
    x.max = (activitySelector(temp.activityLevel).size() > (size_t)x.max) ? activitySelector(temp.activityLevel).size() : x.max;
    x.max = (countDigits(temp.weightGainCalorie) > x.max) ? countDigits(temp.weightGainCalorie) : x.max;
    
    //returns the values.
    return x;
}