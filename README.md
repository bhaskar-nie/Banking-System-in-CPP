# Banking System Project

## Overview
This C++ project implements a basic banking system using a binary search tree data structure for account management. It provides essential banking operations such as account creation, deposits, withdrawals, fund transfers, and more.

## Features
- Account management (creation, closure, display)
- Deposit and withdrawal operations
- Fund transfers between accounts
- Transaction history tracking
- Interest calculation (with minimum balance requirement)
- Card details management

## Data Structures
- Binary Search Tree: For efficient account storage and retrieval
- Linked List: For maintaining transaction history of each account

## Requirements
- C++ compiler (C++11 or later)
- Standard C++ libraries

## Compilation and Execution
To compile and run the program:

```bash
g++ -std=c++11 Banking_System_Project.cpp -o banking_system
./banking_system
```

## Usage
Upon running the program, you'll be presented with a menu of options:

1. Create Account
2. Deposit
3. Withdraw
4. Transfer Funds
5. Display All Accounts
6. Display Account Details
7. Display Transaction History
8. Calculate Interest
9. Close Account
10. Add Card Details
11. Exit

Follow the on-screen prompts to perform various banking operations.

## Key Functions

### `createAccount()`
Creates a new bank account with the provided details.

### `deposit()` and `withdraw()`
Handle adding and removing funds from an account.

### `transfer()`
Transfers funds between two accounts.

### `displayAccount()` and `displayAllAccounts()`
Show details of a specific account or all accounts in the system.

### `displayTransactionHistory()`
Displays the transaction history for a specific account.

### `calculateInterest()`
Calculates and adds interest to accounts meeting the minimum balance requirement.

### `closeAccount()`
Removes an account from the system.

### `addCardDetails()`
Adds debit/credit card information to an account.

## Notes
- Account numbers consist of 8 alphanumeric characters.
- A minimum balance of Rs.5000 is required for interest calculation.
- The interest rate is fixed at 8.02% per annum.

## Future Improvements
- Implement file I/O for data persistence
- Add user authentication and security features
- Develop a graphical user interface
- Implement more advanced banking features (loans, fixed deposits, etc.)

## Contributing
Contributions to improve the project are welcome. Please follow these steps:
1. Fork the repository
2. Create a new branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request
