#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

const int MAX_TRANSACTIONS = 100;
const int MAX_CUSTOMERS = 50;

class Transaction {
public:
    string type;
    double amount;

    Transaction() : type(""), amount(0.0) {}
    Transaction(string t, double a) : type(t), amount(a) {}
};

class Account {
private:
    static int nextAccountNumber;
    int accountNumber;
    double balance;
    Transaction transactions[MAX_TRANSACTIONS];
    int transactionCount;

public:
    Account() : accountNumber(nextAccountNumber++), balance(0.0), transactionCount(0) {}

    int getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount.\n";
            return;
        }
        balance += amount;
        addTransaction("Deposit", amount);
    }

    void withdraw(double amount) {
        if (amount <= 0 || amount > balance) {
            cout << "Invalid or insufficient funds.\n";
            return;
        }
        balance -= amount;
        addTransaction("Withdraw", amount);
    }

    void transfer(Account &receiver, double amount) {
        if (amount <= 0 || amount > balance) {
            cout << "Invalid or insufficient funds for transfer.\n";
            return;
        }
        balance -= amount;
        receiver.balance += amount;
        addTransaction("Transfer Out", amount);
        receiver.addTransaction("Transfer In", amount);
    }

    void addTransaction(string type, double amount) {
        if (transactionCount < MAX_TRANSACTIONS) {
            transactions[transactionCount++] = Transaction(type, amount);
        }
    }

    void showTransactions() const {
        cout << fixed << setprecision(2);
        for (int i = 0; i < transactionCount; i++) {
            cout << "- " << transactions[i].type << ": $" << transactions[i].amount << endl;
        }
    }

    void displayInfo() const {
        cout << "Account Number: " << accountNumber << "\n"
             << "Balance: $" << fixed << setprecision(2) << balance << "\n";
        showTransactions();
    }
};

int Account::nextAccountNumber = 1001;

class Customer {
private:
    string name;
    string password;
    Account account;

public:
    Customer() {}
    Customer(string n, string p) : name(n), password(p) {}

    string getName() const {
        return name;
    }

    bool verifyPassword(const string& pw) const {
        return password == pw;
    }

    Account& getAccount() {
        return account;
    }

    void displayCustomerInfo() const {
        cout << "Customer Name: " << name << "\n";
        account.displayInfo();
    }
};

class BankingSystem {
private:
    Customer customers[MAX_CUSTOMERS];
    int customerCount;

    int findCustomerIndexByName(const string &name) {
        for (int i = 0; i < customerCount; i++) {
            if (customers[i].getName() == name)
                return i;
        }
        return -1;
    }

public:
    BankingSystem() : customerCount(0) {}

    void createCustomer(const string &name, const string &password) {
        if (customerCount >= MAX_CUSTOMERS) {
            cout << "Customer limit reached.\n";
            return;
        }
        if (findCustomerIndexByName(name) != -1) {
            cout << "Customer already exists.\n";
            return;
        }
        customers[customerCount++] = Customer(name, password);
        cout << "Customer created successfully.\n";
    }

    void depositToAccount(const string &name, double amount, const string &password) {
        int index = findCustomerIndexByName(name);
        if (index != -1 && customers[index].verifyPassword(password)) {
            customers[index].getAccount().deposit(amount);
        } else {
            cout << "Customer not found or incorrect password.\n";
        }
    }

    void withdrawFromAccount(const string &name, double amount, const string &password) {
        int index = findCustomerIndexByName(name);
        if (index != -1 && customers[index].verifyPassword(password)) {
            customers[index].getAccount().withdraw(amount);
        } else {
            cout << "Customer not found or incorrect password.\n";
        }
    }

    void transferBetweenAccounts(const string &from, const string &fromPw,
                                 const string &to, double amount) {
        int senderIdx = findCustomerIndexByName(from);
        int receiverIdx = findCustomerIndexByName(to);
        if (senderIdx == -1 || receiverIdx == -1) {
            cout << "Sender or receiver not found.\n";
            return;
        }
        if (!customers[senderIdx].verifyPassword(fromPw)) {
            cout << "Incorrect sender password.\n";
            return;
        }

        customers[senderIdx].getAccount().transfer(customers[receiverIdx].getAccount(), amount);
    }

    void viewCustomerDetails(const string &name, const string &password) {
        int index = findCustomerIndexByName(name);
        if (index != -1 && customers[index].verifyPassword(password)) {
            customers[index].displayCustomerInfo();
        } else {
            cout << "Customer not found or incorrect password.\n";
        }
    }
};

// Main Menu
int main() {
    BankingSystem bank;
    int choice;
    string name, name2, password, password2;
    double amount;

    do {
        cout << "\n--- Banking System Menu ---\n";
        cout << "1. Create Customer\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Transfer\n";
        cout << "5. View Account Info\n";
        cout << "0. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter customer name: ";
                cin >> name;
                cout << "Enter password: ";
                cin >> password;
                bank.createCustomer(name, password);
                break;

            case 2:
                cout << "Enter customer name: ";
                cin >> name;
                cout << "Enter password: ";
                cin >> password;
                cout << "Enter deposit amount: ";
                cin >> amount;
                bank.depositToAccount(name, amount, password);
                break;

            case 3:
                cout << "Enter customer name: ";
                cin >> name;
                cout << "Enter password: ";
                cin >> password;
                cout << "Enter withdrawal amount: ";
                cin >> amount;
                bank.withdrawFromAccount(name, amount, password);
                break;

            case 4:
                cout << "Enter sender name: ";
                cin >> name;
                cout << "Enter sender password: ";
                cin >> password;
                cout << "Enter receiver name: ";
                cin >> name2;
                cout << "Enter transfer amount: ";
                cin >> amount;
                bank.transferBetweenAccounts(name, password, name2, amount);
                break;

            case 5:
                cout << "Enter customer name: ";
                cin >> name;
                cout << "Enter password: ";
                cin >> password;
                bank.viewCustomerDetails(name, password);
                break;

            case 0:
                cout << "Thank you for using the banking system.\n";
                break;

            default:
                cout << "Invalid option. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
