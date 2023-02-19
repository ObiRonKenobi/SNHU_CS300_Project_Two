/*
*	Ronald W. Sudol III
*	SNHU CS300
*	02/19/2023
*	Project Two : ABCU Course Binary Tree
*
*/

#include <iostream>
#include <conio.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>


using namespace std;

// define course structure
struct Course {
	string courseId;
	string title;
	string prereq1;
	string prereq2;

	//default constructor that initializes prereqs as empty for course that do not have prereqs
	Course() {
		courseId = "";
		title = "";
		prereq1 = "N/A";
		prereq2 = "N/A";
	}

	//overloaded constructor
	Course(string courseId, string title, string prereq1, string prereq2){
		this->courseId = courseId;
		this->title = title;
		this->prereq1 = prereq1;
		this->prereq2 = prereq2;;
	}
};


// define structure of binary tree
struct Node {
	Course course;
	Node* left;
	Node* right;

	//default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// intitialize node with a course
	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};

//defines binary search tree class as well as its methods and members
class BinarySearchTree {

//private methods and members
private:
	void inOrder(Node* node);
	void addNode(Node* node, Course course);

//public methods and members
public:
	Node* root;
	BinarySearchTree();
	void InOrder();
	void Insert(Course course);
	Course Search(string courseId);
};

//default constructor for binary tree
BinarySearchTree::BinarySearchTree() {

	//sets root equal to nullptr
	root = nullptr;
}

// Public function to call private method to traverse tree in order
void BinarySearchTree::InOrder() {

	//calls private inOrder function
	this->inOrder(root);
}

// defines private function inOrder
void BinarySearchTree::inOrder(Node* node) {
	
	//if node is not equal to nullptr
	if (node != nullptr) {

		//traverse left
		inOrder(node->left);

		//output course info
		cout << node->course.courseId << ": " << node->course.title << endl;
		
		//traverse right
		inOrder(node->right);
	}
}

//implement logic to insert a course object into binary tree
void BinarySearchTree::addNode(Node* node, Course course) {

	// if node is larger, add to left
	if (node->course.courseId.compare(course.courseId) > 0) {

		//if left node is null
		if (node->left == nullptr) {
			
			//left node becomes inserted node
			node->left = new Node(course);
		}

		//else
		else {
			
			//traverse left
			this->addNode(node->left, course);
		}

	}

	//else is node is smaller than inserted node
	else {

		//if no right node
		if (node->right == nullptr) {

			//inserted node becomes right node
			node->right = new Node(course);
		}

		//else
		else {

			//continue to traverse
			this->addNode(node->right, course);
		}
	}
}

// inserts course object into binary tree
void BinarySearchTree::Insert(Course course) {

	//if root is nullptr
	if (root == nullptr) {

		//new bid becomes root
		root = new Node(course);
	}

	//else
	else {
		this->addNode(root, course);
	}
}

//split line from file into vector of substrings
vector<string> splitString(string& text, char delim) {
	
	// initialize returnable split vector
	vector<string> splitLineVector;

	// Initialize the begining and end indices
	int beginIndex = 0;
	int endIndex = 0;

	// if no delim found us entire string
	endIndex = text.find(delim, beginIndex);
	if (endIndex < 0) {
		endIndex = text.length();
	}

	// iterate through entire string
	while (beginIndex < text.length() - 1) {

		// add substring to vector
		splitLineVector.push_back(text.substr(beginIndex, endIndex - beginIndex));
		
		// update beginning index for next substring
		beginIndex = endIndex + 1;

		// returns substring at end of string
		endIndex = text.find(delim, beginIndex);
		if (endIndex < 0) {
			endIndex = text.length();
		}

	}

	return splitLineVector;
}

// loads in course list file
void loadCourses(string filename, BinarySearchTree* courseTree) {
	
	// for reading file row
	string nextLine;

	// initialize file stream and open course file
	ifstream file;
	file.open(filename);
	

	// iterate through file line by line
	while (getline(file, nextLine)) {
		vector<string> courseData = splitString(nextLine, ',');

		//check that course data contains at least an Id and Title
		if (courseData.size() < 2) {
			cout << "ERROR: Course line must include Course Id and Course Title" << endl;
			break;
		}

		//create course object
		Course course;
		course.courseId = courseData.at(0);
		course.title = courseData.at(1);

		//if course has prerequisites
		if (courseData.size() == 3) {
			course.prereq1 = courseData.at(2);
		}
		if (courseData.size() == 4) {
			course.prereq1 = courseData.at(2);
			course.prereq2 = courseData.at(3);
		}

		// add course to tree
		courseTree->Insert(course);
	}

	//close file at end to free memory
	file.close();
}

//search tree and print specific course info
Course BinarySearchTree::Search(string courseId) {

	//start at root
	Node* current = root;


	//loop until nullptr
	while (current != nullptr) {

		//return match
		if (current->course.courseId.compare(courseId) == 0) {
			return current->course;
		}

		//traverse left
		if (courseId.compare(current->course.courseId) < 0) {
			current = current->left;
		}

		//traverse right
		else {
			current = current->right;
		}
	}

	// if not found return default course
	Course emptyCourse;
	return emptyCourse;
}

//print a specified course
void displayCourse(Course course) {
	cout << course.courseId << ": " << course.title << endl;
	
	//if course has prerequisites, print those too
	if (course.prereq1 != "N/A") {
		cout << "    Prerequisites: " << course.prereq1;
		if (course.prereq2 != "N/A") {
			cout << " & " << course.prereq2;
		}
		cout << endl;
	}
	cout << endl;
}

// This is where the magic happens
int main() {

	//initialize object and variables
	BinarySearchTree* courseTree;
	courseTree = new BinarySearchTree;
	Course course;
	string filePath;
	string courseKey;
	int choice = 0;

	//program name
	cout << "WELCOME to ABCU Course Planner" << endl;
	cout << endl;

	//menu loop
	while (choice != 9) {

		//display menu options
		cout << "~ 1. Load Data Structure" << endl;
		cout << "~ 2. Print Course List" << endl;
		cout << "~ 3. Print Course" << endl;
		cout << "~ 9. Exit" << endl;
		cout << "~ ~  Please make selection. " << endl << endl;

		//read user input
		cin >> choice;

		//handles user choices
		switch (choice) {

		//user chooses option 1
		case 1:

			//prompts user for file name, file name is provided
			//after **HINT** marker for the purpose of this assignment
			cout << endl;
			cout << "Enter file name to be loaded " << endl;
			cout << "**HINT**" << endl;
			cout << "File name is Course_Info_List.txt" << endl;
			cout << "File name is case sensitive" << endl;
			cout << endl;

			// read user input
			cin >> filePath;

			// call file loading function
			loadCourses(filePath, courseTree);
			cout << endl;

			//notify if file not found
			if (courseTree->root == nullptr) {
				cout << "ERROR: File dide not load properly" << endl;
				cout << "Please try again" << endl;
				cout << endl;

			}

			//notify file was loaded succesfully
			else {
				cout << filePath << " loaded successfully!" << endl;
				cout << endl;
			}
			break;

			//user chooses option 2
		case 2:

			//error if file not loaded yet
			if (courseTree->root == nullptr) {
				cout << endl;
				cout << "Please load course file first and try again" << endl;
				cout << endl;
				break;
			}

			//call in order function to print courses alphanumerically
			else {
				cout << endl;
				cout << "Sorted ABCU Course List:" << endl << endl;
				courseTree->InOrder();
				cout << endl;
			}
			break;

			// user choice 3
		case 3:

			// error if file not loaded yet
			if (courseTree->root == nullptr) {
				cout << endl;
				cout << "Please load course file first and try again" << endl;
				cout << endl;
				break;
			}
			//prompt user for course number to be found
			cout << endl;
			cout << "Please Enter Course ID" << endl;
			cout << "Course ID is case sensitive" << endl;
			cout << endl;
			cin >> courseKey;

			// pass user input to search function, returns found or empty course
			course = courseTree->Search(courseKey);

			//if course is not empty, call display function to rpint course info
			if (course.courseId != "") {
				cout << endl;
				displayCourse(course);
				cout << endl;
			}

			// if course is empty display course not found message
			else {
				cout << endl;
				cout << "ERROR: Course not found." << endl;
				cout << " Please Try Again." << endl;
				cout << endl;

			}

			break;

		// user exit choice
		case 9:
			cout << endl;
			cout << "Thank you for using ABCU Course Planner" << endl << endl;
			cout << "Stonks, Bruh!" << endl;
			break;

		// handles errant user input from menu
		default:
			cout << endl;
			cout << "Please Try Again" << endl;
			cout << endl;
			cin.clear();
			cin.ignore(1000, '\n');

		}



	}

	//GOODBYE
	return 0;
}


/*
* Rip and Tear!
* ...until it is done!
*/