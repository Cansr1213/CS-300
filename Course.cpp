#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <climits>

struct Course {
    std::string courseId;
    std::string title;
    std::vector<std::string> prerequisites;

    Course(std::string _id, std::string _title, std::vector<std::string> _prerequisites) :
        courseId(_id), title(_title), prerequisites(_prerequisites) {}

    Course() {}
};

class HashTable {
private:
    const unsigned int DEFAULT_SIZE = 20;

    struct Node {
        Course course;
        unsigned int key;
        Node* next = nullptr; // Initialize next to nullptr

        Node() : key(UINT_MAX), next(nullptr) {}
        Node(Course _course) : key(UINT_MAX), next(nullptr), course(_course) {}
        Node(Course _course, unsigned int _key) : course(_course), key(_key), next(nullptr) {}
    };

    std::vector<Node> nodes;
    std::vector<Course*> alphaOrderedCourses;
    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(std::string key);
    void BubbleSort(std::vector<Course*>& courses);

public:
    HashTable();
    HashTable(unsigned int size);
    ~HashTable();
    void Insert(Course course);
    void InsertToAlpha(Course* course);
    void SortAlphaOrdered();
    void PrintAllCourses();
    void PrintCourse(std::string courseId);
};

HashTable::HashTable() {
    nodes.resize(tableSize);
}

HashTable::HashTable(unsigned int size) : tableSize(size) {
    nodes.resize(tableSize);
}

HashTable::~HashTable() {
    for (auto& node : nodes) {
        Node* current = node.next;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

unsigned int HashTable::hash(std::string key) {
    unsigned int hashValue;
    int k = static_cast<int>(key[0]);
    key = key.substr(4, 7);
    hashValue = (k + std::stoi(key) / 7) % tableSize;
    return hashValue;
}

void HashTable::Insert(Course course) {
    unsigned int key = hash(course.courseId);
    Node* node = new Node(course, key);
    if (nodes[key].key == UINT_MAX) {
        nodes[key] = *node;
    }
    else {
        Node* current = &nodes[key];
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = node;
    }
}

void HashTable::InsertToAlpha(Course* course) {
    alphaOrderedCourses.push_back(course);
}

void HashTable::SortAlphaOrdered() {
    BubbleSort(alphaOrderedCourses);
}

void HashTable::BubbleSort(std::vector<Course*>& courses) {
    for (size_t i = 0; i < courses.size(); ++i) {
        for (size_t j = 0; j < courses.size() - i - 1; ++j) {
            if (courses[j]->title > courses[j + 1]->title) {
                std::swap(courses[j], courses[j + 1]);
            }
        }
    }
}

void HashTable::PrintAllCourses() {
    for (const auto& course : alphaOrderedCourses) {
        std::cout << course->title << std::endl;
    }
}

void HashTable::PrintCourse(std::string _id) {
    try {
        unsigned int key = hash(_id);
        std::cout << std::endl << nodes[key].course.courseId << std::endl << nodes[key].course.title << std::endl;
        if (!nodes[key].course.prerequisites.empty()) {
            std::cout << "Prerequisites:" << std::endl;
            for (const auto& prereq : nodes[key].course.prerequisites) {
                std::cout << "    " << prereq << std::endl;
            }
        }
        std::cout << std::endl;
    }
    catch (...) {
        std::cout << std::endl << _id << " not found" << std::endl << std::endl;
    }
}

void addCourse(std::string courseInformation, HashTable* hashTable) {
    std::string courseId;
    std::string title;
    std::vector<std::string> prerequisites;
    std::string value;
    size_t position = 0;
    int i = 0;

    while ((position = courseInformation.find(",")) != std::string::npos) {
        value = courseInformation.substr(0, position);
        if (i == 0) {
            courseId = value;
        }
        else if (i == 1) {
            title = value;
        }
        else {
            prerequisites.push_back(value);
        }
        courseInformation.erase(0, position + 1);
        ++i;
    }

    Course* course = new Course(courseId, title, prerequisites);
    hashTable->Insert(*course);
    hashTable->InsertToAlpha(course);
}

void loadDataFile(HashTable* hashTable) {
    std::string fileName = "courses.txt"; // Update the file name if needed
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cout << "File " << fileName << " was not found." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        addCourse(line, hashTable);
    }

    file.close();
}

std::string getCourseId() {
    std::string line;
    std::cout << "Enter course ID: ";
    std::getline(std::cin, line);
    return line;
}

void displayMenu() {
    std::cout << std::endl
        << " 1. Load Data Structure." << std::endl
        << " 2. Print Course List." << std::endl
        << " 3. Print Course." << std::endl
        << " 4. Exit" << std::endl;
}

int getSelectionInput() {
    std::string input;
    std::getline(std::cin, input);
    try {
        return std::stoi(input);
    }
    catch (...) {
        std::cout << input << " is not a valid option." << std::endl;
        return -1;
    }
}

void executeSelection(int selection, HashTable* hashTable) {
    if (selection == 1) {
        loadDataFile(hashTable);
        hashTable->SortAlphaOrdered();
    }
    else if (selection == 2) {
        hashTable->PrintAllCourses();
    }
    else if (selection == 3) {
        std::string courseId = getCourseId();
        hashTable->PrintCourse(courseId);
    }
    else if (selection != 4) {
        std::cout << selection << " is not a valid option." << std::endl;
    }
}

int main() {
    HashTable* hashTable = new HashTable();
    int input = 0;
    while (input != 4) {
        displayMenu();
        input = getSelectionInput();
        executeSelection(input, hashTable);
    }
    std::cout << "Thank you for using the course planner!" << std::endl;
    delete hashTable;
    return 0;
}
