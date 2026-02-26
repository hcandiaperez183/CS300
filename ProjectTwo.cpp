//==================================================
//  Name        : ProjectTwo
//  Author      : Haley Candia Perez
//  Version     : 1.0
//==================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>

using namespace std;


//Structure to store course info

struct Course {
    string courseId;
    string courseName;
    vector<string> preList;
};

//Binary Search Tree to store Course objects

class BinarySearchTree {
    
private:
    //Node structure for BST
    struct Node{
        Course course;
        Node* left;
        Node* right;
        
        Node (Course aCourse) {
            course = aCourse;
            left = nullptr;
            right = nullptr;
        }
    };
    
    Node* root;
    int size;
    
    //Recursive helper function for in-order traversal
    void inOrder(Node* node) const {
        if (node == nullptr) {
            return;
        }
        
        inOrder (node->left);
        
        cout << node->course.courseId << ", "
            <<node->course.courseName << endl;
        
        inOrder (node->right);
    }
    
public:
    
    //Constructor
    BinarySearchTree() {
        root = nullptr;
        size = 0;
    }
    
    //Insert a course into the BST
    //Maintains BST ordering based on courseId
    
    void Insert(Course aCourse) {
        
        if (root == nullptr) {
            root = new Node(aCourse);
        }
        else {
            Node* currentNode = root;
            
            while (true) {
                if (aCourse.courseId < currentNode->course.courseId) {
                    
                    if (currentNode->left == nullptr) {
                        currentNode->left = new Node(aCourse);
                        break;
                    }
                    currentNode = currentNode->left;
                }
                else {
                    
                    if (currentNode->right == nullptr) {
                        currentNode->right = new Node(aCourse);
                        break;
                    }
                    currentNode = currentNode->right;
                }
            }
        }
        
        size++;
    }
    
    //Search for a course by courseId
    
    Course Search (string courseId) const {
        
        Node* currentNode = root;
        
        while (currentNode != nullptr) {
            
            if (currentNode->course.courseId == courseId) {
                return currentNode->course;
            }
            else if (courseId < currentNode->course.courseId) {
                currentNode = currentNode->left;
            }
            else {
                currentNode = currentNode->right;
            }
        }
        //Return empty course if not found return Course();
        return Course();
    }
    
    //Print all courses in alphanumeric order
    //Uses in-order traversal
    void PrintAll() const {
        inOrder(root);
    }
    
    //Return number of courses stored
    int Size() const {
        return size;
    }
};

//Convert string to uppercase
//Ensures course searches are case-insensitive
void convertCase (string& text) {
    for (char& c : text) {
        if (isalpha(c)) {
            c = toupper(c);
        }
    }
}

// Trim leading and trailing spaces from a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return ""; // string is all whitespace
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

// Load course data from CSV file into BinarySearchTree
void loadCourses(const string& csvPath, BinarySearchTree& courseList) {

    ifstream inFS(csvPath); // Open the file
    if (!inFS.is_open()) {
        cout << "Error: Could not open file \"" << csvPath << "\"." << endl;
        return;
    }

    string line;
    int loadedCourses = 0;

    while (getline(inFS, line)) {
        // Skip completely empty lines
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        vector<string> tokens;

        // Split line by commas and trim each token
        while (getline(ss, token, ',')) {
            tokens.push_back(trim(token));
        }

        // Ensure there is at least a course ID and course name
        if (tokens.size() < 2 || tokens[0].empty() || tokens[1].empty()) {
            continue; // skip invalid line
        }

        // Create course object
        Course aCourse;
        aCourse.courseId = tokens[0];
        aCourse.courseName = tokens[1];

        // Add any prerequisites (if present)
        for (size_t i = 2; i < tokens.size(); i++) {
            if (!tokens[i].empty()) {
                aCourse.preList.push_back(tokens[i]);
            }
        }

        // Insert course into BST
        courseList.Insert(aCourse);
        loadedCourses++;
    }

    inFS.close(); // Close the file

    cout << loadedCourses << " courses successfully loaded." << endl;
}
//Display detailed course info

void displayCourse (const Course& course) {
    
    cout << course.courseId << ", " << course.courseName << endl;
    
    cout << "Prerequisites: ";
    
    if (course.preList.empty()) {
        cout << "None";
    }
    else {
        for (unsigned int i = 0; i < course.preList.size(); i++) {
            cout << course.preList[i];
            if (i < course.preList.size() - 1) {
                cout << ", ";
            }
        }
    }
    
    cout << endl;
}

//Main program entry point

int main() {
    
    BinarySearchTree courseList;
    bool dataLoaded = false;
    int choice = 0;
    string csvPath;
    string courseKey;
    
    while (choice != 9) {
        
        cout << "\n==============================" << endl;
        cout << "ABCU Computer Science Courses" << endl;
        cout << "1. Load Courses" << endl;
        cout << "2. Display All Courses" << endl;
        cout << "3. Find Course" << endl;
        cout << "9. Exit" << endl;
        cout << "==============================" << endl;
        cout << "Enter choice: ";
        string input;
        getline(cin, input);
        
        try {
            choice = stoi(input);
        }
        catch (const std::invalid_argument& e) {
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        
        
        switch (choice) {
                
            case 1:
                cout << "Enter file name: ";
                getline(cin, csvPath);
                loadCourses(csvPath, courseList);
                cout << courseList.Size() << " courses loaded." << endl;
                dataLoaded = true;
                break;
                
            
            case 2:
                if (!dataLoaded) {
                    cout << "Please load course data first." << endl;
                }
                else {
                    courseList.PrintAll();
                }
                break;
                
            case 3:
                if (!dataLoaded) {
                    cout << "Please load course data first." << endl;
                }
                else {
                    cout << "Enter course ID: ";
                    getline(cin, courseKey);
                    
                    convertCase (courseKey);
                    
                    Course course = courseList.Search(courseKey);
                    
                    if (!course.courseId.empty()) {
                        displayCourse(course);
                    }
                    else {
                        cout << "Course not found." << endl;
                    }
                }
                break;
                
            case 9:
                cout << "Exiting program." << endl;
                break;
                
            default:
                cout << "Invalid selection." << endl;
        }
    }
    
    return 0;
}
