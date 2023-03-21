#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<ctime>
#include<chrono>
#include "Stack.h"

using namespace std;
using namespace std::chrono;

#pragma region all structs

// struct of type type box
typedef struct Box {
	unsigned short width;
	unsigned short length;
	char name;
	bool chekced;
	bool placed;
	bool backtracked;
};

// struct of type PackerProblem
typedef struct PackerProblem {
	unsigned short width;
	unsigned short length;
	unsigned short numBox;
	Box* boxArray;
	unsigned short** container;

	// function to print the original configurations of the container

	void printContainer() {
		// output the first line
		cout << "Container Configuration (" << width << "x" << length << ")" << endl;

		// output the top of the container
		cout << "+";
		for (int i = 0; i < width; i++) {
			cout << "-";
		}
		cout << "+" << endl;

		// output the middle section of the container
		for (int i = 0; i < width; i++) {
			cout << "|";
			for (int j = 0; j < length; j++) {
				cout << container[i][j];
			}
			cout << "|" << endl;
		}

		// output the bottom section of the container
		cout << "+";
		for (int i = 0; i < width; i++) {
			cout << "-";
		}
		cout << "+" << endl;
	}

	// function to print all boxes configurations

	void printBox() {
		// change line
		cout << endl;
		cout << numBox << " Boxes to be placed" << endl;
		cout << endl;

		// the following line of code are used to print the configurations of all boxes
		for (int i = 0; i < numBox; i++) {
			// print width and length
			cout << boxArray[i].name << " " << "(" << boxArray[i].width << "x" << boxArray[i].length << ")" << endl;
			// print box
			for (int j = 0; j < boxArray[i].width; j++) {
				for (int x = 0; x < boxArray[i].length; x++) {
					cout << boxArray[i].name;
				}
				cout << endl;
			}
			cout << endl;
		}
	}
};

//struct of type PackerSolver
typedef struct PackerSolver {
	PackerProblem* solvedProblem;

	// function to print the final configurations of the container

	void printSolvedContainer() {

		// output the top of the container
		cout << "+";
		for (int i = 0; i < solvedProblem->width; i++) {
			cout << "-";
		}
		cout << "+" << endl;

		// output the middle section of the container
		for (int i = 0; i < solvedProblem->width; i++) {
			cout << "|";
			for (int j = 0; j < solvedProblem->length; j++) {
				if (solvedProblem->container[i][j] == 0) cout << 0;
				else cout << (char)solvedProblem->container[i][j];
			}
			cout << "|" << endl;
		}

		// output the bottom section of the container
		cout << "+";
		for (int i = 0; i < solvedProblem->width; i++) {
			cout << "-";
		}
		cout << "+" << endl;
	}
};

// struct of type Coordinates
typedef struct Coordinates {
	int row;
	int col;
	char rotation;
};

// struct of type State
typedef struct State {
	Coordinates coords;
	short currentBox;
};

#pragma endregion

#pragma region sorting

// using merge sort because for the given data type it best to use a comparison based algorithm
// further it is the quickest algorithm from the comparison based algorithms

template <typename T>
void MergeSort(T originalArray[], T temp[], unsigned short left, unsigned short right) {

	// base condition
	if (left == right) return;

	// find mind
	unsigned short mid = left + ((right - left) / 2);

	// divide into two arrays
	MergeSort(originalArray, temp, left, mid);
	MergeSort(originalArray, temp, mid + 1, right);

	// ---------------------------------- //
	// up untill here the recursion is used to sort the arrays

	// following code does the merging

	unsigned short k = left, i = left, j = mid + 1;

	// merging into one array untill there are no more elements in one array
	while ((i <= mid) && (j <= right)) { // as long as left and right is true
		if (originalArray[i].width + originalArray[i].length < originalArray[j].width + originalArray[j].length) { // check if left is smaller than mid
			temp[k] = originalArray[i]; i++; // if yes them put in left of temp the left
		}
		else { // else if left is greater than mid
			temp[k] = originalArray[j]; j++; // put the left in the right
		}
		k++; // increment the general left
	}

	// copy elements from L1
	while (i <= mid) {
		temp[k] = originalArray[i];
		k++; i++;
	}
	//copy elements from L2
	while (j <= right) {
		temp[k] = originalArray[j];
		k++; j++;
	}

	// copy all elements from the temporary array to the original array
	for (i = left; i <= right; i++) {
		originalArray[i] = temp[i];
	}
}

#pragma endregion

#pragma region load

// loadPackerProblem is an essential function to this program since it reads the inputed file and it assings all the mandatory variables

PackerProblem* loadPackerProblem(string filename) {
	// create new packer problem pointer
	PackerProblem* problem = new PackerProblem();

	// opens the given file, creating  two strings to be used for error control
	ifstream file(filename);
	string line;
	string inputs[5];

	// getting the first line
	getline(file, line);

	unsigned short inputIndex = 0;

	// goes through the retrieved line and appends each character of the line in the inputs array
	for (int i = 0; i < line.length(); i++) {
		if (line[i] == ' ') inputIndex++; // if there is a space increase the index
		else inputs[inputIndex] += line[i];
	}

	// if container width and length are provided correctly 
	if (inputs[0] != "" && inputs[1] != "") {
		// do further checks
		if (!isalpha(inputs[0][0]) && stoi(inputs[0]) > 0 && !isalpha(inputs[1][0]) && stoi(inputs[1]) > 0) {
			problem->width = stoi(inputs[0]);
			problem->length = stoi(inputs[1]);
			// reset the input array
			inputs[0] = "";
			inputs[1] = "";
			inputIndex = 0;
		}
		// otherwise raise an according error
		else {
			cout << "Container Width or Length provided are incorect";
			return nullptr;
		}
	}
	// otherwise raise an according error
	else {
		cout << "Container Width or Length provided are incorect";
		return nullptr;
	}

	// get the next line
	getline(file, line);

	// goes through the retrieved line and appends each character of the line in the inputs array
	for (int i = 0; i < line.length(); i++) {
		if (line[i] == ' ') inputIndex++; // if there is a space increase the index
		else inputs[inputIndex] += line[i];
	}

	// if number of boxes is provided correctly
	if (inputs[0][0] != ' ' && inputs[0] != "" && !isalpha(inputs[0][0])) {
		// do further checks
		if (stoi(inputs[0]) > 0) {
			problem->numBox = stoi(inputs[0]);
			// reset the inputs array
			inputs[0] = "";
			inputIndex = 0;
		}
		// otherwise raise an according error
		else {
			cout << "Number of boxes provided is incorect";
			return nullptr;
		}
	}
	// otherwise raise an according error
	else {
		cout << "Number of boxes provided is incorect";
		return nullptr;
	}

	// create a new instance of the box array
	problem->boxArray = new Box[problem->numBox];

	unsigned short currentBox = 0;

	// while loop used to read all the box data
	while (file.good()) {

		// get the next line
		getline(file, line);

		// if the length of the line i not equal to 5 then raise an error
		if (line.length() != 5) {
			cout << "Incorrect file structure provided";
			return nullptr;
		}

		// reads the line and assigns the charchters to the inputs array
		for (int i = 0; i < line.length(); i++) {
			inputs[i] = line[i];
		}

		// performing similar checks as above to ensure that the box information is provided correctly
		if (!isalpha(line[0]) && stoi(inputs[0]) > 0 && inputs[1] == " " && !isalpha(line[2]) && stoi(inputs[2]) > 0 && inputs[3] == " " && !isdigit(line[4])) {
			problem->boxArray[currentBox].width = stoi(inputs[0]);
			problem->boxArray[currentBox].length = stoi(inputs[2]);
			problem->boxArray[currentBox].name = line[4];
			// reset the inputs array
			inputs[0] = "";
			inputs[2] = "";
			inputs[4] = "";
		}
		// raise an according error
		else {
			cout << "Box configurations provided incorrect";
			return nullptr;
		}

		// assigning all three of the booleans to false (these are used in the solver and helper functions)
		problem->boxArray[currentBox].chekced = false;
		problem->boxArray[currentBox].placed = false;
		problem->boxArray[currentBox].backtracked = false;

		currentBox++;
	}

	// check if the nuber of boxes provided is not equal to the expected one
	if (currentBox != problem->numBox) {
		cout << "insufficient number of boxes";
		return nullptr;
	}

	// initializing a double pointer array named container
	problem->container = new unsigned short* [problem->width];

	// initialize the container and fill it with zeros 
	
	for (int i = 0; i < problem->width; i++) {
		problem->container[i] = new unsigned short[problem->length];
	}
	for (int i = 0; i < problem->width; i++) {
		for (int j = 0; j < problem->length; j++) {
			problem->container[i][j] = 0;
		}
	}

	// create an array of type box with length the number of boxes
	Box* temp = new Box[problem->numBox];
	
	//MergeSort<Box>(problem->boxArray, temp, 0, problem->numBox);

	// print and return the problem
	problem->printContainer();

	problem->printBox();

	return problem;
}

#pragma endregion

#pragma region helperFunctions

// canPlace is a function that checks if a box can fit into the container, it further checks if it can be placed vertically or horizontally

bool canPlace(PackerProblem* problem, Coordinates& coords, short& currentBox) {
	// initialize booleans
	bool canPlace = true;
	bool verticalPlace = true;

	// check if a box can be placed vertically
	if (problem->boxArray[currentBox].length + coords.row - 1 >= problem->length || problem->boxArray[currentBox].width + coords.col - 1 >= problem->width) {
		verticalPlace = false; // box cannot be placed vertically 
	}

	// if the box cannot be placed vertically, check if it can be horizontally
	if (!verticalPlace) {
		if (problem->boxArray[currentBox].width + coords.row - 1 >= problem->width || problem->boxArray[currentBox].length + coords.col - 1 >= problem->length) {
			// box cannot be placed horizontally, therefore since both are false return false
			canPlace = false;
			return canPlace;
		}
	}

	// check for contact with other boxes - vertically
	if (verticalPlace) {
		for (int i = coords.row; i <= coords.row + problem->boxArray[currentBox].length - 1; i++) {
			for (int j = coords.col; j <= coords.col + problem->boxArray[currentBox].width - 1; j++) {
				if (problem->container[i][j] != 0) canPlace = false; // check if allready occupied
			}
		}
		// if all indexes are zeros it means that the box can be placed vertically
		if (canPlace) {
			coords.rotation = 'V'; // rotation set to 'V'
			return canPlace; 
		}
		else canPlace = true; // canPlace is reset 
	}

	// check for contact with other boxes - horizontally
	for (int i = coords.row; i <= coords.row + problem->boxArray[currentBox].width - 1; i++) {
		for (int j = coords.col; j <= coords.col + problem->boxArray[currentBox].length - 1; j++) {
			if (problem->container[i][j] != 0) canPlace = false; // check if occupied
		}
	}
	// if all indexes are zeros it means that the box can be placed horizontally
	if (canPlace) coords.rotation = 'H';

	// return the boolean
	return canPlace;
}

// place is a function that places a box into the container, the coords that have been set in the previous function are used.

void place(PackerProblem* problem, Coordinates& coords, short& currentBox) {

	// check if the box is horizontall
	if (coords.rotation == 'H') {
		// loop through the rows up to were the box will be placed
		for (int i = coords.row; i < coords.row + problem->boxArray[currentBox].width; i++) {
			// do the same for the collumns
			for (int j = coords.col; j < coords.col + problem->boxArray[currentBox].length; j++) {
				// change the zeros in the specified area with the box name
				problem->container[i][j] = problem->boxArray[currentBox].name;
			}
		}
	}
	// check if the box is verticall
	else if (coords.rotation == 'V') {
		// loop through the rows up to were the box will be placed 
		for (int i = coords.row; i < coords.row + problem->boxArray[currentBox].length; i++) {
			// do the same for the collumns
			for (int j = coords.col; j < coords.col + problem->boxArray[currentBox].width; j++) {
				// change the zeros in the specified area with the box name
				problem->container[i][j] = problem->boxArray[currentBox].name;
			}
		}
	}
	// since there now is one box less, the total number of boxes is decreased
	problem->numBox--;
}

// backtrack function is used to essentially remove a box from the container - reverting placement

void backtrack(PackerProblem* problem, Coordinates& coords, Stack<State>* stack) {

	// backtrack while the stack isnt empty
	short removedBox;
	while (!stack->isEmpty()) {

		// get the coordinates and the current box from the stack;
		coords = stack->top().coords;
		removedBox = stack->top().currentBox;
		problem->boxArray[removedBox].placed = false;

		// nested for loop is used to essentially undo the placing of the box by going to where the box is in the container and changing the values to 0

		for (int i = 0; i < problem->length; i++) {
			for (int j = 0; j < problem->width; j++) {
				if (problem->container[i][j] == stack->top().coords.rotation) {
					problem->container[i][j] = 0;
				}
			}
		}
		// remove the coords and the current box from the stack since they are not used anymore
		stack->pop();
		problem->numBox++;

		// if the current box hasnt been assigned as a backtracked one then change the boolean(backtracked) to true
		if (!problem->boxArray[removedBox].backtracked) {
			problem->boxArray[removedBox].backtracked = true;
			break;
		}

	}
}

// findNextBox function is used to find the next box that hasnt been placed

void findNextBox(PackerProblem* problem, short& currentBox, unsigned short& numOfBox) {
	if (currentBox > 0) {
		// if the current box is not the first one then move to the previous box
		currentBox--;
	}
	// loop through the box array untill an unplaced box is found
	while (problem->boxArray[currentBox].placed) {
		if (currentBox > 0) {
			// if the current box is not the first one, then move to the previous box
			currentBox--;
		}
		else {
			// if the current box is not the first one then wrap around to the last box
			currentBox = numOfBox;
		}
	}
}

// swapToNextConfig function swaps the box with the next box

void swapToNextConfig(PackerProblem* problem, short& currentBox, unsigned short& numOfBox, unsigned short& traversal) {
	// swapping the current box with the box at the next index
	swap(problem->boxArray[traversal], problem->boxArray[0]); 

	// assigning the new current box to be the first unplaced box
	currentBox = numOfBox;
	
	// incrementing the traversal to move to the next configuration
	traversal++;
	
	// if the traversal variable exceeds the total number of boxes, it resets.
	if (traversal > numOfBox) { // if the cycle exceeds the number of total boxes it should go back to one  
		traversal = 1;
	}
}

#pragma endregion

#pragma region solve

// The following function is the solver, it utilizes all the helper functions in order to find a configuration for the given boxes and container

PackerSolver* solver(PackerProblem* problem, bool debug = false) {

	// creating the packet solver variable and assigning it to the past problem
	PackerSolver* solvedProblem = new PackerSolver();
	solvedProblem->solvedProblem = new PackerProblem();
	solvedProblem->solvedProblem = problem;

	// declaring a stack pointer to store the placed boxes
	Stack<State>* stack = new Stack<State>();

	Coordinates coords = { 0, 0 };

	// assinging variables to help with the execution of the algorithm
	unsigned short steps = 0;
	unsigned short traversal = 1;
	unsigned short numOfBox = problem->numBox - 1;

	bool allChecked = false;

	short currentBox = numOfBox;

	// while all boxes are not placed
	while (0 != problem->numBox) {

		// First Check
		// If a box has not been placed and it can be placed then place the box
		if (!problem->boxArray[currentBox].placed && canPlace(problem, coords, currentBox)) {

			// place it and push it's state into the stack(coordinates, box index and box name)
			place(problem, coords, currentBox);
			stack->push(State{ Coordinates{coords.row, coords.col, problem->boxArray[currentBox].name}, currentBox });

			// set the given box is now placed and decrement the box index
			problem->boxArray[currentBox].placed = true;
			coords.rotation = ' ';
			currentBox--;
			steps++;
		}
		// Second Check
		// If the current position is empty and not all boxes have been checked find the next not placed box
		else if (0 == problem->container[coords.row][coords.col] && !allChecked) {

			// function finds the next box that hasnt been placed
			findNextBox(problem, currentBox, numOfBox);

			// if the next unplaced box hasnt been checked, set it as checked 
			if (!problem->boxArray[currentBox].chekced) problem->boxArray[currentBox].chekced = true;

			// else if its checked or it is the last box remaining unplaced
			else if (problem->boxArray[currentBox].chekced || problem->numBox == 1) { 
				allChecked = true; // set all checked to true

				// uncheck all boxes
				for (int i = 0; i <= numOfBox; i++) {
					problem->boxArray[i].chekced = false;
				}
			}
		}
		//Third Check
		// if the current position is occupied then move one step to the right
		else if (0 != problem->container[coords.row][coords.col]) {
			coords.col++;
			steps++;
		}
		// Forth Check
		// else backtrack
		else {
			// call the backtrack function 
			backtrack(problem, coords, stack);

			// the following lines of code swap boxes so that when the program continues a different configuration is tried
			if (stack->isEmpty()) {
				// call the swap function 
				swapToNextConfig(problem, currentBox, numOfBox, traversal);

				// for loop goes through all boxes and change the values of the struct booleans to false
				for (int i = 0; i <= numOfBox; i++) { 
					problem->boxArray[i].chekced = false;
					problem->boxArray[i].backtracked = false;
				}
			}
			// Boolean all checked is now false since all boxes (checked Boolean) are changed to false 
			allChecked = false;

			// increment the steps
			steps++;
		}

		// if the current position exceed the boundaries move accordingly
		if (coords.col == problem->width) {
			coords.col = 0;
			coords.row++;
			if (coords.row == problem->length) {
				coords.row = 0;
			}
		}

	}
	cout << "Steps needed: " << steps << endl;
	return solvedProblem;
}

#pragma endregion

int main() {

	high_resolution_clock::time_point start, finish;
	duration<double> duration;

	start = high_resolution_clock::now();

	PackerProblem* p = loadPackerProblem("example1.txt");
	if (p == nullptr) return 0;

	PackerSolver* s = solver(p);

	finish = high_resolution_clock::now();
	duration = finish - start;

	s->printSolvedContainer();

	cout << "Time: " << duration.count() << endl;

	// to use heuristic(MergeSort) uncomment the function in the loadPackerProblem function
}