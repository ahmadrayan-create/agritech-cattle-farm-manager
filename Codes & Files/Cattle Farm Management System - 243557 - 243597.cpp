#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;



#define MAX_ANIMALS 100
#define MAX_TASKS 100
#define MAX_FINREC 100


string toTitleCase(string str) {
    if (str.length() == 0)
        return str;
    for (int i = 0; i < str.size(); i++) {
        if (i == 0 || str[i - 1] == ' ') {
            if (str[i] >= 'a' && str[i] <= 'z')
                str[i] = str[i] - ('a' - 'A');
        }
        else {
            if (str[i] >= 'A' && str[i] <= 'Z')
                str[i] = str[i] + ('a' - 'A');
        }
    }
    return str;
}



bool isValidDescription(string desc) {
    for (size_t i = 0; i < desc.size(); i++) {
        char c = desc[i];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')) {
            return false;
        }
    }
    return true;
}








void clearScreen() {
    system("cls"); // Use "clear" on Unix-based OS
}



void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// =============================================================================
//                        USER & LOGIN MANAGER CLASSES
// =============================================================================

class User {
protected:
    string realName;
    string username;
    string password;
    string role;
    string backupWord1;
    string backupWord2;


    string generateBackupWord() {
        string word = "";
        for (int i = 0; i < 5; i++) {
            word += 'a' + rand() % 26;
        }
        return word;
    }

public:
 
 
 
    User() {
        username = "";
        realName = "";
        password = "";
        role = "";
        backupWord1 = generateBackupWord();
        backupWord2 = generateBackupWord();
    }




    User(string name, string u, string p, string r) {
        username = u;
        password = p;
        realName = name;
        r = toTitleCase(r);
        while (r != "Owner" && r != "Worker" && r != "Vetdoctor") {
            cout << "Invalid role entered. Please enter 'Owner', 'Worker', or 'VetDoctor': ";
            cin >> r;
            r = toTitleCase(r);
        }
        role = r;
        backupWord1 = generateBackupWord();
        backupWord2 = generateBackupWord();
    }


    string getRealName() { return realName; }
    string getUsername() { return username; }
    string getPassword() { return password; }
    string getRole() { return role; }
    string getBackupWord1() { return backupWord1; }
    string getBackupWord2() { return backupWord2; }

  
    void displayUserInfo() {
        cout << "Name: " << realName << " | Role: " << role << endl;
    }

    void saveToFile() {
        ofstream fout("users.txt", ios::app);
        if (fout.is_open()) {
            fout << username << " " << password << " " << role << " "
                 << backupWord1 << " " << backupWord2 << "\n";
            fout.close();
        }
        else {
            cout << "Error: Cannot write to users.txt" << endl;
        }
    }



    static bool loadUser(string uname, User &userOut) {
        ifstream fin("users.txt");
        if (!fin.is_open()) {
            cout << "User database not found. Please register first." << endl;
            return false;
        }
        string u, p, r, bw1, bw2;
        while (fin >> u >> p >> r >> bw1 >> bw2) {
            if (u == uname) {
                userOut.username = u;
                userOut.password = p;
                userOut.role = r;
                userOut.backupWord1 = bw1;
                userOut.backupWord2 = bw2;
                fin.close();
                return true;
            }
        }
        fin.close();
        return false;
    }
}; 


class LoginManager : public User {
public:


    LoginManager() { }


    void registerUser() {
        clearScreen();
        string usernameInput, passwordInput, roleInput;
        cout << "\n--- Register New User ---" << endl;
        
        cout << "Enter your Full Name (e.g.Ahmad Rayan Qasim): ";
        getline(cin, realName);
        
        
        
        while (!isValidDescription(realName)) {
            cout << "Invalid name. Please use only alphabetic characters and spaces: ";
            getline(cin, realName);
        }
        
        
        
        bool usernameValid = false;
        do {
            cout << "Enter Username (example: ahmadrayan88): ";
            cin >> usernameInput;
            if (usernameInput == "") {
                cout << "Username cannot be empty. Please enter a valid username." << endl;
            }
            else {
                User tempUser;
                if (User::loadUser(usernameInput, tempUser)) {
                    cout << "Username already exists. Please choose a different username." << endl;
                }
                else {
                    usernameValid = true;
                }
            }
        } while (!usernameValid);
        



        do {
            cout << "Enter Password (minimum 8 characters): ";
            cin >> passwordInput;
            if (passwordInput.length() < 8) {
                cout << "Password must be at least 8 characters long." << endl;
            }
        } while (passwordInput.length() < 8);
        



        do {
            cout << "\nRole Selection:" << endl;
            cout << "1. Owner" << endl;
            cout << "2. Worker" << endl;
            cout << "3. Vet Doctor" << endl;
            cout << "Enter your choice (1/2/3): ";
            int c1;
            while (!(cin >> c1)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter 1, 2, or 3: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (c1 == 1) {
                roleInput = "Owner";
            }
            else if (c1 == 2) {
                roleInput = "Worker";
            }
            else if (c1 == 3) {
                roleInput = "Vetdoctor";
            }
            else {
                roleInput = "";
                cout << "Invalid choice. Please select 1, 2, or 3." << endl;
            }
        } while (roleInput != "Owner" && roleInput != "Worker" && roleInput != "Vetdoctor");
        
        User newUser(realName, usernameInput, passwordInput, roleInput);
        newUser.saveToFile();
        
        cout << "\nUser registered successfully!" << endl;
        cout << "IMPORTANT: Your backup words are: " 
             << newUser.getBackupWord1() << " " << newUser.getBackupWord2() << endl;
        pauseScreen();
    }



    
    void login() {
        clearScreen();
        string uname, pwd;
        cout << "\n--- Login ---" << endl;
        cout << "Enter Username: ";
        cin >> uname;
        cout << "Enter Password: ";
        cin >> pwd;
        
        User userFound;
        if (!User::loadUser(uname, userFound)) {
            cout << "User not found. Please register first." << endl;
            exit(0);
        }
        
        int attempts = 0;
        while (attempts < 3) {
            if (pwd == userFound.getPassword()) {
                cout << "\nLogin successful!" << endl;
                cout << "-----------------------------------------" << endl;
                cout << "Welcome, " << userFound.getRealName() 
                     << "! Your role is: " << userFound.getRole() << endl;
                cout << "-----------------------------------------" << endl;
                username = userFound.getUsername();
                password = userFound.getPassword();
                role = userFound.getRole();
                pauseScreen();
                return;
            }
            
            
            
            
            
            attempts++;
            if (attempts < 3) {
                cout << "Incorrect password. Try again (" << 3 - attempts << " attempts left): ";
                cin >> pwd;
            }
        }
        cout << "\nMax attempts reached. Enter your backup words:" << endl;
        string inputBW1, inputBW2;
        cout << "Enter Backup Word 1: ";
        cin >> inputBW1;
        cout << "Enter Backup Word 2: ";
        cin >> inputBW2;
        if ((inputBW1 == userFound.getBackupWord1()) && (inputBW2 == userFound.getBackupWord2())) {
            cout << "Backup authentication successful. Login granted!" << endl;
            username = userFound.getUsername();
            password = userFound.getPassword();
            role = userFound.getRole();
            pauseScreen();
            return;
        }
        else {
            cout << "Backup authentication failed. Access denied. Exiting program..." << endl;
            exit(0);
        }
    }
};







class Animal {
protected:
    int id;
    string name;
    string gender;
    int ageYears;
    string category;
    string condition;

public:



    Animal() {
        id = 0;
        name = "";
        gender = "";
        ageYears = 0;
        category = "";
        condition = "Healthy";
    }
    

    Animal(int id, string name, string gender, int ageYears, string category, string condition) {
        this->id = id;
        this->name = name;
        this->gender = gender;
        this->ageYears = ageYears;
        this->category = category;
        this->condition = condition;
    }
    




    void displayInfo() {
        cout << "ID: " << id 
             << " | Name: " << name 
             << " | Gender: " << gender 
             << " | Age: " << ageYears << " years" 
             << " | Category: " << category 
             << " | Condition: " << condition;
    }
    





    void updateRecord() {
    cout << "Updating record for Animal ID: " << id << endl;
    cin.ignore();

 
 
 
    cout << "Enter new name: ";
    getline(cin, name);
    while (!isValidDescription(name)) {
        cout << "Invalid name. Please use only alphabetic characters and spaces: ";
        getline(cin, name);
    }




    do {
        cout << "Enter new gender (Male/Female): ";
        getline(cin, gender);
        while (!isValidDescription(gender)) {
            cout << "Invalid gender. Please use only alphabetic characters and spaces: ";
            getline(cin, gender);
        }
        gender = toTitleCase(gender);
    } while (gender != "Male" && gender != "Female");





    cout << "Enter new age in years: ";
    while (!(cin >> ageYears)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter a numeric value: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    
    
    




    do {
        cout << "Enter new condition (Healthy/Sick): ";
        getline(cin, condition);
        condition = toTitleCase(condition);
        if (condition != "Healthy" && condition != "Sick") {
            cout << "Invalid input! Please enter only 'Healthy' or 'Sick'." << endl;
        }
    } while (condition != "Healthy" && condition != "Sick");
}

    





    bool validateAnimalID(int searchID) {
        return (id == searchID);
    }
    




    bool operator==(Animal &other) {
        return (this->id == other.id);
    }
    





    Animal& operator=(Animal &other) {
        if (this != &other) {
            this->id = other.id;
            this->name = other.name;
            this->gender = other.gender;
            this->ageYears = other.ageYears;
            this->category = other.category;
            this->condition = other.condition;
        }
        return *this;
    }
    



    int getID() { return id; }
    string getName() { return name; }
    string getGender() { return gender; }
    int getAge() { return ageYears; }
    string getCategory() { return category; }
    string getCondition() { return condition; }
    




    void setID(int newID) { id = newID; }
    void setName(string newName) { name = newName; }
    void setGender(string newGender) { gender = newGender; }
    void setAge(int newAge) { ageYears = newAge; }
    void setCategory(string newCategory) { category = newCategory; }
    void setCondition(string newCondition) { condition = newCondition; }
};






class Cow : public Animal {
public:
    float milkProduction;       
    string breed;
    string breedingDate;
    string lastHeatDate;
    string nextHeatDate;         
    
    
    
    
Cow& operator=(const Cow &other) {
        if (this != &other) {
            this->id = other.id;
            this->name = other.name;
            this->gender = other.gender;
            this->ageYears = other.ageYears;
            this->category = other.category;
            this->condition = other.condition;
            this->milkProduction = other.milkProduction;
            this->breed = other.breed;
            this->breedingDate = other.breedingDate;
            this->lastHeatDate = other.lastHeatDate;
            this->nextHeatDate = other.nextHeatDate;
        }
        return *this;
    }





    Cow() : Animal() {
        milkProduction = 0.0;
        breed = "";
        breedingDate = "";
        lastHeatDate = "";
        nextHeatDate = "";
    }
    



    Cow(int id, string name, string gender, int ageYears, string condition, 
        float milkProduction, string breed, string breedingDate, string lastHeatDate)
    : Animal(id, name, gender, ageYears, "Cow", condition) {
        this->milkProduction = milkProduction;
        this->breed = breed;
        this->breedingDate = breedingDate;
        this->lastHeatDate = lastHeatDate;
        
        
        nextHeatDate = "21 days After last heat date";
    }
    




    void displayInfo() {
        Animal::displayInfo();
        cout << " | Milk Production: " << milkProduction << " liters"
             << " | Breed: " << breed 
             << " | Breeding Date: " << breedingDate 
             << " | Last Heat Date: " << lastHeatDate 
             << " | Next Heat Date: " << nextHeatDate << endl;
    }
    

void updateRecord() {
    cout << "Updating Cow record with ID: " << id << endl;
    cin.ignore();

   
    cout << "Enter new name: ";
    getline(cin, name);
    while (!isValidDescription(name)) {
        cout << "Invalid name. Please use only alphabetic characters and spaces: ";
        getline(cin, name);
    }

  
gender = "Female";


    cout << "Enter new age in years: ";
    while (!(cin >> ageYears)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter a numeric value: ";
    }


    do {
        cout << "Enter new milk production (liters, must be non-negative): ";
        cin >> milkProduction;
        if (milkProduction < 0)
            cout << "Invalid input. Milk production cannot be negative." << endl;
    } while (milkProduction < 0);
    cin.ignore();


    cout << "Enter new breed: ";
    getline(cin, breed);
    while (!isValidDescription(breed)) {
        cout << "Invalid breed. Please use only alphabetic characters and spaces: ";
        getline(cin, breed);
    }
    breed = toTitleCase(breed);


    cout << "Enter new breeding date (YYYY-MM-DD): ";
    while (!(getline(cin, breedingDate)) || breedingDate.length() != 10 || breedingDate[4] != '-' || breedingDate[7] != '-') {
        cout << "Invalid format! Please enter date in YYYY-MM-DD format: ";
    }


    cout << "Enter new last heat date (YYYY-MM-DD): ";
    while (!(getline(cin, lastHeatDate)) || lastHeatDate.length() != 10 || lastHeatDate[4] != '-' || lastHeatDate[7] != '-') {
        cout << "Invalid format! Please enter date in YYYY-MM-DD format: ";
    }


    nextHeatDate = "21 days after last heat date";
}

};


class Bull : public Animal {
public:
    float fertilityRate;         
    float breedingSuccessRate;  

    Bull() : Animal() {
        fertilityRate = 0.0;
        breedingSuccessRate = 0.0;
    }
    
    Bull(int id, string name, string gender, int ageYears, string condition, 
         float fertilityRate, float breedingSuccessRate)
    : Animal(id, name, gender, ageYears, "Bull", condition) {
        this->fertilityRate = fertilityRate;
        this->breedingSuccessRate = breedingSuccessRate;
    }
    
    void displayInfo() {
        Animal::displayInfo();
        cout << " | Fertility Rate: " << fertilityRate << "%"
             << " | Breeding Success Rate: " << breedingSuccessRate << "%" << endl;
    }
    

   Bull& operator=(const Bull &other) {
        if (this != &other) {
            this->id = other.id;
            this->name = other.name;
            this->gender = other.gender;
            this->ageYears = other.ageYears;
            this->category = other.category;
            this->condition = other.condition;
            this->fertilityRate = other.fertilityRate;
            this->breedingSuccessRate = other.breedingSuccessRate;
        }
        return *this;
    }
    
    
    
void updateRecord() {
    cout << "Updating Bull record with ID: " << id << endl;
    cin.ignore();

    // Validate Name
    cout << "Enter new name: ";
    getline(cin, name);
    while (!isValidDescription(name)) {
        cout << "Invalid name. Please use only alphabetic characters and spaces: ";
        getline(cin, name);
    }
    name = toTitleCase(name);

    gender ="Male";

    // Validate Age
    cout << "Enter new age in years: ";
    while (!(cin >> ageYears) || ageYears <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter a valid positive number: ";
    }

    // Validate Fertility Rate
    do {
        cout << "Enter new fertility rate (percentage, non-negative): ";
        while (!(cin >> fertilityRate) || fertilityRate < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a non-negative percentage: ";
        }
    } while (fertilityRate < 0);

    // Validate Breeding Success Rate
    do {
        cout << "Enter new breeding success rate (percentage, non-negative): ";
        while (!(cin >> breedingSuccessRate) || breedingSuccessRate < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a non-negative percentage: ";
        }
    } while (breedingSuccessRate < 0);
}

};

// -----------------------------------------------------------------------------
// Derived Class: Calf
//   Contains calf-specific attributes: breed, DOB, Mother id, Father id.
// -----------------------------------------------------------------------------
class Calf : public Animal {
public:
    string breed;
    string DOB;
    int motherID;
    int fatherID;


    Calf() : Animal() {
        breed = "";
        DOB = "";
        motherID = 0;
        fatherID = 0;
    }
    
    Calf(int id, string name, string gender, int ageYears, string condition, 
         string breed, string DOB, int motherID, int fatherID)
    : Animal(id, name, gender, ageYears, "Calf", condition) {
        this->breed = breed;
        this->DOB = DOB;
        this->motherID = motherID;
        this->fatherID = fatherID;
    }
    
    
    Calf& operator=(const Calf &other) {
        if (this != &other) {
            this->id = other.id;
            this->name = other.name;
            this->gender = other.gender;
            this->ageYears = other.ageYears;
            this->category = other.category;
            this->condition = other.condition;
            this->breed = other.breed;
            this->DOB = other.DOB;
            this->motherID = other.motherID;
            this->fatherID = other.fatherID;
        }
        return *this;
    }
    
    
    
    void displayInfo() {
        Animal::displayInfo();
        cout << " | Breed: " << breed 
             << " | DOB: " << DOB 
             << " | Mother ID: " << motherID 
             << " | Father ID: " << fatherID << endl;
    }
    
    

void updateRecord() {
    cout << "Updating Calf record with ID: " << id << endl;
    cin.ignore();

    // Validate Name
    cout << "Enter new name: ";
    getline(cin, name);
    while (!isValidDescription(name)) {
        cout << "Invalid name. Please use only alphabetic characters and spaces: ";
        getline(cin, name);
    }
    name = toTitleCase(name);

    // Validate Gender
    cout << "Enter new gender: ";
    getline(cin, gender);
    while (!isValidDescription(gender)) {
        cout << "Invalid gender. Please use only alphabetic characters and spaces: ";
        getline(cin, gender);
    }
    gender = toTitleCase(gender);


    cout << "Enter new age in years: ";
    while (!(cin >> ageYears) || ageYears <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter a valid positive number: ";
    }
    cin.ignore();


    cout << "Enter new breed: ";
    getline(cin, breed);
    while (!isValidDescription(breed)) {
        cout << "Invalid breed. Please use only alphabetic characters and spaces: ";
        getline(cin, breed);
    }
    breed = toTitleCase(breed);

 
    cout << "Enter new DOB (YYYY-MM-DD): ";
    while (!(getline(cin, DOB)) || DOB.length() != 10 || DOB[4] != '-' || DOB[7] != '-') {
        cout << "Invalid format! Please enter date in YYYY-MM-DD format: ";
    }


    do {
        cout << "Enter new Mother ID (must be a Cow ID): ";
        while (!(cin >> motherID)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a numeric Cow ID: ";
        }

    } while (motherID == -1);


    do {
        cout << "Enter new Father ID (must be a Bull ID): ";
        while (!(cin >> fatherID)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a numeric Bull ID: ";
        }

    } while (fatherID == -1);
}

};


class MeatAnimal : public Animal {
public:
    float growthRate;
    float weight;
    float targetWeight;
    float requiredGrowthRate;  

    MeatAnimal() : Animal() {
        growthRate = 0.0;
        weight = 0.0;
        targetWeight = 0.0;
        requiredGrowthRate = 0.0;
    }
    
    MeatAnimal(int id, string name, string gender, int ageYears, string condition, 
               float growthRate, float weight, float targetWeight)
    : Animal(id, name, gender, ageYears, "MeatAnimal", condition) {
        this->growthRate = growthRate;
        this->weight = weight;
        this->targetWeight = targetWeight;
        if (growthRate > 0)
            requiredGrowthRate = (targetWeight - weight) / growthRate;
        else
            requiredGrowthRate = 0.0;
    }
    
    void displayInfo() {
        Animal::displayInfo();
        cout << " | Growth Rate: " << growthRate
             << " | Weight: " << weight
             << " | Target Weight: " << targetWeight 
             << " | Required Growth Rate: " << requiredGrowthRate << endl;
    }
    
    
  MeatAnimal& operator=(const MeatAnimal &other) {
        if (this != &other) {
            this->id = other.id;
            this->name = other.name;
            this->gender = other.gender;
            this->ageYears = other.ageYears;
            this->category = other.category;
            this->condition = other.condition;
            this->growthRate = other.growthRate;
            this->weight = other.weight;
            this->targetWeight = other.targetWeight;
            this->requiredGrowthRate = other.requiredGrowthRate;
        }
        return *this;
    }  
    
    
    
void updateRecord() {
    cout << "Updating Meat Animal record with ID: " << id << endl;
    cin.ignore();

    
    cout << "Enter new name: ";
    getline(cin, name);
    while (!isValidDescription(name)) {
        cout << "Invalid name! Please use only alphabetic characters and spaces: ";
        getline(cin, name);
    }
    name = toTitleCase(name);

    
    cout << "Enter new gender (Male/Female): ";
    do {
        getline(cin, gender);
        while (!isValidDescription(gender)) {
            cout << "Invalid gender! Please use only alphabetic characters and spaces: ";
            getline(cin, gender);
        }
        gender = toTitleCase(gender);
    } while (gender != "Male" && gender != "Female");

   
    do {
        cout << "Enter age in years: ";
        while (!(cin >> ageYears) || ageYears <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Age must be a positive number: ";
        }
    } while (ageYears <= 0);

    // Validate Growth Rate
    do {
        cout << "Enter new growth rate (must be non-negative): ";
        while (!(cin >> growthRate) || growthRate < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Growth rate cannot be negative: ";
        }
    }while (growthRate < 0);

    // Validate Current Weight
    do {
        cout << "Enter new current weight (must be non-negative): ";
        while (!(cin >> weight) || weight < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Weight cannot be negative: ";
        }
    }while (weight < 0);

    
    do {
        cout << "Enter new target weight (must be greater than current weight): ";
        while (!(cin >> targetWeight) || targetWeight < weight) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Target weight must be greater than current weight: ";
        }
    }while (targetWeight < weight);

    // Calculate Required Growth Rate
    requiredGrowthRate = (growthRate > 0) ? (targetWeight - weight) / growthRate : 0.0;
}

};


class AnimalManager {
public:
    Cow cows[MAX_ANIMALS];
    Bull bulls[MAX_ANIMALS];
    Calf calves[MAX_ANIMALS];
    MeatAnimal meatAnimals[MAX_ANIMALS];
    int cowCount, bullCount, calfCount, meatAnimalCount;


    AnimalManager() {
        cowCount = bullCount = calfCount = meatAnimalCount = 0;
    }


void loadAllAnimalsFromFiles() {
    ifstream fin;
    int id, age;
    string name, gender, condition;

    // Load Cows
    fin.open("cow_records.txt");
    if (!fin) {
        cout << "Error: Unable to open cow_records.txt" << endl;
        return;
    }
    while (fin >> id >> name >> gender >> age >> condition) {
        float milk;
        string breed, breedingDate, lastHeat, nextHeat;
        if (!(fin >> milk >> breed >> breedingDate >> lastHeat >> nextHeat)) {
            cout << "Error: Invalid data format in cow_records.txt" << endl;
            break;
        }
        cows[cowCount] = Cow(id, name, gender, age, condition, milk, breed, breedingDate, lastHeat);
        cowCount++;
    }
    fin.close();
    fin.clear();

    // Load Bulls
    fin.open("bull_records.txt");
    if (!fin) {
        cout << "Error: Unable to open bull_records.txt" << endl;
        return;
    }
    while (fin >> id >> name >> gender >> age >> condition) {
        float fertility, success;
        if (!(fin >> fertility >> success)) {
            cout << "Error: Invalid data format in bull_records.txt" << endl;
            break;
        }
        bulls[bullCount] = Bull(id, name, gender, age, condition, fertility, success);
        bullCount++;
    }
    fin.close();
    fin.clear();

    // Load Calves
    fin.open("calf_records.txt");
    if (!fin) {
        cout << "Error: Unable to open calf_records.txt" << endl;
        return;
    }
    while (fin >> id >> name >> gender >> age >> condition) {
        string breed, DOB;
        int motherID, fatherID;
        if (!(fin >> breed >> DOB >> motherID >> fatherID)) {
            cout << "Error: Invalid data format in calf_records.txt" << endl;
            break;
        }
        calves[calfCount] = Calf(id, name, gender, age, condition, breed, DOB, motherID, fatherID);
        calfCount++;
    }
    fin.close();
    fin.clear();

    // Load Meat Animals
    fin.open("meatanimal_records.txt");
    if (!fin) {
        cout << "Error: Unable to open meatanimal_records.txt" << endl;
        return;
    }
    while (fin >> id >> name >> gender >> age >> condition) {
        float growth, weight, target, reqGrowth;
        if (!(fin >> growth >> weight >> target >> reqGrowth)) {
            cout << "Error: Invalid data format in meatanimal_records.txt" << endl;
            break;
        }
        meatAnimals[meatAnimalCount] = MeatAnimal(id, name, gender, age, condition, growth, weight, target);
        meatAnimalCount++;
    }
    fin.close();
    fin.clear();
}





bool validateAnimalID(int searchID) {


    for (int i = 0; i < cowCount; i++) {
        if (cows[i].getID() == searchID) return true;
    }


    for (int i = 0; i < bullCount; i++) {
        if (bulls[i].getID() == searchID) return true;
    }
    
    
    for (int i = 0; i < calfCount; i++) {
        if (calves[i].getID() == searchID) return true;
    }



    for (int i = 0; i < meatAnimalCount; i++) {
        if (meatAnimals[i].getID() == searchID) return true;
    }
    return false; 
}


void locateAnimal(int animalId, int hour) {


    if (hour < 1 || hour > 24) {
        cout<<"Error: Invalid hour! Please enter a value between 1 and 24."<<endl;
    }


    string category = "";
    for (int i = 0; i < cowCount; i++) {
        if (cows[i].getID() == animalId) {
            category = "Cow";
            break;
        }
    }
    for (int i = 0; i < bullCount; i++) {
        if (bulls[i].getID() == animalId) {
            category = "Bull";
            break;
        }
    }
    for (int i = 0; i < calfCount; i++) {
        if (calves[i].getID() == animalId) {
            category = "Calf";
            break;
        }
    }
    for (int i = 0; i < meatAnimalCount; i++) {
        if (meatAnimals[i].getID() == animalId) {
            category = "MeatAnimal";
            break;
        }
    }

    if (category == "") {
       cout<<"Error: Animal ID not found!"<<endl;
       return;
    }




    string location;
    if (category == "Cow") {
        if (hour >= 1 && hour <= 7) location = "Cow Shed";
        else if (hour >= 8 && hour <= 14) location = "Pasture";
        else if (hour >= 15 && hour <= 19) location = "Milking Station";
        else location = "Resting Barn";
    } else if (category == "Bull") {
        if (hour >= 1 && hour <= 7) location = "Bull Barn";
        else if (hour >= 8 && hour <= 14) location = "Exercise Field";
        else if (hour >= 15 && hour <= 19) location = "Feeding Area";
        else location = "Resting Area";
    } else if (category == "Calf") {
        if (hour >= 1 && hour <= 7) location = "Calf Shed";
        else if (hour >= 8 && hour <= 14) location = "Play Area";
        else if (hour >= 15 && hour <= 19) location = "Feeding Area";
        else location = "Sleeping Barn";
    } else if (category == "MeatAnimal") {
        if (hour >= 1 && hour <= 7) location = "Growth Shed";
        else if (hour >= 8 && hour <= 14) location = "Feeding Lot";
        else if (hour >= 15 && hour <= 19) location = "Cooling Stall";
        else location = "Processing Zone";
    }

cout<<"Animal ID: " <<animalId<<" | Category: "<<category<< " | Expected Location at Hour "<<hour<<": "<<location<<endl;

}







    int findAnimalByID(int searchID, string type) {
        if (type == "Cow") {
            for (int i = 0; i < cowCount; i++) {
                if (cows[i].getID() == searchID)
                    return i;
            }
        }
        else if (type == "Bull") {
            for (int i = 0; i < bullCount; i++) {
                if (bulls[i].getID() == searchID)
                    return i;
            }
        }
        else if (type == "Calf") {
            for (int i = 0; i < calfCount; i++) {
                if (calves[i].getID() == searchID)
                    return i;
            }
        }
        else if (type == "MeatAnimal") {
            for (int i = 0; i < meatAnimalCount; i++) {
                if (meatAnimals[i].getID() == searchID)
                    return i;
            }
        }
        return -1;
    }










    void addAnimal() {
        clearScreen();
        cout << "\n--- Add New Animal Record ---" << endl;
        cout << "Select Animal Type:" << endl;
        cout << "1. Cow\n2. Bull\n3. Calf\n4. MeatAnimal" << endl;
        string choice;
        do {
            cout << "Enter your choice (1/2/3/4): ";
            cin >> choice;
        } while(choice != "1" && choice != "2" && choice != "3" && choice != "4");
        
int id, age;
string name, gender, condition;



cout << "Enter Animal ID (integer): ";
while (!(cin >> id)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input! Please enter a numeric ID: ";
}
if (validateAnimalID(id)) {
    cout << "Error: Animal ID already exists!" << endl;
    pauseScreen();
    return;
}
cin.ignore();



cout << "Enter Animal Name: ";
getline(cin, name);
while (!isValidDescription(name)) {
    cout << "Invalid name! Please use only alphabetic characters and spaces: ";
    getline(cin, name);
}



cout << "Enter Gender (Male/Female): ";
do {
    getline(cin, gender);
    while (!isValidDescription(gender)) {
        cout << "Invalid gender! Please use only alphabetic characters and spaces: ";
        getline(cin, gender);
    }
    gender = toTitleCase(gender);
} while (gender != "Male" && gender != "Female");




do {
    cout << "Enter age in years: ";
    while (!(cin >> age) || age <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter a valid positive number: ";
    }
} while (age <= 0);
cin.ignore();




do {
    cout << "Enter Condition (Healthy/Sick): ";
    getline(cin, condition);
    condition = toTitleCase(condition);
    if (condition != "Healthy" && condition != "Sick") {
        cout << "Invalid input! Please enter only 'Healthy' or 'Sick': ";
    }
} while (condition != "Healthy" && condition != "Sick");

        

if (choice == "1") {
    if (findAnimalByID(id, "Cow") != -1) {
        cout << "Error: Duplicate ID found in Cow records." << endl;
        pauseScreen();
        return;
    }

    float milkProduction;
    string breed, breedingDate, lastHeatDate;



    do {
        cout << "Enter Milk Production (liters, non-negative): ";
        while (!(cin >> milkProduction) || milkProduction < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Milk production cannot be negative. Please enter a valid number: ";
        }
    } while (milkProduction < 0);
    cin.ignore();




    cout << "Enter Breed: ";
    getline(cin, breed);
    while (!isValidDescription(breed)) {
        cout << "Invalid breed! Please use only alphabetic characters and spaces: ";
        getline(cin, breed);
    }
    breed = toTitleCase(breed);



    cout << "Enter Breeding Date (YYYY-MM-DD): ";
    while (!(getline(cin, breedingDate)) || breedingDate.length() != 10 || breedingDate[4] != '-' || breedingDate[7] != '-') {
        cout << "Invalid format! Please enter date in YYYY-MM-DD format: ";
    }



    cout << "Enter Last Heat Date (YYYY-MM-DD): ";
    while (!(getline(cin, lastHeatDate)) || lastHeatDate.length() != 10 || lastHeatDate[4] != '-' || lastHeatDate[7] != '-') {
        cout << "Invalid format! Please enter date in YYYY-MM-DD format: ";
    }



    Cow newCow(id, name, gender, age, condition, milkProduction, breed, breedingDate, lastHeatDate);
    cows[cowCount] = newCow;
    cowCount++;



    ofstream fout("cow_records.txt", ios::app);
    if (fout.is_open()) {
        fout << id << " " << name << " " << gender << " " << age << " " << condition
             << " " << milkProduction << " " << breed << " " << breedingDate << " " << lastHeatDate 
             << " " << newCow.nextHeatDate << "\n";
        fout.close();
    }
}

        
        
else if (choice == "2") {
    if (findAnimalByID(id, "Bull") != -1) {
        cout << "Error: Duplicate ID found in Bull records." << endl;
        pauseScreen();
        return;
    }

    float fertility, success;


    do {
        cout << "Enter Fertility Rate (percentage, non-negative): ";
        while (!(cin >> fertility) || fertility < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Fertility rate cannot be negative. Please enter a valid number: ";
        }
    } while (fertility < 0);



    do {
        cout << "Enter Breeding Success Rate (percentage, non-negative): ";
        while (!(cin >> success) || success < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Success Rate cannot be negative. Please enter a valid number: ";
        }
    } while (success < 0);


    Bull newBull(id, name, gender, age, condition, fertility, success);
    bulls[bullCount] = newBull;
    bullCount++;



    ofstream fout("bull_records.txt", ios::app);
    if (fout.is_open()) {
        fout << id << " " << name << " " << gender << " " << age << " " << condition
             << " " << fertility << " " << success << "\n";
        fout.close();
    }
}

        
        
        
        
else if (choice == "3") {
    if (findAnimalByID(id, "Calf") != -1) {
        cout << "Error: Duplicate ID found in Calf records." << endl;
        pauseScreen();
        return;
    }

    string breed, DOB;
    int motherID, fatherID;
    cin.ignore();


    cout << "Enter Breed: ";
    getline(cin, breed);
    while (!isValidDescription(breed)) {
        cout << "Invalid breed! Please use only alphabetic characters and spaces: ";
        getline(cin, breed);
    }
    breed = toTitleCase(breed);



    cout << "Enter DOB (YYYY-MM-DD): ";
    while (!(getline(cin, DOB)) || DOB.length() != 10 || DOB[4] != '-' || DOB[7] != '-') {
        cout << "Invalid format! Please enter date in YYYY-MM-DD format: ";
    }



    do {
        cout << "Enter Mother ID (must be a Cow ID): ";
        while (!(cin >> motherID)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a numeric Cow ID: ";
        }

        if (!findAnimalByID(motherID, "Cow")) {
            cout << "Error: Mother ID not found in Cow records! Please enter a valid Cow ID.\n";
            motherID = -1;
        }
    } while (motherID == -1);



    do {
        cout << "Enter Father ID (must be a Bull ID): ";
        while (!(cin >> fatherID)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a numeric Bull ID: ";
        }

        if (!findAnimalByID(fatherID, "Bull")) {
            cout << "Error: Father ID not found in Bull records! Please enter a valid Bull ID.\n";
            fatherID = -1;
        }
    } while (fatherID == -1);



    Calf newCalf(id, name, gender, age, condition, breed, DOB, motherID, fatherID);
    calves[calfCount] = newCalf;
    calfCount++;



    ofstream fout("calf_records.txt", ios::app);
    if (fout.is_open()) {
        fout << id << " " << name << " " << gender << " " << age << " " << condition
             << " " << breed << " " << DOB << " " << motherID << " " << fatherID << "\n";
        fout.close();
    }
}

        
        
        
        
        
else if (choice == "4") {
    if (findAnimalByID(id, "MeatAnimal") != -1) {
        cout << "Error: Duplicate ID found in MeatAnimal records." << endl;
        pauseScreen();
        return;
    }

    float growthRate, weight, targetWeight;



    do {
        cout << "Enter Growth Rate (non-negative): ";
        while (!(cin >> growthRate) || growthRate < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Growth rate cannot be negative. Please enter a valid number: ";
        }
    } while (growthRate < 0);



    do {
        cout << "Enter Current Weight (non-negative): ";
        while (!(cin >> weight) || weight < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Weight cannot be negative. Please enter a valid number: ";
        }
    } while (weight < 0);


    do {
        cout << "Enter Target Weight (must be greater than current weight): ";
        while (!(cin >> targetWeight) || targetWeight <= weight) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Target weight must be greater than current weight. Please enter a valid number: ";
        }
    } while (targetWeight <= weight);



    MeatAnimal newMeat(id, name, gender, age, condition, growthRate, weight, targetWeight);
    meatAnimals[meatAnimalCount] = newMeat;
    meatAnimalCount++;

    
    
    ofstream fout("meatanimal_records.txt", ios::app);
    if (fout.is_open()) {
        float reqGrowth = (growthRate > 0) ? ((targetWeight - weight) / growthRate) : 0.0;
        fout << id << " " << name << " " << gender << " " << age << " " << condition
             << " " << growthRate << " " << weight << " " << targetWeight << " " << reqGrowth << "\n";
        fout.close();
    }
}

        
        cout << "Animal record added successfully!" << endl;
        pauseScreen();
    }

void searchAnimalByID(int id) {
    clearScreen();
    cout << "\n--- Searching for Animal ID: " << id << " ---" << endl;

    bool found = false;

    for (int i = 0; i < cowCount; i++) {
        if (cows[i].getID() == id) {
            cows[i].displayInfo();
            found = true;
        }
    }
    for (int i = 0; i < bullCount; i++) {
        if (bulls[i].getID() == id) {
            bulls[i].displayInfo();
            found = true;
        }
    }
    for (int i = 0; i < calfCount; i++) {
        if (calves[i].getID() == id) {
            calves[i].displayInfo();
            found = true;
        }
    }
    for (int i = 0; i < meatAnimalCount; i++) {
        if (meatAnimals[i].getID() == id) {
            meatAnimals[i].displayInfo();
            found = true;
        }
    }

    if (!found) {
        cout << "Animal ID " << id << " not found in any category!" << endl;
    }

    pauseScreen();
}






    void removeAnimal() {
        clearScreen();
        cout << "\n--- Remove Animal Record ---" << endl;
        cout << "Select Animal Type to remove:" << endl;
        cout << "1. Cow\n2. Bull\n3. Calf\n4. MeatAnimal" << endl;
        string choice;
        do {
            cout << "Enter your choice (1/2/3/4): ";
            cin >> choice;
        } while(choice != "1" && choice != "2" && choice != "3" && choice != "4");
        int id;
cout << "Enter the ID of the animal to remove: ";
while (!(cin >> id)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input! Please enter a numeric ID: ";
}

        
        if (choice == "1") {
            int index = findAnimalByID(id, "Cow");
            if (index == -1) {
                cout << "Animal not found." << endl;
                pauseScreen();
                return;
            }
            
            for (int i = index; i < cowCount - 1; i++) {
                cows[i] = cows[i + 1];
            }
            cowCount--;
        
            ofstream fout("cow_records.txt");
            for (int i = 0; i < cowCount; i++) {
                fout << cows[i].getID() << " " << cows[i].getName() << " " << cows[i].getGender()
                     << " " << cows[i].getAge() << " " << cows[i].getCondition()
                     << " " /* milkProduction */ " " << "\n"; 
            }
            fout.close();
        }
        else if (choice == "2") {
            int index = findAnimalByID(id, "Bull");
            if (index == -1) {
                cout << "Animal not found." << endl;
                pauseScreen();
                return;
            }
            for (int i = index; i < bullCount - 1; i++) {
                bulls[i] = bulls[i + 1];
            }
            bullCount--;
            ofstream fout("bull_records.txt");
            for (int i = 0; i < bullCount; i++) {
                fout << bulls[i].getID() << " " << bulls[i].getName() << " " << bulls[i].getGender()
                     << " " << bulls[i].getAge() << " " << bulls[i].getCondition() << "\n";
            }
            fout.close();
        }
        else if (choice == "3") {
            int index = findAnimalByID(id, "Calf");
            if (index == -1) {
                cout << "Animal not found." << endl;
                pauseScreen();
                return;
            }
            for (int i = index; i < calfCount - 1; i++) {
                calves[i] = calves[i + 1];
            }
            calfCount--;
            ofstream fout("calf_records.txt");
            for (int i = 0; i < calfCount; i++) {
                fout << calves[i].getID() << " " << calves[i].getName() << " " << calves[i].getGender()
                     << " " << calves[i].getAge() << " " << calves[i].getCondition() << "\n";
            }
            fout.close();
        }
        else if (choice == "4") {
            int index = findAnimalByID(id, "MeatAnimal");
            if (index == -1) {
                cout << "Animal not found." << endl;
                pauseScreen();
                return;
            }
            for (int i = index; i < meatAnimalCount - 1; i++) {
                meatAnimals[i] = meatAnimals[i + 1];
            }
            meatAnimalCount--;
            ofstream fout("meatanimal_records.txt");
            for (int i = 0; i < meatAnimalCount; i++) {
                fout << meatAnimals[i].getID() << " " << meatAnimals[i].getName() << " " << meatAnimals[i].getGender()
                     << " " << meatAnimals[i].getAge() << " " << meatAnimals[i].getCondition() << "\n";
            }
            fout.close();
        }
        cout << "Animal record removed successfully!" << endl;
        pauseScreen();
    }




    void updateAnimal() {
        clearScreen();
        cout << "\n--- Update Animal Record ---" << endl;
        cout << "Select Animal Type to update:" << endl;
        cout << "1. Cow\n2. Bull\n3. Calf\n4. MeatAnimal" << endl;
        string choice;
        do {
            cout << "Enter your choice (1/2/3/4): ";
            cin >> choice;
        } while(choice != "1" && choice != "2" && choice != "3" && choice != "4");

int id;
cout << "Enter the ID of the animal to update: ";
while (!(cin >> id)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input! Please enter a numeric ID: ";
}

        
        if (choice == "1") {
            int index = findAnimalByID(id, "Cow");
            if (index == -1) {
                cout << "Animal not found." << endl;
                pauseScreen();
                return;
            }
            cows[index].updateRecord();

            ofstream fout("cow_records.txt");
            for (int i = 0; i < cowCount; i++) {
                fout << cows[i].getID() << " " << cows[i].getName() << " " 
                     << cows[i].getGender() << " " << cows[i].getAge() << " " 
                     << cows[i].getCondition() << " " 
                     /* milkProduction, breed, breedingDate, lastHeatDate, nextHeatDate */
                     << " " << " " << "\n";
            }
            fout.close();
        }
        else if (choice == "2") {
            int index = findAnimalByID(id, "Bull");
            if (index == -1) {
                cout << "Animal not found." << endl;
                pauseScreen();
                return;
            }
            bulls[index].updateRecord();
            ofstream fout("bull_records.txt");
            for (int i = 0; i < bullCount; i++) {
                fout << bulls[i].getID() << " " << bulls[i].getName() << " " 
                     << bulls[i].getGender() << " " << bulls[i].getAge() << " " 
                     << bulls[i].getCondition() << "\n";
            }
            fout.close();
        }
        else if (choice == "3") {
            int index = findAnimalByID(id, "Calf");
            if (index == -1) {
                cout << "Animal not found." << endl;
                pauseScreen();
                return;
            }
            calves[index].updateRecord();
            ofstream fout("calf_records.txt");
            for (int i = 0; i < calfCount; i++) {
                fout << calves[i].getID() << " " << calves[i].getName() << " " 
                     << calves[i].getGender() << " " << calves[i].getAge() << " " 
                     << calves[i].getCondition() << "\n";
            }
            fout.close();
        }
        else if (choice == "4") {
            int index = findAnimalByID(id, "MeatAnimal");
            if (index == -1) {
                cout << "Animal not found." << endl;
                pauseScreen();
                return;
            }
            meatAnimals[index].updateRecord();
            ofstream fout("meatanimal_records.txt");
            for (int i = 0; i < meatAnimalCount; i++) {
                fout << meatAnimals[i].getID() << " " << meatAnimals[i].getName() << " " 
                     << meatAnimals[i].getGender() << " " << meatAnimals[i].getAge() << " " 
                     << meatAnimals[i].getCondition() << "\n";
            }
            fout.close();
        }
        cout << "Animal record updated successfully!" << endl;
        pauseScreen();
    }
    


    void displayAllAnimals() {
        clearScreen();
        cout << "\n--- Display All Animal Records ---" << endl;
        cout << "\nCows:" << endl;
        for (int i = 0; i < cowCount; i++) {
            cows[i].displayInfo();
            cout << endl;
        }
        cout << "\nBulls:" << endl;
        for (int i = 0; i < bullCount; i++) {
            bulls[i].displayInfo();
            cout << endl;
        }
        cout << "\nCalves:" << endl;
        for (int i = 0; i < calfCount; i++) {
            calves[i].displayInfo();
            cout << endl;
        }
        cout << "\nMeat Animals:" << endl;
        for (int i = 0; i < meatAnimalCount; i++) {
            meatAnimals[i].displayInfo();
            cout << endl;
        }
        pauseScreen();
    }
    
 
 
 
    void searchAnimalByType() {
        clearScreen();
        cout << "\n--- Search Animal by Type ---" << endl;
        cout << "Select Animal Type:" << endl;
        cout << "1. Cow\n2. Bull\n3. Calf\n4. MeatAnimal" << endl;
        string choice;
        do {
            cout << "Enter your choice (1/2/3/4): ";
            cin >> choice;
        } while(choice != "1" && choice != "2" && choice != "3" && choice != "4");
        
        if (choice == "1") {
            cout << "\nCows:" << endl;
            for (int i = 0; i < cowCount; i++) {
                cows[i].displayInfo();
                cout << endl;
            }
        }
        else if (choice == "2") {
            cout << "\nBulls:" << endl;
            for (int i = 0; i < bullCount; i++) {
                bulls[i].displayInfo();
                cout << endl;
            }
        }
        else if (choice == "3") {
            cout << "\nCalves:" << endl;
            for (int i = 0; i < calfCount; i++) {
                calves[i].displayInfo();
                cout << endl;
            }
        }
        else if (choice == "4") {
            cout << "\nMeat Animals:" << endl;
            for (int i = 0; i < meatAnimalCount; i++) {
                meatAnimals[i].displayInfo();
                cout << endl;
            }
        }
        pauseScreen();
    }
};  



class Task {
public:
    int animalId;
    string timeSlot;
    string taskType;
    string location;
    string description;
    string completed;  

    Task() {
        animalId = 0;
        timeSlot = "";
        taskType = "";
        location = "";
        description = "";
        completed = "No";
    }

    Task(int animalId, string timeSlot, string taskType, string location, string description, string completed) {
        this->animalId = animalId;
        this->timeSlot = timeSlot;
        this->taskType = taskType;
        this->location = location;
        this->description = description;
        this->completed = completed;
    }
    


    void displayTask() {
        cout << "Animal ID: " << animalId
             << " | Time Slot: " << timeSlot
             << " | Task Type: " << taskType
             << " | Location: " << location
             << " | Description: " << description
             << " | Completed: " << completed << endl;
    }
};



class TaskManager {
public:
	
    Task tasks[MAX_TASKS];
    int taskCount;

    TaskManager() {
        taskCount = 0;
    }
    


    void loadAllTasksFromFile() {
        ifstream fin("tasks.txt");
        if (fin.is_open()) {
            while (fin) {
                int animalId;
                string timeSlot, taskType, location, description, completed;
                if (!(fin >> animalId >> timeSlot >> taskType >> location >> description >> completed))
                    break;
                tasks[taskCount] = Task(animalId, timeSlot, taskType, location, description, completed);
                taskCount++;
            }
            fin.close();
        }
    }
    


    bool validateTaskID(int animalId) {
        for (int i = 0; i < taskCount; i++) {
            if (tasks[i].animalId == animalId)
                return true;
        }
        return false;
    }
    
    AnimalManager am;


void addTask() {
    clearScreen();
    cout << "\n--- Add New Task ---" << endl;
    int animalId;
    string timeSlot, taskType, location, description;
    
am.loadAllAnimalsFromFiles();


    cout << "Enter Animal ID for the task: ";
    while (!(cin >> animalId)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter a numeric ID: ";
    }



    if (!am.validateAnimalID(animalId)) {
        cout << "Error: Animal ID does not exist. Task cannot be assigned!" << endl;
        pauseScreen();
        return;
    }



    if (validateTaskID(animalId)) {
        cout << "Task for this Animal ID already exists. Use update function." << endl;
        pauseScreen();
        return;
    }


    cout << "Enter Time Slot (e.g., 08:00-10:00): ";
    while (!(cin >> timeSlot) || timeSlot.length() != 11 || timeSlot[2] != ':' || timeSlot[5] != '-' || timeSlot[8] != ':') {
        cout << "Invalid format! Please enter the time slot in HH:MM-HH:MM format: ";
    }
    cin.ignore();


    cout << "Enter Task Type (e.g., Feeding): ";
    getline(cin, taskType);
    while (!isValidDescription(taskType)) {
        cout << "Invalid task type! Please use only alphabetic characters: ";
        getline(cin, taskType);
    }



    cout << "Enter Location (e.g., Barn1): ";
    getline(cin, location);
    while (!isValidDescription(location)) {
        cout << "Invalid location! Please use only alphabetic characters: ";
        getline(cin, location);
    }


    cout << "Enter Description: ";
    getline(cin, description);
    while (description.empty()) {
        cout << "Invalid input! Description cannot be empty: ";
        getline(cin, description);
    }



    tasks[taskCount] = Task(animalId, timeSlot, taskType, location, description, "No");
    taskCount++;



    ofstream fout("tasks.txt", ios::app);
    if (fout.is_open()) {
        fout << animalId << " " << timeSlot << " " << taskType << " " 
             << location << " " << description << " " << "No" << "\n";
        fout.close();
    }

    cout << "Task added successfully!" << endl;
    pauseScreen();
}

    

 void removeTask() {
    clearScreen();
    cout << "\n--- Remove Task ---" << endl;
    int animalId;

am.loadAllAnimalsFromFiles();


    cout << "Enter Animal ID for the task to remove: ";
    while (!(cin >> animalId)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter a numeric ID: ";
    }


    if (!am.validateAnimalID(animalId)) {
        cout << "Error: Animal ID does not exist. Cannot remove task!" << endl;
        pauseScreen();
        return;
    }


    if (taskCount == 0) {
        cout << "No tasks available to remove." << endl;
        pauseScreen();
        return;
    }


    int index = -1;
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].animalId == animalId) {
            index = i;
            break;
        }
    }


    if (index == -1) {
        cout << "Task not found." << endl;
        pauseScreen();
        return;
    }



    for (int i = index; i < taskCount - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    taskCount--;


    tasks[taskCount] = Task();


    ofstream fout("tasks.txt");
    if (fout.is_open()) {
        for (int i = 0; i < taskCount; i++) {
            fout << tasks[i].animalId << " " << tasks[i].timeSlot << " "
                 << tasks[i].taskType << " " << tasks[i].location << " "
                 << tasks[i].description << " " << tasks[i].completed << "\n";
        }
        fout.close();
    }

    cout << "Task removed successfully!" << endl;
    pauseScreen();
}





void updateTaskStatus() {
    clearScreen();
    cout << "\n--- Update Task Status ---" << endl;
    int animalId;


    cout << "Enter Animal ID for the task: ";
    while (!(cin >> animalId)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter a numeric ID: ";
    }


    int index = -1;
    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].animalId == animalId) {
            index = i;
            break;
        }
    }


    if (index == -1) {
        cout << "Task not found." << endl;
        pauseScreen();
        return;
    }


    cout << "Current status: " << tasks[index].completed << endl;


    string status;
    do {
        cout << "Mark as completed? (Yes/No): ";
        cin.ignore();
        getline(cin, status);
        status = toTitleCase(status);
        if (status != "Yes" && status != "No") {
            cout << "Invalid input! Please enter 'Yes' or 'No': ";
        }
    } while (status != "Yes" && status != "No");


    tasks[index].completed = status;


    ofstream fout("tasks.txt");
    if (fout.is_open()) {
        for (int i = 0; i < taskCount; i++) {
            fout << tasks[i].animalId << " " << tasks[i].timeSlot << " "
                 << tasks[i].taskType << " " << tasks[i].location << " "
                 << tasks[i].description << " " << tasks[i].completed << "\n";
        }
        fout.close();
    }

    cout << "Task status updated successfully!" << endl;
    pauseScreen();
}


    void displayAllTasks() {
        clearScreen();
        cout << "\n--- All Scheduled Tasks ---" << endl;
        for (int i = 0; i < taskCount; i++) {
            tasks[i].displayTask();
        }
        pauseScreen();
    }
};  





class FinancialRecord {
public:
    string type;     
    string category; 
    float amount;
    string date;     
    
    FinancialRecord() {
        type = "";
        category = "";
        amount = 0.0;
        date = "";
    }
    
    FinancialRecord(string type, string category, float amount, string date) {
        this->type = type;
        this->category = category;
        this->amount = amount;
        this->date = date;
    }
    

    void displayRecord() {
        cout << "Type: " << type << " | Category: " << category 
             << " | Amount: " << amount << " | Date: " << date << endl;
    }
};



class FinancialManager {
public:
    FinancialRecord records[MAX_FINREC];
    int recordCount;
    
    FinancialManager() {
        recordCount = 0;
    }
    

    void loadFinancialRecords() {
        ifstream fin("financial_records.txt");
        if (fin.is_open()){
            while(fin) {
                string type, category, date;
                float amount;
                if (!(fin >> type >> category >> amount >> date))
                    break;
                records[recordCount] = FinancialRecord(type, category, amount, date);
                recordCount++;
            }
            fin.close();
        }
    }
    

 void recordExpense() {
    clearScreen();
    cout << "\n--- Record New Expense ---" << endl;
    string category, date;
    float expense;


    cout << "Enter Expense Category (e.g., Feed, Veterinary): ";
    do {
        cin.ignore();
        getline(cin, category);
        category = toTitleCase(category);
        if (category != "Feed" && category != "Veterinary") {
            cout << "Invalid input! Please enter only 'Feed' or 'Veterinary': ";
        }
    } while (category != "Feed" && category != "Veterinary");


    do {
        cout << "Enter Expense Amount (non-negative): ";
        while (!(cin >> expense) || expense < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Expense cannot be negative. Please enter a valid number: ";
        }
    } while (expense < 0);


    cout << "Enter Date (YYYY-MM-DD): ";
    while (!(cin >> date) || date.length() != 10 || date[4] != '-' || date[7] != '-') {
        cout << "Invalid format! Please enter date in YYYY-MM-DD format: ";
    }


    records[recordCount] = FinancialRecord("Expense", category, expense, date);
    recordCount++;

    ofstream fout("financial_records.txt", ios::app);
    if (fout.is_open()) {
        fout << "Expense " << category << " " << expense << " " << date << "\n";
        fout.close();
    }

    cout << "Expense recorded successfully!" << endl;
    pauseScreen();
}




 void recordIncome() {
    clearScreen();
    cout << "\n--- Record New Income ---" << endl;
    string category, date;
    float income;

   
    cout << "Enter Income Category (e.g., Milk, Meat, Stud): ";
    do {
        cin.ignore();
        getline(cin, category);
        category = toTitleCase(category);
        if (category != "Milk" && category != "Meat" && category != "Stud") {
            cout << "Invalid input! Please enter only 'Milk', 'Meat', or 'Stud': ";
        }
    } while (category != "Milk" && category != "Meat" && category != "Stud");



    do {
        cout << "Enter Income Amount (non-negative): ";
        while (!(cin >> income) || income < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Income cannot be negative. Please enter a valid number: ";
        }
    } while (income < 0);


    cout << "Enter Date (YYYY-MM-DD): ";
    while (!(cin >> date) || date.length() != 10 || date[4] != '-' || date[7] != '-') {
        cout << "Invalid format! Please enter date in YYYY-MM-DD format: ";
    }


    records[recordCount] = FinancialRecord("Income", category, income, date);
    recordCount++;

    ofstream fout("financial_records.txt", ios::app);
    if (fout.is_open()) {
        fout << "Income " << category << " " << income << " " << date << "\n";
        fout.close();
    }

    cout << "Income recorded successfully!" << endl;
    pauseScreen();
}

    
    
    


void calculateProfit() {
    clearScreen();
    cout << "\n--- Profit Calculation ---" << endl;
    float totalIncome = 0.0, totalExpense = 0.0;

    for (int i = 0; i < recordCount; i++) {
        if (records[i].type == "Income")
            totalIncome += records[i].amount;
        else if (records[i].type == "Expense")
            totalExpense += records[i].amount;
    }

    float profit = totalIncome - totalExpense;

    cout << "Total Income: " << totalIncome << endl;
    cout << "Total Expenses: " << totalExpense << endl;
    cout << "Net Profit: " << profit << endl;


    if (profit < 0) {
        cout << "\n WARNING: Your farm is running at a loss! \n";
        cout << "Consider reviewing expenses and exploring ways to increase income." << endl;
    } else if (profit == 0) {
        cout << "\nALERT: Break-even point reached! \n";
        cout << "Your income and expenses are equal. Aim for more profitability!" << endl;
    } else {
        cout << "\n GOOD NEWS: Your farm is profitable! " << endl;
    }

    pauseScreen();
}

    


    void generateFinancialReport() {
        clearScreen();
        cout << "\n--- Financial Report ---" << endl;
        for (int i = 0; i < recordCount; i++) {
            records[i].displayRecord();
        }
        pauseScreen();
    }
};




void ownerMenu(AnimalManager &am, TaskManager &tm, FinancialManager &fm) {
    string option;
    do {
        clearScreen();
        cout << "\n--- Owner Menu ---" << endl;
        cout << "1. Add New Animal Record" << endl;
        cout << "2. Remove Animal Record" << endl;
        cout << "3. Update Animal Record" << endl;
        cout << "4. Display All Animals" << endl;
        cout << "5. Search Animal by ID" << endl;
        cout << "6. Search Animal by Type" << endl;
        cout << "7. Manage Tasks" << endl;
        cout << "8. Financial Management" << endl;
        cout << "9. Locate Animal by ID & Time" << endl;
        cout << "10. Logout" << endl;
        cout << "Enter your choice (e.g. 1,2 ..): ";
        cin >> option;
        if (option == "1") { 
            am.addAnimal();
        }
        else if (option == "2") {
            am.removeAnimal();
        }
        else if (option == "3") {
            am.updateAnimal();
        }
        else if (option == "4") {
            am.displayAllAnimals();
        }
        else if (option == "5") {
        	int anid;
        cout << "Enter Animal ID to search: ";
        cin >> anid;
        if (!am.validateAnimalID(anid)) {
        cout << "Error: Animal ID does not exist" << endl;
        pauseScreen();
        return;
    }
    
            am.searchAnimalByID(anid);
        }
        else if (option == "6") {
            am.searchAnimalByType();
        }
        else if (option == "7") {
            string taskOption;
            do {
                clearScreen();
                cout << "\n--- Task Management ---" << endl;
                cout << "1. Add Task" << endl;
                cout << "2. Remove Task" << endl;
                cout << "3. Update Task Status" << endl;
                cout << "4. Display All Tasks" << endl;
                cout << "5. Return to Owner Menu" << endl;
                cout << "Enter your choice: ";
                cin >> taskOption;
                if (taskOption == "1"){
                am.loadAllAnimalsFromFiles();
                tm.addTask();	
				}
                    
                else if (taskOption == "2"){
                	
                	am.loadAllAnimalsFromFiles();
                    tm.removeTask();
				}
                
                else if (taskOption == "3")
                    tm.updateTaskStatus();
                else if (taskOption == "4")
                    tm.displayAllTasks();
            } while(taskOption != "5");
        }
        else if (option == "8") {
            string finOption;
            do {
                clearScreen();
                cout << "\n--- Financial Management ---" << endl;
                cout << "1. Record Expense" << endl;
                cout << "2. Record Income" << endl;
                cout << "3. Calculate Profit" << endl;
                cout << "4. Generate Financial Report" << endl;
                cout << "5. Return to Owner Menu" << endl;
                cout << "Enter your choice: ";
                cin >> finOption;
                if (finOption == "1")
                    fm.recordExpense();
                else if (finOption == "2")
                    fm.recordIncome();
                else if (finOption == "3")
                    fm.calculateProfit();
                else if (finOption == "4")
                    fm.generateFinancialReport();
            } while (finOption != "5");
        }
        
        
    else if (option == "9") { 
    int animalId, hour;
    cout << "Enter Animal ID to locate: ";
    cin >> animalId;
    do {
    cout << "Enter Current Hour (1-24): ";
    cin >> hour;


    if (cin.fail()) {
        cin.clear();  
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  
        cout << "Invalid input! Please enter a numeric hour between 1 and 24.\n";
        hour = -1;  
    }
} while (hour < 1 || hour > 24);
    am.locateAnimal(animalId, hour); 
    pauseScreen();
}
        
    } while(option != "10");
}



void workerMenu(AnimalManager &am, TaskManager &tm) {
    string option;
    do {
        clearScreen();
        cout << "\n--- Worker Menu ---" << endl;
        cout << "1. View Assigned Tasks" << endl;
        cout << "2. Find Animal by ID" << endl;
        cout << "3. Locate Animal by ID & Time" << endl;

        cout << "4. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> option;
        if (option == "1") {
            tm.displayAllTasks();
        }
        else if (option == "2") {
            cout << "Enter Animal ID to find: ";
            int id;
            cin >> id;
            int index;
            index = am.findAnimalByID(id, "Cow");
            if (index != -1) {
                am.cows[index].displayInfo();
                cout << endl;
            }
            index = am.findAnimalByID(id, "Bull");
            if (index != -1) {
                am.bulls[index].displayInfo();
                cout << endl;
            }
            index = am.findAnimalByID(id, "Calf");
            if (index != -1) {
                am.calves[index].displayInfo();
                cout << endl;
            }
            index = am.findAnimalByID(id, "MeatAnimal");
            if (index != -1) {
                am.meatAnimals[index].displayInfo();
                cout << endl;
            }
            pauseScreen();
        }
        
        else if (option == "3") { 
    int animalId, hour;
    cout << "Enter Animal ID to locate: ";
    cin >> animalId;
    cout << "Enter Current Hour (1-24): ";
    cin >> hour;
   am.locateAnimal(animalId, hour);
    pauseScreen();
}

        
        
    } while(option != "4");
}


void vetDoctorMenu(AnimalManager &am) {
    string option;
    do {
        clearScreen();
        cout << "\n--- Vet Doctor Menu ---" << endl;
        cout << "1. View Sick Animals" << endl;
        cout << "2. Update Health Record (Cow only)" << endl;
        cout << "3. Locate Animal by ID & Time" << endl;

        cout << "4. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> option;
        if (option == "1") {
            clearScreen();
            cout << "\n--- Sick Animals ---" << endl;
            for (int i = 0; i < am.cowCount; i++) {
                if (am.cows[i].getCondition() == "Sick")
                    am.cows[i].displayInfo();
            }
            for (int i = 0; i < am.bullCount; i++) {
                if (am.bulls[i].getCondition() == "Sick")
                    am.bulls[i].displayInfo();
            }
            for (int i = 0; i < am.calfCount; i++) {
                if (am.calves[i].getCondition() == "Sick")
                    am.calves[i].displayInfo();
            }
            for (int i = 0; i < am.meatAnimalCount; i++) {
                if (am.meatAnimals[i].getCondition() == "Sick")
                    am.meatAnimals[i].displayInfo();
            }
            pauseScreen();
        }
        
        
else if (option == "2") {
    clearScreen();
    cout << "\n--- Update Animal Health Details ---" << endl;

    cout << "Select Animal Type to update health:" << endl;
    cout << "1. Cow\n2. Bull\n3. Calf\n4. Meat Animal" << endl;
    string choice;
    do {
        cout << "Enter your choice (1/2/3/4): ";
        cin >> choice;
    } while(choice != "1" && choice != "2" && choice != "3" && choice != "4");

  int id;
cout << "Enter Animal ID to update health: ";
while (!(cin >> id)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input! Please enter a numeric ID: ";
}


    string category = (choice == "1") ? "Cow" : (choice == "2") ? "Bull" : (choice == "3") ? "Calf" : "MeatAnimal";
    int index = am.findAnimalByID(id, category);

    if (index != -1) {
        string newCondition;
        do {
            cout << "Enter updated condition (Healthy/Sick): ";
            cin >> newCondition;
            newCondition = toTitleCase(newCondition);
            if (newCondition != "Healthy" && newCondition != "Sick") {
                cout << "Invalid input! Please enter only 'Healthy' or 'Sick'." << endl;
            }
        } while (newCondition != "Healthy" && newCondition != "Sick");

        if (category == "Cow") {
            am.cows[index].setCondition(newCondition);
        } else if (category == "Bull") {
            am.bulls[index].setCondition(newCondition);
        } else if (category == "Calf") {
            am.calves[index].setCondition(newCondition);
        } else if (category == "MeatAnimal") {
            am.meatAnimals[index].setCondition(newCondition);
        }


        string filename = (category == "Cow") ? "cow_records.txt" :
                          (category == "Bull") ? "bull_records.txt" :
                          (category == "Calf") ? "calf_records.txt" : "meatanimal_records.txt";
        
        ofstream fout(filename.c_str());
        if (fout.is_open()) {
            if (category == "Cow") {
                for (int i = 0; i < am.cowCount; i++) {
                    fout << am.cows[i].getID() << " " << am.cows[i].getName() << " "
                         << am.cows[i].getGender() << " " << am.cows[i].getAge() << " "
                         << am.cows[i].getCondition() << "\n";
                }
            } else if (category == "Bull") {
                for (int i = 0; i < am.bullCount; i++) {
                    fout << am.bulls[i].getID() << " " << am.bulls[i].getName() << " "
                         << am.bulls[i].getGender() << " " << am.bulls[i].getAge() << " "
                         << am.bulls[i].getCondition() << "\n";
                }
            } else if (category == "Calf") {
                for (int i = 0; i < am.calfCount; i++) {
                    fout << am.calves[i].getID() << " " << am.calves[i].getName() << " "
                         << am.calves[i].getGender() << " " << am.calves[i].getAge() << " "
                         << am.calves[i].getCondition() << "\n";
                }
            } else if (category == "MeatAnimal") {
                for (int i = 0; i < am.meatAnimalCount; i++) {
                    fout << am.meatAnimals[i].getID() << " " << am.meatAnimals[i].getName() << " "
                         << am.meatAnimals[i].getGender() << " " << am.meatAnimals[i].getAge() << " "
                         << am.meatAnimals[i].getCondition() << "\n";
                }
            }
            fout.close();
        }
        cout << category << " health updated successfully." << endl;
    } else {
        cout << "Error: Animal ID not found in " << category << " records." << endl;
    }

    pauseScreen();
}

        
        else if (option == "3") { 
        
        
    int animalId, hour;



cout << "Enter Animal ID to locate: ";
while (!(cin >> animalId)) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input! Please enter a numeric Animal ID: ";
}


cout << "Enter Current Hour (1-24): ";
while (!(cin >> hour) || hour < 1 || hour > 24) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input! Please enter an hour between 1 and 24: ";
}
    am.locateAnimal(animalId, hour);
    pauseScreen();
}

        
        
    } while(option != "4");
}


int main() {


    srand(time(0));
    

    LoginManager loginMgr;
    string initialOption;
    
    do {
    clearScreen();
    cout << "\n===========================================================\n";
    cout << "         WELCOME TO THE CATTLE FARM MANAGEMENT SYSTEM      \n";
    cout << "===========================================================\n";
    cout << "-> A comprehensive system for managing your farm records <-\n\n";
    
    cout << "We hope you're having a great day!" << endl;
    cout << "Please select an option to proceed:\n";
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cout << "3. Exit" << endl;
    cout << "-----------------------------------------------------------" << endl;
    cout << "Enter your choice: ";
    cin >> initialOption;
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    if (initialOption == "1") {
        loginMgr.login();
        break;
    }
    else if (initialOption == "2") {
        loginMgr.registerUser();
    }
    else if (initialOption == "3") {
    	clearScreen();
        cout << "\nThank you for visiting the Cattle Farm Management System!" << endl;
        cout << "We hope to see you again soon! Have a wonderful day! " << endl;
        cout<<endl << "Happy Farming! :) " << endl;
        exit(0);
    }
    else {
        cout << "\nInvalid option! Please enter 1, 2, or 3.\n";
        pauseScreen();
    }
} while(initialOption != "1");
    

    AnimalManager animalMgr;
    TaskManager taskMgr;
    FinancialManager finMgr;
    

    animalMgr.loadAllAnimalsFromFiles();
    taskMgr.loadAllTasksFromFile();
    finMgr.loadFinancialRecords();
    


    if (loginMgr.getRole() == "Owner") {
        ownerMenu(animalMgr, taskMgr, finMgr);
    }
    
    else if (loginMgr.getRole() == "Worker") {
        workerMenu(animalMgr, taskMgr);
    }
    
    else if (loginMgr.getRole() == "Vetdoctor") {
        vetDoctorMenu(animalMgr);
    }
    
    clearScreen();
    cout << "Thank you for using the Cattle Farm Management System. Have a great day!" << endl;
    return 0;
}



