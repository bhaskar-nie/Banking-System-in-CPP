#include <iostream>
#include <string>
#include <ctime>
#include <unordered_map>
#include<vector>
using namespace std;

// Structure for representing a transaction
struct Transaction {
    time_t timestamp;
    float amount;
    string type; // "Deposit", "Withdrawal", "Transfer"
    string sourceAccNumber;
    Transaction* next;
};

// Structure for representing a bank account
struct Account {
    string accNumber;
    string accHolderName;
    float balance;
    string cardNumber;
    string cardType;
    Account* left;
    Account* right;
    Transaction* transactions;
    bool interestAdded; // Flag to indicate whether interest has been added for the current year

    // Constructor
    Account(string accNumber, string accHolderName, float balance) : 
        accNumber(accNumber), accHolderName(accHolderName), balance(balance), 
        left(NULL), right(NULL), transactions(NULL), interestAdded(false) {}
};

// Function to create a new account
Account* createAccount(string accNumber, string accHolderName, float balance) {
    return new Account(accNumber, accHolderName, balance);
}



// Function to insert a new account into the binary search tree
Account* insertAccount(Account* root, Account* newAccount) {
    if (root == NULL)
        return newAccount;
    if (newAccount->accNumber < root->accNumber)
        root->left = insertAccount(root->left, newAccount);
    else if (newAccount->accNumber > root->accNumber)
        root->right = insertAccount(root->right, newAccount);

    return root;
}


// Function to search for an account by account number
Account* searchAccount(Account* root, string accNumber) {
    if (root == NULL || root->accNumber == accNumber)
        return root;
    if (accNumber < root->accNumber)
        return searchAccount(root->left, accNumber);
    return searchAccount(root->right, accNumber);
}

// Function to perform deposit
void deposit(Account* acc, float amount) {
    acc->balance += amount;

    // Record transaction
    Transaction* transaction = new Transaction;
    transaction->timestamp = time(NULL);
    transaction->amount = amount;
    transaction->type = "Deposit";
    transaction->sourceAccNumber = "";
    transaction->next = acc->transactions;
    acc->transactions = transaction;
}

// Function to perform withdrawal
bool withdraw(Account* acc, float amount) {
    if (acc->balance >= amount) {
        acc->balance -= amount;

        // Record transaction
        Transaction* transaction = new Transaction;
        transaction->timestamp = time(NULL);
        transaction->amount = amount;
        transaction->type = "Withdrawal";
        transaction->sourceAccNumber = "";
        transaction->next = acc->transactions;
        acc->transactions = transaction;

        return true; // Successful withdrawal
    } else {
        cout << "Insufficient balance!" << endl;
        return false; // Failed withdrawal
    }
}

// Function to transfer funds between accounts
void transfer(Account* sourceAcc, Account* destAcc, float amount) {
    if (sourceAcc == destAcc) {
        cout << "Transfer failed: Source and destination accounts cannot be the same." << endl;
        return;
    }

    if (withdraw(sourceAcc, amount)) {
        deposit(destAcc, amount);

        // Record transaction in source account
        Transaction* transaction = new Transaction;
        transaction->timestamp = time(NULL);
        transaction->amount = amount;
        transaction->type = "Transfer";
        transaction->sourceAccNumber = destAcc->accNumber; // Source is the destination for transfers
        transaction->next = sourceAcc->transactions;
        sourceAcc->transactions = transaction;

        // Record transaction in destination account
        transaction = new Transaction;
        transaction->timestamp = time(NULL);
        transaction->amount = amount;
        transaction->type = "Transfer";
        transaction->sourceAccNumber = sourceAcc->accNumber; // Destination is the source for transfers
        transaction->next = destAcc->transactions;
        destAcc->transactions = transaction;

        cout << "Transfer successful!" << endl;
    } else {
        cout << "Transfer failed due to insufficient balance." << endl;
    }
}

// Function to display account details
void displayAccount(Account* acc) {
    cout << "Account Number: " << acc->accNumber << endl;
    cout << "Account Holder Name: " << acc->accHolderName << endl;
    cout << "Balance: " << acc->balance << endl;
    if (!acc->cardNumber.empty() && !acc->cardType.empty()) {
        cout << "Card Number: " << acc->cardNumber << endl;
        cout << "Card Type: " << acc->cardType << endl;
    } else {
        cout << "Card details not found." << endl;
    }
}

// Function to display all accounts (in-order traversal of the binary search tree)
void displayAllAccounts(Account* root) {
    if (root != NULL) {
        displayAllAccounts(root->left);
        displayAccount(root);
        cout << endl;
        displayAllAccounts(root->right);
    }
}

// Function to display transaction history for an account
void displayTransactionHistory(Account* acc) {
    cout << "Transaction History for Account Number " << acc->accNumber << ":" << endl; 
    Transaction* transaction = acc->transactions;
    while (transaction != NULL) {
        cout << "Timestamp: " << ctime(&transaction->timestamp);
        cout << "Type: " << transaction->type << endl;
        cout << "Amount: " << transaction->amount << endl;
        if (transaction->type == "Transfer") {
            cout << "Source Account: " << transaction->sourceAccNumber << endl; 
        }
        transaction = transaction->next;
    }
}

// Function to calculate interest for an account
void calculateInterest(Account* acc) {
    // Check if interest has already been added for the current year
    if (acc->interestAdded) {
        cout << "Interest has already been added for the current year for Account Number " << acc->accNumber << "." << endl;
        return;
    }
    // Check if balance is below minimum balance
if (acc->balance < 5000) {
    cout << "Minimum balance requirement not met for Account Number " << acc->accNumber << ". Interest cannot be added. Minimum Balance should be Rs.5000\n";
    return;
}

// Define the interest rate
float rate = 8.02; // 8.02 percent per annum

// Calculate interest
float interest = acc->balance * rate / 100.0;

// Add interest to the account balance
acc->balance += interest;

// Record interest transaction
Transaction* transaction = new Transaction;
transaction->timestamp = time(NULL);
transaction->amount = interest;
transaction->type = "Interest";
transaction->sourceAccNumber = "";
transaction->next = acc->transactions;
acc->transactions = transaction;

// Set the flag to indicate interest has been added for the current year
acc->interestAdded = true;

// Display success message
cout << "Interest rate for Account Number " << acc->accNumber << " is 8.02 percent per annum." << endl;
cout << "Interest calculated and added to account balance." << endl;
}
// Function to close an account
void closeAccount(Account** root, string accNumber) {
    Account* temp = *root;
    Account* parent = NULL;
    while (temp != NULL && temp->accNumber != accNumber) {
        parent = temp;
        if (accNumber < temp->accNumber)
            temp = temp->left;
        else
            temp = temp->right;
    }

    if (temp == NULL) {
        cout << "Account not found!" << endl;
        return;
    }

    if (temp->left == NULL) {
        if (temp->right == NULL) {
            if (parent == NULL)
                *root = NULL;
            else if (temp == parent->left)
                parent->left = NULL;
            else
                parent->right = NULL;
            delete temp;
        } else {
            if (parent == NULL)
                *root = temp->right;
            else if (temp == parent->left)
                parent->left = temp->right;
            else
                parent->right = temp->right;
            delete temp;
        }
    } else if (temp->right == NULL) {
        if (parent == NULL)
            *root = temp->left;
        else if (temp == parent->left)
            parent->left = temp->left;
        else
            parent->right = temp->left;
        delete temp;
    } else {
        Account* succ = temp->right;
        parent = NULL;
        while (succ->left != NULL) {
            parent = succ;
            succ = succ->left;
        }
        temp->accHolderName = succ->accHolderName;
        temp->accNumber = succ->accNumber;
        temp->balance = succ->balance;
        temp->cardNumber = succ->cardNumber;
        temp->cardType = succ->cardType;
        parent->left = succ->right;
        delete succ;
    }
    cout << "Account closed successfully!" << endl;
}

// Function to add card details to an account
void addCardDetails(Account* acc, string cardNumber, string cardType) {
    acc->cardNumber = cardNumber;
    acc->cardType = cardType;
    cout << "Card details added successfully!" << endl;
}

int main() {
    Account* root = NULL;
    int choice;
    string accNumber, destAccNumber,accHolderName;
    float input; // Changed to float for input validation
    float balance, amount; // Changed from string to float for input validation
    string cardNumber, cardType;
    Account* acc, *destAcc;

    do {
        cout << "\nBanking System Menu" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Transfer Funds" << endl;
        cout << "5. Display All Accounts" << endl;
        cout << "6. Display Account Details" << endl;
        cout << "7. Display Transaction History" << endl;
        cout << "8. Calculate Interest" << endl;
        cout << "9. Close Account" << endl;
        cout << "10. Add Card Details" << endl;
        cout << "11. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
    case 1: {
        cout << "Enter Account Number (Consists of 8 alphanumeric characters): ";
        cin.ignore(); // Clear input buffer
        getline(cin, accNumber);
        // Input validation and other necessary checks
        if (searchAccount(root, accNumber) != NULL) {
            cout << "Account already exists!" << endl;
            break;
        }
        cout << "Enter Account Holder Name: ";
        getline(cin, accHolderName); // Use getline directly
        // Input validation and other necessary checks
        cout << "Enter Initial Balance: ";
        cin >> balance;
        // Input validation and other necessary checks
        root = insertAccount(root, createAccount(accNumber, accHolderName, balance)); // Pass parameters directly
        cout << "Account created successfully!" << endl;
        break;
    }
    case 2: {
        cout << "Enter Account Number: ";
        cin.ignore(); // Clear input buffer
        getline(cin, accNumber);
        acc = searchAccount(root, accNumber);
        if (acc != NULL) {
            cout << "Enter Amount to Deposit: ";
            cin >> amount;
            // Input validation and other necessary checks
            deposit(acc, amount);
            cout << "Amount deposited successfully!" << endl;
        } else {
            cout << "Account not found!" << endl;
        }
        break;
    }
    case 3: {
        cout << "Enter Account Number: ";
        cin.ignore(); // Clear input buffer
        getline(cin, accNumber);
        acc = searchAccount(root, accNumber);
        if (acc != NULL) {
            cout << "Enter Amount to Withdraw: ";
            cin >> amount;
            // Input validation and other necessary checks
            if (withdraw(acc, amount)) {
                cout << "Amount withdrawn successfully!" << endl;
            }
        } else {
            cout << "Account not found!" << endl;
        }
        break;
    }
    case 4: {
        cout << "Enter Source Account Number: ";
        cin.ignore(); // Clear input buffer
        getline(cin, accNumber);
        acc = searchAccount(root, accNumber);
        if (acc != NULL) {
            cout << "Enter Destination Account Number: ";
            getline(cin, destAccNumber);
            destAcc = searchAccount(root, destAccNumber);
            if (destAcc != NULL) {
                cout << "Enter Amount to Transfer: ";
                cin >> amount;
                // Input validation and other necessary checks
                transfer(acc, destAcc, amount);
            } else {
                cout << "Destination Account not found!" << endl;
            }
        } else {
            cout << "Source Account not found!" << endl;
        }
        break;
    }
    case 5: {
        cout << "All Accounts:" << endl;
        displayAllAccounts(root);
        break;
    }
    case 6: {
        cout << "Enter Account Number: ";
        cin.ignore(); // Clear input buffer
        getline(cin, accNumber);
        acc = searchAccount(root, accNumber);
        if (acc != NULL) {
            displayAccount(acc);
        } else {
            cout << "Account not found!" << endl;
        }
        break;
    }
    case 7: {
        cout << "Enter Account Number: ";
        cin.ignore(); // Clear input buffer
        getline(cin, accNumber);
        acc = searchAccount(root, accNumber);
        if (acc != NULL) {
            displayTransactionHistory(acc);
        } else {
            cout << "Account not found!" << endl;
        }
        break;
    }
    case 8: {
        cout << "Enter Account Number: ";
        cin.ignore(); // Clear input buffer
        getline(cin, accNumber);
        acc = searchAccount(root, accNumber);
        if (acc != NULL) {
            calculateInterest(acc);
        } else {
            cout << "Account not found!" << endl;
        }
        break;
    }
    case 9: {
        cout << "Enter Account Number to Close: ";
        cin.ignore(); // Clear input buffer
        getline(cin, accNumber);
        closeAccount(&root, accNumber);
        break;
    }
    case 10: {
        cout << "Enter Account Number: ";
        cin.ignore(); // Clear input buffer
        getline(cin, accNumber);
        acc = searchAccount(root, accNumber);
        if (acc != NULL) {
            cout << "Enter Card Number: ";
            getline(cin, cardNumber);
            cout << "Enter Card Type: ";
            getline(cin, cardType);
            addCardDetails(acc, cardNumber, cardType);
        } else {
            cout << "Account not found!" << endl;
        }
        break;
    }
    case 11: {
        cout << "Exiting..." << endl;
        break;
    }
    default:
        cout << "Invalid choice!" << endl;
}



    } while (choice != 11);

    return 0;
}
