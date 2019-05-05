// Finn Nguyen
// CPSC 301-01

#include <iostream>
#include <fstream>
#include <string>
#include <cmath> 
using namespace std;

class digit
{
public:
	int data;
	digit *next = nullptr;
};

// function prototypes
digit * loadNumber(ifstream & file);
int digcmp(digit * left, digit *right);
char getOperator(ifstream & file);
void writeNumber(digit * num, ofstream & file);
void deleteNumber(digit * num);
digit * addNumbers(digit * left, digit * right);
digit * subNumbers(digit * left, digit * right);

int main() {
	digit *left, *right, *result;
	// open files
	ifstream inFile("largeNumbers.txt");
	ofstream outFile("output.txt");
	// operator
	char op;

	while (!inFile.eof()) {
		// load left and right operands
		left = loadNumber(inFile);

		if (left == nullptr)
			break;

		right = loadNumber(inFile);
		// get operator
		op = getOperator(inFile);
		
		// perform operation
		if (op == '+')
			result = addNumbers(left, right);
		else
			result = subNumbers(left, right);

		// write result
		writeNumber(result, outFile);

		// delete numbers
		deleteNumber(left);
		deleteNumber(right);
		deleteNumber(result);
	}

	// close files
	outFile.close();
	inFile.close();
	
	return 0;
}

/*
*  returns a number <0 || 0 || >0
*  <0 right number is larger
*  0  the numbers are equal
*  >0 left number is larger
*  only works if the number is stored in reverse order
*/
int digcmp(digit * left, digit * right) {
	int result = 0;
	do
	{
		if (left->data - right->data == 0) {
		} // take no action
		else {
			result = left->data - right->data;
		}
		left = left->next;
		right = right->next;

	} while (left != nullptr && right != nullptr);

	if (left == nullptr && right != nullptr) {
		// right number is larger
		result = -1;
	}

	if (left != nullptr && right == nullptr) {
		// left number is larger
		result = 1;
	}
	return result;
}

// Returns a pointer to the first element in the linked list
digit * loadNumber(ifstream & file) {
	digit * head = nullptr;
	string number;

	// read number
	if (!(file >> number))
		return nullptr;

	// for each digit
	for (size_t i = 0; i < number.length(); i++)
	{
		// create new digit
		digit * d = new digit;

		// store data
		d->data = number[i] - '0';

		// link digit before head
		d->next = head;

		// update head
		head = d;
	}

	// return head
	return head;
}

// Returns the operator
char getOperator(ifstream & file) {
	char op;

	// read operator
	file >> op;

	// return operator
	return op;
}

// Writes the resulting number to the file.
void writeNumber(digit * num, ofstream & file) {
	digit * d = num;

	while (d != nullptr)
	{
		file << d->data;
		d = d->next;
	}

	file << endl;
}

// Deletes list
void deleteNumber(digit * num) {
	// while list is not empty
	while (num != nullptr)
	{
		// store head to be deleted
		digit * d = num;
		// move head to next of it
		num = num->next;
		// delete older head
		delete d;
	}
}

// Adds the left and right operands. Returns a pointer to the first element
// of the result in a newly-created linked list.
digit * addNumbers(digit * left, digit * right) {
	// result number
	digit * result = nullptr;
	// carry
	int carry = 0;

	while (left != nullptr && right != nullptr)
	{
		// add digits
		int data = left->data + right->data + carry;
		// create new digit
		digit * d = new digit;
		
		// store digit
		d->data = data % 10;
		carry = data / 10;

		// link new digit
		d->next = result;
		result = d;

		// move left and right to next
		left = left->next;
		right = right->next;
	}

	while (left != nullptr)
	{
		// add carry
		int data = left->data + carry;
		// create new digit
		digit * d = new digit;

		// store digit
		d->data = data % 10;
		carry = data / 10;

		// link new digit
		d->next = result;
		result = d;

		// move left to next
		left = left->next;
	}

	while (right != nullptr)
	{
		// add carry
		int data = right->data + carry;
		// create new digit
		digit * d = new digit;

		// store digit
		d->data = data % 10;
		carry = data / 10;

		// link new digit
		d->next = result;
		result = d;

		// move right to next
		right = right->next;
	}

	if (carry > 0)
	{
		// create new digit
		digit * d = new digit;

		// store digit
		d->data = carry;
		
		// link new digit
		d->next = result;
		result = d;
	}

	return result;
}

// Subtracts the right operand from left and returns a pointer to the first element
// of the result in a newly-created linked list.
digit * subNumbers(digit * left, digit * right) {
	// result number
	digit * result = nullptr;
	// carry
	int carry = 0;
	// flag to indicate that result will be negative
	bool negative = false;

	// if right operand is larger
	if (digcmp(left, right) < 0)
	{
		// swap left and right
		digit * temp = left;
		left = right;
		right = temp;
		negative = true;
	}

	while (left != nullptr && right != nullptr)
	{
		// subtract digits
		int data = carry + left->data - right->data;
		// create new digit
		digit * d = new digit;

		if (data < 0)
		{
			data = data + 10;
			carry = -1;
		}
		else
			carry = 0;

		// store digit
		d->data = data;
		
		// link new digit
		d->next = result;
		result = d;

		// move left and right to next
		left = left->next;
		right = right->next;
	}

	while (left != nullptr)
	{
		// add carry
		int data = carry + left->data;
		// create new digit
		digit * d = new digit;

		if (data < 0)
		{
			data = data + 10;
			carry = -1;
		}
		else
			carry = 0;

		// store digit
		d->data = data;
		
		// link new digit
		d->next = result;
		result = d;

		// move left to next
		left = left->next;
	}

	// if result is negative then set first digit to negative
	if (negative)
		result->data = -result->data;

	return result;
}
