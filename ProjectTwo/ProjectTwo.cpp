#include <iostream> 
#include <fstream>
#include <string>
#include <vector> 
#include <limits> 
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;


// function declarations (in order of line defined)
void printMenu();
void loadData();
void printList();
void printCourse();


// data model
struct Course {
    string number;
    string title;
    vector<string> prereqs;
};


// global variables
bool fileLoaded = false;
vector<Course> courses;


// tiny helpers
string upper(string s) {
    for (char& c : s) c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    return s;
}
vector<string> splitCSV(const string& line) {
    vector<string> out;
    string item;
    stringstream ss(line);
    while (getline(ss, item, ',')) {
        // trim leading/trailing spaces and \r
        size_t a = item.find_first_not_of(" \t\r");
        size_t b = item.find_last_not_of(" \t\r");
        out.push_back(a == string::npos ? "" : item.substr(a, b - a + 1));
    }
    return out;
}
int findIndex(const string& num) {
    string key = upper(num);
    for (size_t i = 0; i < courses.size(); ++i) {
        if (upper(courses[i].number) == key) return static_cast<int>(i);
    }
    return -1;
}


// I know it's unorthodox to have the main at the top, but anytime I build something from
// from scratch, this is the fromat I prefer to work with
int main () {
    cout << "Welcome!" << endl;
    
    
    
    while (true) {
        printMenu();
        
        int choice;
        // error handling so letter inputs don't cause an infinite loop
        if (!(cin >> choice)) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch(choice) {
            case 1: 
                loadData();
                break;
            
            case 2: 
                if (fileLoaded == true) {
                    printList(); }
                else {
                    cout << "Data must be loaded first!" << endl; }
                break;
            
            case 3:
                if (fileLoaded == true) {
                    printCourse(); }
                else {
                    cout << "Data must be loaded first!" << endl; }
                break;
            
            case 9:
                cout << "Goodbye!";
                return 0;
            
            default:
                cout << "Invalid selection, please try again." << endl;
                break;
        }
    }
    
    return 0;
}



// function definitions
void printMenu() {
    cout << "Please enter the number of your selection and press enter: " << endl;
    cout << "1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "9. Exit" << endl;
}

void loadData() {
    cout << "Enter data file name: ";
    string path;
    cin >> path;
    ifstream in(path);
    if (!in){ cout << "Error opening file.\n"; return; }

    courses.clear();
    string line;
    while (getline(in, line)){
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        auto t = splitCSV(line);
        if (t.size() < 2) continue; // bad line; skip

        Course c;
        c.number = upper(t[0]);
        c.title  = t[1];
        for (size_t i=2;i<t.size();++i)
            if(!t[i].empty()) c.prereqs.push_back(upper(t[i]));
        courses.push_back(c);
    }
    fileLoaded = !courses.empty();
    cout << "Loaded " << courses.size() << " course(s).\n";
}


static bool byCourse(const Course& a, const Course& b) {
    return a.number < b.number; // alphanumeric compare by course code
}


void printList() {
    vector<Course> tmp = courses;
    sort(tmp.begin(), tmp.end(), byCourse);
    for (const auto& c : tmp) {
        cout << c.number << ", " << c.title << "\n";
    }
}

void printCourse() {
    cout << "Enter course number: ";
    string key; cin >> key; key = upper(key);

    int idx = findIndex(key);
    if (idx == -1){ cout << "Course not found.\n"; return; }

    const Course& c = courses[idx];
    cout << c.number << ", " << c.title << "\n";
    if (c.prereqs.empty()){ cout << "No prerequisites\n"; return; }

    cout << "Prerequisites:\n";
    for (auto& pre : c.prereqs){
        int p = findIndex(pre);
        if (p != -1) cout << "  " << courses[p].number << " - " << courses[p].title << "\n";
        else         cout << "  " << pre << " (not found)\n";
    }
}