//Jacob Newton
//Project1_Newton_jan0051.cpp
//How to compile: g++ project1_Newton_jan0051.cpp
//How to run: execute the a.out file, enter the loan amount, interest rate, and monthly payments when prompted
//Sources: ChatGPT was used to assist in creating the formatDouble function (lines 16-24). No other sources were used.

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <sstream>

using namespace std;

//Function to assist in the formatting for the Amortization Table
string formatDouble(double value, int precision = 2, bool addDollar = false) {
    ostringstream oss;
    oss << fixed << setprecision(precision);
    if (addDollar) {
        oss << "$";
    }
    oss << value;
    return oss.str();
}

void heading(double loan, double rate, double monthlyPayments) {
    cout << "Loan Amount: $" << loan << endl;
    cout << "Interest Rate " << "%( per year): " << rate << endl;
    cout << "Monthly Payments: " << monthlyPayments << endl;
    cout << "**********************************************************" << endl;
    cout << "        Amortization Table" << endl;
    cout << "**********************************************************" << endl;
    cout << "Month   Balance   Payment      Rate   Interest   Principal" << endl;
    cout << "0       $" << loan << "     N/A          N/A    N/A        N/A" << endl;
}

void printTable(double loan, double rate, double monthlyPayments, double &totalInterest, int &totalMonths) {
    double balance = loan;
    double interest;
    double principal;
    double rateMonthly = rate / 12;
    double payment = monthlyPayments;
    int month = 1;

    while (balance > 0) {
        interest = balance * rateMonthly / 100;
       
        if (interest >= payment) { //Error handling for insufficient payment to cover interest
            cout << "ERROR: Insufficient payment to cover interest." << endl;
            exit(1);
        }

        principal = payment - interest;
        balance -= principal;

        if (balance < 0) { //Adjusting final payment to ensure balance is 0
            principal += balance;
            balance = 0;
        }

        totalInterest += interest;
        totalMonths++;

        cout << left << setw(8)  << month
            << left << setw(10) << formatDouble(balance, 2, true)
            << left << setw(13)  << formatDouble(payment, 2, true)
             << left << setw(7)  << formatDouble(rateMonthly, 2)
            << left << setw(11) << formatDouble(interest, 2, true)
            << left << formatDouble(principal, 2, true)
            << endl;

        month++;
    }
}

void footing(double totalInterest, int totalMonths) {
    cout << "**********************************************************" << endl;
    cout << '\n';
    cout << "It takes " << totalMonths << " months to pay off the loan." << endl;
    cout << fixed << setprecision(2);
    cout << "Total interest paid off is: $" << totalInterest << '.' << endl;
}

//Collecting user inputs for loan amount, interest rate, and monthly payments
void getInputs(double &loan, double &rate, double &monthlyPayments) {
    cout << "Loan Amount: ";
    cin >> loan;
    cout << "Interest Rate (% per year): ";
    cin >> rate;
    cout << "Monthly Payments: ";
    cin >> monthlyPayments;

    //Error handling for invalid inputs
    if (loan < 0 || rate < 0 || monthlyPayments < 0 || cin.fail()) {
        cout << "ERROR: Invalid input. Please enter a positive number." << endl;
        exit(1);
    }
}

int main() {
    double loan, rate, monthlyPayments;
    double totalInterest = 0.0;
    int totalMonths = 0;

    getInputs(loan, rate, monthlyPayments);
    heading(loan, rate, monthlyPayments);
    printTable(loan, rate, monthlyPayments, totalInterest, totalMonths);
    footing(totalInterest, totalMonths);

    return 0;
}