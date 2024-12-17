#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

struct Account {
    string name;
    int accountNumber;
    int age;
    double balance;

    Account(string n, int accNum, int a, double bal)
        : name(n), accountNumber(accNum), age(a), balance(bal) {}
};

const double MIN_BALANCE = 1000.0;
vector<Account> accounts;

// Function to save accounts to a file
void saveAccountsToFile() {
    ofstream outFile("accounts.dat", ios::binary);
    if (!outFile) {
        cout << "Error saving accounts to file." << endl;
        return;
    }
    for (const auto& account : accounts) {
        outFile.write(reinterpret_cast<const char*>(&account), sizeof(Account));
    }
    outFile.close();
}

// Function to load accounts from a file
void loadAccountsFromFile() {
    ifstream inFile("accounts.dat", ios::binary);
    if (!inFile) {
        return; 
    }
    Account account{"", 0, 0, 0.0};
    while (inFile.read(reinterpret_cast<char*>(&account), sizeof(Account))) {
        accounts.push_back(account);
    }
    inFile.close();
}

void createAccount() {
    if (accounts.size() >= 5) {
        cout << "Maximum account limit reached. Cannot create more accounts." << endl;
        return;
    }

    string name;
    int accountNumber, age;
    double initialBalance;

    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Account Number: ";
    cin >> accountNumber;
    cout << "Enter Age: ";
    cin >> age;
    cout << "Enter Initial Balance (Minimum Rs. 1000): ";
    cin >> initialBalance;

    if (initialBalance < MIN_BALANCE) {
        cout << "Initial balance must be at least Rs. 1000." << endl;
        return;
    }

    accounts.push_back(Account(name, accountNumber, age, initialBalance));
    saveAccountsToFile();
    cout << "Account created successfully!" << endl;
}

Account* findAccount(int accountNumber) {
    for (auto& account : accounts) {
        if (account.accountNumber == accountNumber) {
            return &account;
        }
    }
    return nullptr;
}

void depositMoney() {
    int accountNumber;
    double amount;

    cout << "Enter Account Number: ";
    cin >> accountNumber;

    Account* account = findAccount(accountNumber);
    if (account) {
        cout << "Enter amount to deposit: ";
        cin >> amount;
        if (amount > 0) {
            account->balance += amount;
            saveAccountsToFile();
            cout << "Deposit successful! New balance: Rs. " << account->balance << endl;
        } else {
            cout << "Invalid amount." << endl;
        }
    } else {
        cout << "Account not found." << endl;
    }
}

void withdrawMoney() {
    int accountNumber;
    double amount;

    cout << "Enter Account Number: ";
    cin >> accountNumber;

    Account* account = findAccount(accountNumber);
    if (account) {
        cout << "Enter amount to withdraw: ";
        cin >> amount;
        if (amount > 0 && (account->balance - amount) >= MIN_BALANCE) {
            account->balance -= amount;
            saveAccountsToFile();
            cout << "Withdrawal successful! New balance: Rs. " << account->balance << endl;
        } else {
            cout << "Insufficient balance or invalid amount." << endl;
        }
    } else {
        cout << "Account not found." << endl;
    }
}

void checkBalance() {
    int accountNumber;

    cout << "Enter Account Number: ";
    cin >> accountNumber;

    Account* account = findAccount(accountNumber);
    if (account) {
        cout << "Current balance for " << account->name << " is Rs. " << account->balance << endl;
    } else {
        cout << "Account not found." << endl;
    }
}

void updateAccountDetails() {
    int accountNumber;
    cout << "Enter Account Number: ";
    cin >> accountNumber;

    Account* account = findAccount(accountNumber);
    if (account) {
        cout << "Updating details for " << account->name << endl;
        cout << "Enter New Name: ";
        cin.ignore();
        getline(cin, account->name);
        cout << "Enter New Age: ";
        cin >> account->age;
        saveAccountsToFile();
        cout << "Account details updated successfully!" << endl;
    } else {
        cout << "Account not found." << endl;
    }
}

void deleteAccount() {
    int accountNumber;
    cout << "Enter Account Number: ";
    cin >> accountNumber;

    auto it = remove_if(accounts.begin(), accounts.end(), [accountNumber](const Account& acc) {
        return acc.accountNumber == accountNumber;
    });

    if (it != accounts.end()) {
        accounts.erase(it, accounts.end());
        saveAccountsToFile();
        cout << "Account deleted successfully." << endl;
    } else {
        cout << "Account not found." << endl;
    }
}

// Main menu
void showMenu() {
    int choice;
    do {
        cout << "\nBank Management System" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Deposit Money" << endl;
        cout << "3. Withdraw Money" << endl;
        cout << "4. Check Balance" << endl;
        cout << "5. Update Account Details" << endl;
        cout << "6. Delete Account" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                depositMoney();
                break;
            case 3:
                withdrawMoney();
                break;
            case 4:
                checkBalance();
                break;
            case 5:
                updateAccountDetails();
                break;
            case 6:
                deleteAccount();
                break;
            case 7:
                cout << "Exiting... Thank you!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 7);
}

int main() {
    loadAccountsFromFile();
    showMenu();
    return 0;
}
