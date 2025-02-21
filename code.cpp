#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <ctime>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// --- Telecom Directory (Existing Class) ---
class tele {
private:
    char name[50];
    char address[100];
    char email[50];
    int pincode;

public:
    char contact[11];
    void addData();
    void displayData() const;
    void displayAll();
    bool searchByContact(const char* searchContact) const;
    bool checkDuplicateContact(const char* contactToCheck) const;
    void modifyData(const tele& newData);
};

// --- Telecom Directory Functions ---
void tele::addData() {
    cout << "Enter the following details to add data to the directory:" << endl;
    cout << "Enter name: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "Enter address: ";
    cin.getline(address, 100);
    cout << "Enter email: ";
    cin.getline(email, 50);
    cout << "Enter phone number: ";
    cin.getline(contact, 11);
    cout << "Enter pincode: ";
    cin >> pincode;
    cin.ignore();
}

void tele::displayData() const {
    cout << "\nDetails:";
    cout << "\nName: " << name;
    cout << "\nAddress: " << address;
    cout << "\nEmail: " << email;
    cout << "\nContact: " << contact;
    cout << "\nPincode: " << pincode << endl;
}
// returns true or false based on whether contact matches the search contact for search operation
bool tele::searchByContact(const char* searchContact) const {
    return strcmp(contact, searchContact) == 0;
}

// checks for duplicate contact numbers

bool tele::checkDuplicateContact(const char* contactToCheck) const {
    return strcmp(contact, contactToCheck) == 0;
}

void tele::modifyData(const tele& newData) {
    strcpy(name, newData.name);
    strcpy(address, newData.address);
    strcpy(email, newData.email);
    strcpy(contact, newData.contact);
    pincode = newData.pincode;
}

// --- File Operations for Directory ---
// function used for writing into file and checking for duplicate contacts
void write_Data() {
    tele t;
    ifstream infile("telephone.dat", ios::binary);

    if (!infile) {
        cout << "File could not be opened. Press any key to exit..." << endl;
        cin.ignore();
        cin.get();
        return;
    }

    // Collect data from user
    t.addData();

    // Check for duplicate contact numbers
    tele temp;
    bool duplicate = false;
    while (infile.read(reinterpret_cast<char*>(&temp), sizeof(tele))) {
        if (temp.checkDuplicateContact(t.contact)) {
            duplicate = true;
            break;
        }
    }
    infile.close();

    if (duplicate) {
        cout << "Contact number already exists! Cannot add duplicate contact." << endl;
        cin.ignore();
        cin.get();
        return;
    }

    // If no duplicate, write data to file
    ofstream outfile("telephone.dat", ios::binary | ios::app);
    if (!outfile) {
        cout << "File could not be opened. Press any key to exit..." << endl;
        cin.ignore();
        cin.get();
        return;
    }

    t.displayData();
    outfile.write(reinterpret_cast<char*>(&t), sizeof(tele));

    if (!outfile) {
        cerr << "Error writing to file. Press any key to exit..." << endl;
        cin.ignore();
        cin.get();
        return;
    }

    outfile.close();
    cout << "\nRecord has been created" << endl;
    cin.ignore();
    cin.get();
}

void tele::displayAll() {
    tele t;
    ifstream infile("telephone.dat", ios::binary);
    if (!infile) {
        cout << "File could not be opened." << endl;
        return;
    }

    const int widthName = 20;
    const int widthAddress = 30;
    const int widthEmail = 30;
    const int widthPhone = 20;
    const int widthPincode = 10;

    cout << left << setw(widthName) << "Name"
         << setw(widthAddress) << "Address"
         << setw(widthEmail) << "Email"
         << setw(widthPhone) << "Phone"
         << setw(widthPincode) << "Pincode"
         << endl;
    cout << string(widthName + widthAddress + widthEmail + widthPhone + widthPincode, '-') << endl;

    while (infile.read(reinterpret_cast<char*>(&t), sizeof(tele))) {
        cout << left << setw(widthName) << t.name
             << setw(widthAddress) << t.address
             << setw(widthEmail) << t.email
             << setw(widthPhone) << t.contact
             << setw(widthPincode) << t.pincode
             << endl;
    }

    infile.close();
}

void searchData() {
    tele t;
    ifstream infile("telephone.dat", ios::binary);
    if (!infile) {
        cout << "File could not be opened. Press any key to exit..." << endl;
        cin.ignore();
        cin.get();
        return;
    }
    char searchContact[11];
    cout << "Enter the contact number to search: ";
    cin.ignore();
    cin.getline(searchContact, 11);
    bool found = false;
    while (infile.read(reinterpret_cast<char*>(&t), sizeof(tele))) {
        if (t.searchByContact(searchContact)) {
            t.displayData();
            found = true;
            break;
        }
    }
    infile.close();
    if (!found) {
        cout << "Record not found." << endl;
    }
    cin.ignore();
    cin.get();
}

void modify_Data() {
    tele t, newData;
    fstream file("telephone.dat", ios::binary | ios::in | ios::out);
    if (!file) {
        cerr << "File could not be opened." << endl;
        return;
    }

    char searchContact[11];
    cout << "Enter the contact number to modify: ";
    cin.ignore();
    cin.getline(searchContact, 11);

    bool found = false;
    streampos pos;
    while (file.read(reinterpret_cast<char*>(&t), sizeof(tele))) {
        if (t.searchByContact(searchContact)) {
            cout << "Current record:\n";
            t.displayData();
            cout << "Enter new details:\n";
            newData.addData();

            pos = file.tellg();
            file.seekp(pos - static_cast<streamoff>(sizeof(tele)));
            t.modifyData(newData);
            file.write(reinterpret_cast<char*>(&t), sizeof(tele));
            cout << "Record updated successfully." << endl;
            found = true;
            break;
        }
    }

    file.close();

    if (!found) {
        cout << "Record not found." << endl;
    }
}

void delete_Data() {
    tele t;
    ifstream infile("telephone.dat", ios::binary);
    if (!infile) {
        cout << "File could not be opened." << endl;
        return;
    }
    ofstream outfile("temp.dat", ios::binary);
    if (!outfile) {
        cout << "Temporary file could not be created." << endl;
        return;
    }
    char searchContact[11];
    cout << "Enter the contact number to delete: ";
    cin.ignore();
    cin.getline(searchContact, 11);
    bool found = false;
    while (infile.read(reinterpret_cast<char*>(&t), sizeof(tele))) {
        if (!t.searchByContact(searchContact)) {
            outfile.write(reinterpret_cast<char*>(&t), sizeof(tele));
        } else {
            found = true;
        }
    }
    infile.close();
    outfile.close();
    remove("telephone.dat");
    rename("temp.dat", "telephone.dat");
    if (found) {
        cout << "Record deleted successfully." << endl;
    } else {
        cout << "Record not found." << endl;
    }
}

// --- Call Management System ---
class CallManagementSystem {
public:
    void routeCall(string from, string to) {
        cout << "Routing call from " << from << " to " << to << endl;
        logCall(from, to);
    }

    void forwardCall(string from, string to) {
        cout << "Forwarding call from " << from << " to " << to << endl;
        logCall(from, to);
    }

    void logCall(string from, string to) {
        ofstream outfile("call_logs.txt", ios::app);
        if (!outfile) {
            cout << "Error opening call log file." << endl;
            return;
        }

        time_t now = time(0);
        char* dt = ctime(&now);

        outfile << "Call from: " << from << " to: " << to << " at " << dt;
        outfile.close();
    }

    void displayCallLogs() {
        ifstream infile("call_logs.txt");
        if (!infile) {
            cout << "No call logs found." << endl;
            return;
        }

        string log;
        while (getline(infile, log)) {
            cout << log << endl;
        }

        infile.close();
    }
};

// --- Billing System ---
class BillingSystem {
private:
    map<string, double> charges; // Stores call charges based on phone number

public:
    void chargeCall(string number, double duration) {
        double ratePerMinute = 0.05; // Example rate per minute
        double charge = duration * ratePerMinute;
        charges[number] += charge;

        cout << "Charged $" << charge << " for " << duration << " minutes call to " << number << endl;
    }

    void processPayment(string number, double amount) {
        if (charges.find(number) != charges.end()) {
            charges[number] -= amount;
            cout << "Payment of $" << amount <<" processed for " << number << endl;
        } else {
            cout << "No outstanding charges for " << number << endl;
        }
    }

    void displayCharges() {
        cout << "Billing Summary:" << endl;
        for (const auto& entry : charges) {
            cout << "Phone number: " << entry.first << ", Outstanding balance: $" << entry.second << endl;
        }
    }
};

// --- Telecom Network Optimization ---
class TelecomNetworkOptimization {
public:
    void optimizeNetwork() {
        cout << "Optimizing network traffic routing..." << endl;
        vector<string> paths = {"Path A", "Path B", "Path C"};
        cout << "Before optimization: ";
        for (const auto& path : paths) {
            cout << path << " ";
        }
        cout << endl;

    std::sort(paths.begin(), paths.end());
        cout << "After optimization: ";
        for (const auto& path : paths) {
            cout << path << " ";
        }
        cout << endl;
    }
};

// --- Main Program ---
void displayMainMenu();
void manageCalls(CallManagementSystem& cms);
void manageBilling(BillingSystem& bs);
void optimizeNetwork(TelecomNetworkOptimization& tno);

int main() {
    CallManagementSystem cms;
    BillingSystem bs;
    TelecomNetworkOptimization tno;
    tele directory;

    int choice;
    do {
        displayMainMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                manageCalls(cms);
                break;
            case 2:
                manageBilling(bs);
                break;
            case 3:
                tno.optimizeNetwork();
                break;
            case 4:
                write_Data();
                break;
            case 5:
                directory.displayAll();
                break;
            case 6:
                searchData();
                break;
            case 7:
                modify_Data();
                break;
            case 8:
                delete_Data();
                break;
            case 9:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    } while (choice != 9);

    return 0;
}

void displayMainMenu() {
    cout << "\nTelecom Management System Menu:\n";
    cout << "1. Manage Calls\n";
    cout << "2. Manage Billing\n";
    cout << "3. Optimize Network\n";
    cout << "4. Add Entry to Directory\n";
    cout << "5. Display All Entries\n";
    cout << "6. Search Entry\n";
    cout << "7. Modify Entry\n";
    cout << "8. Delete Entry\n";
    cout << "9. Exit\n";
    cout << "Enter your choice: ";
}

void manageCalls(CallManagementSystem& cms) {
    int callChoice;
    string from, to;
    cout << "1. Route Call\n";
    cout << "2. Forward Call\n";
    cout << "3. View Call Logs\n";
    cout << "Enter your choice: ";
    cin >> callChoice;

    switch (callChoice) {
        case 1:
            cout << "Enter source number: ";
            cin >> from;
            cout << "Enter destination number: ";
            cin >> to;
            cms.routeCall(from, to);
            break;
        case 2:
            cout << "Enter source number: ";
            cin >> from;
            cout << "Enter destination number: ";
            cin >> to;
            cms.forwardCall(from, to);
            break;
        case 3:
            cms.displayCallLogs();
            break;
        default:
            cout << "Invalid choice." << endl;
    }
}

void manageBilling(BillingSystem& bs) {
    int billingChoice;
    string number;
    double duration, amount;
    cout << "1. Charge Call\n";
    cout << "2. Process Payment\n";
    cout << "3. View Charges\n";
    cout << "Enter your choice: ";
    cin >> billingChoice;

    switch (billingChoice) {
        case 1:
            cout << "Enter phone number: ";
            cin >> number;
            cout << "Enter call duration (minutes): ";
            cin >> duration;
            bs.chargeCall(number, duration);
            break;
        case 2:
            cout << "Enter phone number: ";
            cin >> number;
            cout << "Enter payment amount: ";
            cin >> amount;
            bs.processPayment(number, amount);
            break;
        case 3:
            bs.displayCharges();
            break;
        default:
            cout << "Invalid choice." << endl;
    }
}

void optimizeNetwork(TelecomNetworkOptimization& tno) {
    tno.optimizeNetwork();
}
