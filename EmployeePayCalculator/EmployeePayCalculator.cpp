// EmployeePayCalculator.cpp
// Calculate gross pay, net pay, pension and deductions for employees of a virtual company.
// Lavino Wei-Chung Chen
// June 24, 2020


#include <iostream>
#include <conio.h>  // Required for _getch() function
#include <fstream>  // Required for file input/output
#include <iomanip>

using namespace std;

//global constants

// social insurance check variables
const int SOCIAL_INS_NUM_LOWER_CHECK = 100000000; // lower boundary check for social insurance number
const int SOCIAL_INS_NUM_UPPER_CHECK = 1000000000; // upper boundary check for social insurance number

// exemption calculation variables
const int EXEMPTION_TOP_CHECK = 19; // top boundary check for exemptions

// gross pay calculation variables
const double PAY_RATE_MAX = 99.99; // maximum pay rate per hour
const double HOUR_MAX = 54.00; // maximum work hour
const double REGULAR_HOUR = 40.00; // regular work hour
const double OVERTIME_RATE_BASE = 1.5; // overtime calculation rate

//pension calculation constant variables
const double PENSION_RATE = 0.077; // calculation rate for pension
const double PENSION_BASE_VALUE = 16.5; // pension base value

// deduction calculation constant variables
const double TAXABLE_EXEMPTION_INDEX = 14.00; // taxable exemption index value
const double TAXABLE_INDEX = 11.00; // taxable general index value
const double FEDERAL_RATE_FOR_TAXABLE = 0.00023; // taxable rate for dederal tax calculation
const double FEDERAL_RATE_BASE = 0.14; // taxable rate base for dederal tax calculation
const double PROVINCIAL_RATE_INDEX = 0.35; // taxable rate for provincial tax calculation

// One way to specify a file name:
const char * IN_FILE = "EmployeePayInput.txt";

// A second way to specify a file name:
#define OUT_FILE "EmployeePayOutput.txt"

bool socialInsuranceNumCheck(long socialInsuranceN); // check if social insurance number is valid
bool numberOfExemptionsCheck(int numOfExemptions); // check if number of exemptions is valid
bool payRateCheck(double payRateInput); // check if pay rate is valid
bool hoursworkCheck(double hours); // check if hours worked is valid

double grossPayCalculation(double payRateInput, double hours); // function to calculate gross pay
double deductionCalculation(double gross, int numOfExemptions, double pension); // function to calculate deductions
double netPayCalculation(double gross, double deduction); // function to calculate net pay
double pensionCalulation(double gross); // function to calculate pension


int main()
{
	//Declare variables
	long socialInsuranceNum;  // Will store social security number of employee
	int numberOfExemptions;  // Will store number of excemptions for employee
	int count = 0; // vairable for counting number of employees processed

	double payRate,			// Will store the pay rate for the employee
		hoursWorked,		// Will store hours worked for the employee
		grossPay,			// variable for gross pay
		netPay,				// variable for net pay
		pension,			// variable for pension
		deductions,			// variable for deductions
		totalGrossPay = 0,		// variable for total gross pay
		totalNetPay = 0,		// variable for total net pay
		totalPension = 0,		// variable for total pension
		totalDeductions = 0;	// variable for total deductions

	//Define ifstream object and open file
	ifstream ins;
	ins.open(IN_FILE);

	//Check that file opened without any issues
	if (ins.fail())
	{
		cerr << "ERROR--> Unable to open input file : " << IN_FILE << endl;
		cerr << '\n' << endl;
		_getch(); // causes execution to pause until a char is entered
		return -1; //error return code
	}

	//Define ofstream object and open file
	ofstream outs;
	outs.open(OUT_FILE);

	//Check that file opened without any issues
	if (outs.fail())
	{
		cerr << "ERROR--> Unable to open output file : " << OUT_FILE << endl;
		cerr << '\n' << endl;
		_getch(); // causes execution to pause until a char is entered
		return -2; //error return code
	}

	// Read in first social security number
	ins >> socialInsuranceNum;

	outs << "Bellus Telecom Supplies Company" << endl;
	outs << "---------------------------------\n" << endl;

	outs << "Social insurance no." << setw(12) << "gross pay" << setw(11) << "net pay" << setw(11) << "pension" << setw(14) << "deductions" << endl;
	outs << "--------------------------------------------------------------------" << endl;

	// Process data until end of file is reached
	while (!ins.eof()){

		// Read in other data for the employee
		ins >> payRate >> numberOfExemptions >> hoursWorked;

		outs << fixed << showpoint << setprecision(2);
		cerr << fixed << showpoint << setprecision(2);

		// if social insurance number is not valid, print the error message
		if (!socialInsuranceNumCheck(socialInsuranceNum))
		{
			cerr << setw(10) << socialInsuranceNum << setw(4) << "" << "INPUT DATA ERROR:  Invalid social security #" << endl;
		}

		// if number of exemptions is not valid, print the error message
		if (!numberOfExemptionsCheck(numberOfExemptions))
		{
			cerr << setw(10) << socialInsuranceNum << setw(4) << "" << "INPUT DATA ERROR:  Invalid number of exemptions: " << numberOfExemptions << endl;
		}

		// if pay rate is not valid, print the error message
		if (!payRateCheck(payRate))
		{
			cerr << setw(10) << socialInsuranceNum << setw(4) << "" << "INPUT DATA ERROR:  Invalid pay rate: " << payRate << endl;
		}

		// if hours worked is not valid, print the error message
		if (!hoursworkCheck(hoursWorked))
		{

			cerr << setw(10) << socialInsuranceNum << setw(4) << "" << "INPUT DATA ERROR:  Invalid hours worked: " << hoursWorked << endl;
		}

		// if all inputs are valid, process the calculations and print the report
		if (socialInsuranceNumCheck(socialInsuranceNum) && numberOfExemptionsCheck(numberOfExemptions) && payRateCheck(payRate)
			&& hoursworkCheck(hoursWorked)) 
		{
			count++;

			// calculate individual numbers in report
			grossPay = grossPayCalculation(payRate, hoursWorked);
			pension = pensionCalulation(grossPay);
			deductions = deductionCalculation(grossPay, numberOfExemptions, pension);
			netPay = netPayCalculation(grossPay, deductions);

			// output the numbers in report
			outs << socialInsuranceNum << setw(23) << grossPay << setw(11) << netPay << setw(11) << pension << setw(14) << deductions << endl;
		
			// calculate summary numbers in report
			totalGrossPay += grossPay;
			totalNetPay += netPay;
			totalPension += pension;
			totalDeductions += deductions;
		}

		// The following line of code is used to confirm that reading and writing from/to files is working correctly.
		//  This line of code (and these 2 comment lines) will need to be removed in your final solution.
		//outs << socialInsuranceNum << "  " << payRate << "  " << numberOfExemptions << "  " << hoursWorked << '\n';

		// Read in next social security number
		ins >> socialInsuranceNum;
	}

	outs << "\n\n" << endl;

	outs << "Summary" << endl;
	outs << "-------\n" << endl;

	outs << "Number of employees processed:" << setw(26) << count << endl;
	outs << "Total gross pay for all employees:" << setw(22) << totalGrossPay << endl;
	outs << "Total net pay for all employees:" << setw(24) << totalNetPay << endl;
	outs << "Total pension withheld for all employees:" << setw(15) << totalPension << endl;
	outs << "Total deductions for all employees:" << setw(21) << totalDeductions << endl;

	// Close files
	ins.close();
	outs.close();

	cout << '\n' << endl;

	// Remove following line of code (and this comment) from your solution
	//cout << "Type any key to continue ... \n\n";

	_getch(); // causes execution to pause until char is entered

}

// function to check if social insurance number is valid
bool socialInsuranceNumCheck(long socialInsuranceN)
{
	if (socialInsuranceN < SOCIAL_INS_NUM_LOWER_CHECK || socialInsuranceN > SOCIAL_INS_NUM_UPPER_CHECK)
	{
		return false;
	}

	return true;
}

// function to check if number of exemptions is valid
bool numberOfExemptionsCheck(int numOfExemptions)
{
	if (numOfExemptions < 0 || numOfExemptions > EXEMPTION_TOP_CHECK)
	{
		return false;
	} 

	return true;
}

// function to check if pay rate is valid
bool payRateCheck(double payRateInput) 
{
	if (payRateInput < 0 || payRateInput > PAY_RATE_MAX)
	{	
		return false;
	}

	return true;
}

// function to check if hours worked is valid
bool hoursworkCheck(double hours) 
{
	if (hours < 0 || hours > HOUR_MAX) 
	{
		return false;
	}

	return true;
}

// function to calculate gross pay
double grossPayCalculation(double payRateInput, double hours)  
{
	double grossPay = 0;

	if (hours <= REGULAR_HOUR) 
	{
		grossPay = payRateInput * hours;
	}
	else if (hours > REGULAR_HOUR && hours <= HOUR_MAX) // times additional half if worked hour over 40 hours, to maximum 54 hours
	{
		grossPay = payRateInput * REGULAR_HOUR + payRateInput * (hours - REGULAR_HOUR) * OVERTIME_RATE_BASE; 
	}

	if (grossPay < 0) // if gross pay is less than 0, set it to 0
	{
		grossPay = 0;
	}

	return grossPay;
}

// function to calcuate pension
double pensionCalulation(double gross) 
{
	double pension;

	pension = PENSION_RATE * gross; 

	if (pension > PENSION_BASE_VALUE) 
	{
		pension = PENSION_BASE_VALUE;
	}

	if (pension < 0) // if pension is less than 0, set it to 0
	{
		pension = 0;
	}
	
	return pension;
}

// function to calcuate deductions
double deductionCalculation(double gross, int numOfExemptions, double pension)
{
	double deduction;
	double taxable; 
	double federal;
	double provincial; 	

	taxable = gross - (TAXABLE_EXEMPTION_INDEX * numOfExemptions) - TAXABLE_INDEX;  

	if (taxable < 0) // if taxable is less than 0, set it to 0
	{
		taxable = 0;
	}

	federal = taxable * (FEDERAL_RATE_BASE + (FEDERAL_RATE_FOR_TAXABLE * taxable)); 
	provincial = federal * PROVINCIAL_RATE_INDEX;	

	deduction = federal + provincial + pension;

	if (deduction < 0) // if deductions is less than 0, set it to 0
	{
		deduction = 0;
	}

	return deduction;
}

// function to calculate net pay
double netPayCalculation(double gross, double deduction)
{
	double netPay;

	netPay = gross - deduction;

	if (netPay < 0) // if net pay is less than 0, set it to 0
	{
		netPay = 0;
	}

	return netPay;
}
