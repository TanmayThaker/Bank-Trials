#include <bits/stdc++.h>
#include <mutex>
#include <thread>
using namespace std;
#include "logger.h"
#include "employee.h"
mutex m1;
// void mainMenu();

int countDigits(int n)
{
    int count = 0;
    while (n != 0)
    {
        n /= 10;
        count++;
    }
    return count;
}

class Bank
{
private:
    char username[30];
    int password;
    int balance;
    double accountNumber;
    char accountType[30];

public:
    Bank()
    {
        balance = 0;
    }
    char *getUsernames()
    {
        // Return username
        Logger::Info("Username function is called");
        return username;
    }
    char *getAccountType()
    {
        Logger::Info("Returning account type of user");
        return accountType;
    }
    int getPassword()
    {
        // Return the password
        Logger::Info("Returning password of user");
        return password;
    }

    // Function to set the data
    void setData()
    {
        Logger::Info("Set data function called");
        cin.ignore();
        cout << "\nEnter username:";
        cin.getline(username, 30);
        cout << "\nEnter 4-digit password: ";
        cin >> password;
        while (countDigits(password) > 4 || countDigits(password) < 4)
        {
            Logger::Error("Invalid password entered");
            cout << "ERROR: Invalid Pin: " << endl;
            cout << "Enter your 4 digit pin please: ";
            cin >> password;
        }
        cin.ignore();
        cout << "\nEnter initial balance: " << endl;
        cin >> balance;
        while (balance < 0 || balance == 0)
        {
            Logger::Error("Invalid balance is entered");
            cout << "\n Please enter a valid number\nEnter initial balance: " << endl;
            cin >> balance;
        }
        cin.ignore();
        cout << "\nEnter 4 digit account number: " << endl;
        cin >> accountNumber;
        while (countDigits(accountNumber) < 4 || countDigits(accountNumber) > 4)
        {
            Logger::Error("Invalid account number is entered");
            cout << "Invalid account number. please enter a valid account number" << endl;
            cout << "\nEnter 4 digit account number: " << endl;
            cin >> accountNumber;
        }
        cout << "\nEnter account type " << endl;
        cin >> accountType;
        cin.ignore();
    }

    // Function displaying the data
    void showData()
    {
        Logger::Info("Show data function called");
        cout << "Username: " << username << endl;
        cout << "Password: " << password << endl;
        cout << "Balance: " << balance << endl;
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account type: " << accountType << endl;
    }

    // Function to add user
    int addUser()
    {
        Logger::Info("addUser function is called");
        Bank bank;
        ofstream file;

        // Open file in write mode
        file.open("data.txt", ios::out | ios::app);
        if (!file)
        {
            cout << "Error in creating file..." << endl;
            return 0;
        }

        // Read from user
        bank.setData();

        // Write into file
        file.write((char *)&bank, sizeof(bank)); // typecasting &bank into character pointer

        // Close the file
        file.close();

        return 0;
    }

    //  View Users
    int ViewAllUsers()
    {
        Logger::Info("View all users function called");
        Bank bank;

        ifstream file1;

        // Open file in read mode
        file1.open("data.txt", ios::in);
        if (!file1)
        {
            cout << "Error in opening file..";
            return 0;
        }

        // Read data from file
        file1.read((char *)&bank, sizeof(bank));
        while (!file1.eof())
        {

            // Display data on console
            bank.showData();
            file1.read((char *)&bank, sizeof(bank));
        }

        // Close the file
        file1.close();
        return 0;
    }

    // Function to delete the user
    void deleteUser(char uname[])
    {
        Logger::Info("Delete user function called", uname);
        Bank bank;
        fstream original, temp;
        original.open("data.txt", ios::in);
        if (!original) // If file notfound then return
        {
            cout << "\nfile not found";
            return;
        }
        else
        {
            temp.open("temp.txt", ios::out | ios::app);
            original.read((char *)&bank, sizeof(bank));

            // Till end of file is reached
            while (!original.eof())
            {

                if (!strcmp(bank.getUsernames(), uname)) // compare the username
                {
                    cout << "data found and deleted\n"
                         << bank.username << "\n";
                }
                else
                {
                    temp.write((char *)&bank, sizeof(bank));
                }

                original.read((char *)&bank, sizeof(bank));
            }
            original.close();               // I will close my original file i.e. data.txt file
            temp.close();                   // I will close my temp file i.e. temp.txt file
            remove("data.txt");             // I will remove the main data.txt file
            rename("temp.txt", "data.txt"); // I will rename my temp.txt file with data.txt file
            cout << "\nRemaining Users: " << endl;
            bank.ViewAllUsers();
        }
        return;
    }

    // function to deposit money
    void depositMoney(char uname[])
    {
        Logger::Info("Deposit Money function called");
        Bank bank;
        fstream file, temp;
        file.open("data.txt", ios::in | ios::out | ios::ate);
        temp.open("temp.txt", ios::out | ios::app);
        file.seekg(0); // pointing to the beginning of the file
        file.read((char *)&bank, sizeof(bank));

        // Till end of the file
        while (!file.eof())
        {
            if (!strcmp(bank.getUsernames(), uname))
            {
                int depositAmount;
                cout << "\nEnter amount to deposit:";
                cin >> depositAmount;
                try
                {
                    if (depositAmount > 0)
                    {
                        bank.balance = bank.balance + depositAmount;
                        cout << "\nBalance is: " << bank.balance;
                        temp.write((char *)&bank, sizeof(bank));
                    }
                    while (depositAmount < 0)
                    {
                        Logger::Critical("Negative amount is entered");
                        throw depositAmount;
                    }
                }
                catch (int amt)
                {
                    Logger::Error("Exception occured while depositing money");
                    cout << "Negative amount not allowed" << endl;
                    continue;
                }
            }
            else
            {
                temp.write((char *)&bank, sizeof(bank));
            }
            file.read((char *)&bank, sizeof(bank));
        }
        file.close();
        temp.close();
        remove("data.txt");
        rename("temp.txt", "data.txt");
    }

    // function to withdraw money
    void withdrawMoney(char uname[])
    {
        Logger::Info("Withdraw Money function called");
        Bank bank;
        fstream file, temp;
        file.open("data.txt", ios::in | ios::out | ios::ate);
        temp.open("temp.txt", ios::out | ios::app);
        file.seekg(0);
        file.read((char *)&bank, sizeof(bank));

        // Till end of file is reached
        while (!file.eof())
        {
            if (!strcmp(bank.getUsernames(), uname))
            {
                int withdrawMoney;
                cout << "\nEnter amount to withdraw:";
                cin >> withdrawMoney;

                try
                {
                    /*
                    if (bank.balance > withdrawMoney && withdrawMoney > 0)
                    {
                    }*/
                    while (bank.balance < withdrawMoney || withdrawMoney <= 0)
                    {
                        Logger::Critical("Invalid amount is entered");
                        throw withdrawMoney;
                    }
                    bank.balance = bank.balance - withdrawMoney;
                    temp.write((char *)&bank, sizeof(bank));
                    cout << "\nBalance is: " << bank.balance;
                }
                catch (int withdrawMoney)
                {
                    Logger::Error("Exception Occured while withdrawing money");
                    cout << "Please Enter the valid amount" << endl;
                    continue;
                }
            }
            else
            {
                temp.write((char *)&bank,
                           sizeof(bank));
            }
            file.read((char *)&bank, sizeof(bank));
        }

        // Close the file
        file.close();
        temp.close();
        remove("data.txt");
        rename("temp.txt", "data.txt");
    }

    //  Search user
    int searchUser(char uname[], int pass)
    {
        Logger::Info("Search User function called");
        Bank bank;
        fstream f;

        // Open the file
        f.open("data.txt", ios::in);
        if (!f)
        {
            cout << "Error in opening file...";
            return 0;
        }

        // Read data from file
        f.read((char *)&bank, sizeof(bank));
        while (!f.eof())
        {

            if (!strcmp(bank.getUsernames(), uname))
            {
                if (bank.getPassword() == pass)
                {
                    return 1;
                }
            }
            f.read((char *)&bank, sizeof(bank));
        }

        // Close the file
        f.close();
        return 0;
    }

    //  Function to search specific user
    int searchUserByUsername(
        char uname[])
    {
        Logger::Info("Search user by username function called");
        Bank bank;
        fstream file1;

        // Open the file
        file1.open("data.txt", ios::in);
        if (!file1)
        {
            cout << "Error in opening file..";
            return 0;
        }

        // Read data from file
        file1.read((char *)&bank, sizeof(bank));
        while (!file1.eof())
        {
            if (!strcmp(bank.getUsernames(), uname))
            {
                bank.showData();
                return 0;
            }
            file1.read((char *)&bank, sizeof(bank));
        }

        // Close the file
        file1.close();
        return 0;
    }
};

void atmMenu(int i)
{
    lock_guard<mutex> lock(m1);
    cout << "\nUser: " << i << endl;
    Bank bank;
    char uname[30];
    int pass, choice2, found = 0;
    cout << "\nEnter details to login as User\n ";
    cin.ignore();
    cout << "\nEnter username: ";
    cin.getline(uname, 30);
    cout << "\nEnter password: ";
    cin >> pass;
    found = bank.searchUser(uname, pass);
    if (found)
    {
    user:
        // System("cls");
        cout << "\nWelcome " << uname;
        cout << "\nWelcome to user Menu";
        cout << "\n 1. Deposit\n 2. Withdraw\n 3. View Account\n 4. Exit";
        cout << "\nEnter your choice:";
        cin >> choice2;
        switch (choice2)
        {
        case 1:
            bank.depositMoney(uname);
            goto user;
        case 2:
            bank.withdrawMoney(uname);
            goto user;
        case 3:
            bank.searchUserByUsername(uname);
            // sleep(4);
            goto user;
        case 4:
            cout << "Thank you";
            break;
        default:
            cout << "Please enter a valid choice" << endl;
            goto user;
        }
    }
    else
    {
        cout << "\nNo account found with username " << uname;
    }
}

/*
1) ATM management--> ATM menu()
2) Admin menu --> admin Menu()
3) Exit*/
void empMenu()
{
    Bank bank;
    Manager mg;
    Cashier cash;
    Employee emp;
    Employee *emp1;
    int pass, choice1;
    char uname[30];
rerun:
    cout << "\nEnter details to login as Admin\n";
    cout << "\nEnter password:";
    cin >> pass;
    if (countDigits(pass) > 5 || countDigits(pass) < 4)
    {
        cout << "ERROR: Invalid Pin: " << endl;
        cout << "Enter your 4 digit pin please: ";
        cin >> pass;
    }
    if (pass == 1234)
    {
    admin:
        cout << "\nWelcome to Admin/Manager Menu";
        cout << "\n 1. Add User\n 2. Delete User\n 3. View All User\n 4. Enter new record about Employee\n 5. Set Employee 's Salary \n 6. Get Employee' s Salary\n 7. Get Employment Details \n 8. Get Manager's Salary\n 9. Get Cashier's Salary\n 10. Export Data in Text Format \n 11. Exit";
        cout
            << "\nSelect one : ";
        cin >> choice1;
        switch (choice1)
        {
        case 1:
            bank.addUser();
            goto admin;

        case 2:
            cout << "\nEnter the username to be deleted : ";
            cin.ignore();
            cin.getline(uname, 30);
            bank.deleteUser(uname);
            goto admin;

        case 3:
            bank.ViewAllUsers();
            goto admin;
        case 4:
            emp.ReadData();
            goto admin;
        case 5:
            cout << "Enter the salary of Employee: ";
            int sal;
            cin >> sal;
            while (sal < 0)
            {
                Logger::Error("Invalid salary is entered");
                cout << "Salary cannot be negative" << endl;
                cout << "Please enter a valid salary" << endl;
                cin >> sal;
            }
            emp.setSalary(sal);
            goto admin;
        case 6:
            emp.getSalary();
            goto admin;
        case 7:
            int id;
            cout << "Welcome to Employee Search\n";
            cout << "Please enter employment ID: ";
            cin >> id;
            emp.getEmployeeDetails(id);
            goto admin;
        case 8:
            emp1 = &mg;
            emp1->getSalary();
            goto admin;

        case 9:
            emp1 = &cash;
            emp1->getSalary();
            goto admin;
        case 10:
            emp.exportDataAsTxt();
            goto admin;

        case 11:
            break;
        default:
            cout << "Invalid number! Please enter a valid number" << endl;
            goto admin;
        }
    }
    else
    {
        cout << "\nDetails are incorrect ! Please try again";

        goto rerun;
    }
}

void mainMenu()
{
    Logger::Info("mainMenu function called");
    int choice;
rerun:
    cout << "\nWelcome to Bank Management System" << endl;
    cout << "\n1. ATM Management";
    cout << "\n2. Admin/Manager Menu ";
    cout << "\n3. Exit";
    cout << "\n Enter your choice: " << endl;
    cin >> choice;
    int n = 2;
    vector<thread> threads(n);
    switch (choice)
    {
    case 1:
        int n;
        cout << "Enter number of people: " << endl;
        cin >> n;
        threads.resize(n);
        // creating n threads:
        for (int i = 0; i < n; i++)
        {
            threads[i] = thread(atmMenu, i + 1);
        }

        for (auto &th : threads)
        {
            th.join();
        }
        goto rerun;
    case 2:
        // cout << "Inside main menu case2";
        empMenu();
        goto rerun;
    case 3:
        cout << "Thank you for visiting the bank!" << endl;
        break;
    default:
        cout << "Invalid number! Please enter a valid choice" << endl;
        goto rerun;
    }
}

// My main function
int main()
{
    mainMenu();
    return 0;
}
