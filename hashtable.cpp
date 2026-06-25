#include "hashtable.h"
#include "color.h"

#include <iostream>
#include <iomanip>
#include <cctype>

using namespace std;

void initTable(HashTable& table) {

    for (int i = 0; i < TABLE_SIZE; i++) {
        table.bucket[i] = nullptr;
    }
}

string getLastName(string fullName) {

    while (!fullName.empty() && fullName.back() == ' ')
        fullName.pop_back();

    size_t pos = fullName.find_last_of(' ');

    if (pos == string::npos)
        return fullName;

    return fullName.substr(pos + 1);
}

int hashFunction(string fullName) {

    string lastName = getLastName(fullName);

    if (lastName.empty())
        return 0;

    char firstChar = toupper(lastName[0]);

    if (firstChar < 'A' || firstChar > 'Z')
        return 0;

    return firstChar - 'A';
}

Node* findCandidate(HashTable& table, string examId) {

    for (int i = 0; i < TABLE_SIZE; i++) {

        Node* current = table.bucket[i];

        while (current != nullptr) {

            if (current->data.examId == examId)
                return current;

            current = current->next;
        }
    }

    return nullptr;
}

void insertCandidate(HashTable& table, Candidate c) {

    if (findCandidate(table, c.examId) != nullptr) {
        cout << RED << "Ma du thi da ton tai!\n" << RESET;
        return;
    }

    int index = hashFunction(c.fullName);

    Node* node = new Node;
    node->data = c;
    node->next = nullptr;

    if (table.bucket[index] == nullptr || table.bucket[index]->data.fullName >= c.fullName) {
        node->next = table.bucket[index];
        table.bucket[index] = node;
    } else {
        Node* current = table.bucket[index];
        while (current->next != nullptr && current->next->data.fullName < c.fullName) {
            current = current->next;
        }
        node->next = current->next;
        current->next = node;
    }
}

void displayAll(HashTable table) {

    cout << BOLD << BLUE;
    cout << left
         << setw(12) << "Ma DT"
         << setw(30) << "Ho ten"
         << setw(12) << "Gioi tinh"
         << setw(15) << "Ngay sinh"
         << setw(20) << "Que quan"
         << setw(10) << "Nhom"
         << endl << RESET;

    cout << string(100, '-') << endl;

    for (int i = 0; i < TABLE_SIZE; i++) {

        Node* current = table.bucket[i];

        while (current != nullptr) {

            cout << left
                 << setw(12) << current->data.examId
                 << setw(30) << current->data.fullName
                 << setw(12) << current->data.gender
                 << setw(15) << current->data.birthDate
                 << setw(20) << current->data.hometown
                 << char(i + 'A')
                 << endl;

            current = current->next;
        }
    }
}

void distributeToRooms(HashTable table, int numRooms) {

    if (numRooms <= 0) return;

    for (int r = 0; r < numRooms; r++) {

        cout << BOLD << MAGENTA << "\n=== PHONG THI " << r + 1 << " ===\n" << RESET;
        cout << BLUE;
        cout << left
             << setw(12) << "Ma DT"
             << setw(30) << "Ho ten"
             << setw(15) << "Ngay sinh" << RESET
             << endl;
        cout << string(60, '-') << endl;

        int globalCount = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table.bucket[i];
            while (current != nullptr) {
                // Phan phoi xoay vong (Round-robin)
                if (globalCount % numRooms == r) {
                    cout << left
                         << setw(12) << current->data.examId
                         << setw(30) << current->data.fullName
                         << setw(15) << current->data.birthDate
                         << endl;
                }
                globalCount++;
                current = current->next;
            }
        }
    }
}

bool deleteCandidate(HashTable& table, string examId) {

    for (int i = 0; i < TABLE_SIZE; i++) {

        Node* current = table.bucket[i];
        Node* previous = nullptr;

        while (current != nullptr) {

            if (current->data.examId == examId) {

                if (previous == nullptr)
                    table.bucket[i] = current->next;
                else
                    previous->next = current->next;

                delete current;

                return true;
            }

            previous = current;
            current = current->next;
        }
    }

    return false;
}

bool updateCandidate(HashTable& table, string examId) {

    Node* node = findCandidate(table, examId);

    if (node == nullptr)
        return false;

    Candidate updatedData = node->data;

    cout << "Ho ten moi: ";
    string nameInput;
    getline(cin >> ws, nameInput);
    updatedData.fullName = normalizeName(nameInput);

    string genderInput;
    do {
        cout << "Gioi tinh moi (Nam/Nu): ";
        getline(cin, genderInput);
        if (isValidGender(genderInput)) {
            updatedData.gender = genderInput;
            break;
        }
        cout << RED << "Gioi tinh khong hop le! Vui long nhap lai (Nam/Nu).\n" << RESET;
    } while (true);

    string dateInput;
    do {
        cout << "Ngay sinh moi (DD/MM/YYYY): ";
        getline(cin, dateInput);
        if (isValidDate(dateInput)) {
            updatedData.birthDate = dateInput;
            break;
        }
        cout << RED << "Ngay sinh khong hop le! Vui long nhap lai dung dinh dang DD/MM/YYYY.\n" << RESET;
    } while (true);

    cout << "Que quan moi: ";
    getline(cin, updatedData.hometown);
    updatedData.hometown = normalizeName(updatedData.hometown);

    deleteCandidate(table, examId);
    insertCandidate(table, updatedData);

    return true;
}

void searchByFullName(HashTable table, string fullName) {
    bool found = false;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table.bucket[i];
        
        while (current != nullptr) {
            if (current->data.fullName.find(fullName) != string::npos) {
                if (!found) {
                    cout << BOLD << YELLOW << "\n=== KET QUA TIM KIEM ===\n" << RESET;
                    cout << BOLD << BLUE;
                    cout << left
                         << setw(12) << "Ma DT"
                         << setw(30) << "Ho ten"
                         << setw(12) << "Gioi tinh"
                         << setw(15) << "Ngay sinh"
                         << setw(20) << "Que quan"
                         << endl << RESET;
                    cout << string(100, '-') << endl;
                }
                
                cout << left
                     << setw(12) << current->data.examId
                     << setw(30) << current->data.fullName
                     << setw(12) << current->data.gender
                     << setw(15) << current->data.birthDate
                     << setw(20) << current->data.hometown
                     << endl;
                
                found = true;
            }
            current = current->next;
        }
    }
    
    if (!found) {
        cout << RED << "Khong tim thay!\n" << RESET;
    }
}

string generateExamId(HashTable table) {
    int maxIdNum = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table.bucket[i];
        while (current != nullptr) {
            string idStr = current->data.examId;
            if (idStr.length() > 2 && toupper(idStr[0]) == 'T' && toupper(idStr[1]) == 'S') {
                string numStr = idStr.substr(2);
                bool allDigits = true;
                for (char c : numStr) {
                    if (!isdigit(c)) {
                        allDigits = false;
                        break;
                    }
                }
                if (allDigits && !numStr.empty()) {
                    int num = stoi(numStr);
                    if (num > maxIdNum) {
                        maxIdNum = num;
                    }
                }
            }
            current = current->next;
        }
    }
    string nextIdStr = to_string(maxIdNum + 1);
    while (nextIdStr.length() < 3) {
        nextIdStr = "0" + nextIdStr;
    }
    return "TS" + nextIdStr;
}

void thongKeGioiTinh(HashTable table, int numRooms) {
    int tongNam = 0;
    int tongNu = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table.bucket[i];
        while (current != nullptr) {
            if (current->data.gender == "Nam" || current->data.gender == "nam") {
                tongNam++;
            } else if (current->data.gender == "Nu" || current->data.gender == "nu" || current->data.gender == "Nữ" || current->data.gender == "nữ") {
                tongNu++;
            }
            current = current->next;
        }
    }
    
    cout << "Tong Nam: " << tongNam << endl;
    cout << "Tong Nu: " << tongNu << endl;
    if (numRooms <= 0) return;
    for (int r = 0; r < numRooms; r++) {
        int namPhong = 0;
        int nuPhong = 0;
        int globalCount = 0;
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table.bucket[i];
            while (current != nullptr) {
                if (globalCount % numRooms == r) {
                    if (current->data.gender == "Nam" || current->data.gender == "nam") {
                        namPhong++;
                    } else if (current->data.gender == "Nu" || current->data.gender == "nu" || current->data.gender == "Nữ" || current->data.gender == "nữ") {
                        nuPhong++;
                    }
                }
                globalCount++;
                current = current->next;
            }
        }
        cout << "Phong " << r + 1 << " - Nam: " << namPhong << ", Nu: " << nuPhong << endl;
    }
}

void searchByHometown(HashTable table, string hometown) {
    bool found = false;
    
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* current = table.bucket[i];
        
        while (current != nullptr) {
            if (current->data.hometown.find(hometown) != string::npos) {
                if (!found) {
                    cout << BOLD << YELLOW << "\n=== KET QUA TIM KIEM ===\n" << RESET;
                    cout << BOLD << BLUE;
                    cout << left
                         << setw(12) << "Ma DT"
                         << setw(30) << "Ho ten"
                         << setw(12) << "Gioi tinh"
                         << setw(15) << "Ngay sinh"
                         << setw(20) << "Que quan"
                         << endl << RESET;
                    cout << string(100, '-') << endl;
                }
                
                cout << left
                     << setw(12) << current->data.examId
                     << setw(30) << current->data.fullName
                     << setw(12) << current->data.gender
                     << setw(15) << current->data.birthDate
                     << setw(20) << current->data.hometown
                     << endl;
                
                found = true;
            }
            current = current->next;
        }
    }
    
    if (!found) {
        cout << RED << "Khong tim thay!\n" << RESET;
    }
}

//Utility Functions

string normalizeName(string name) {
    string result = "";
    bool space = true;
    for (char c : name) {
        if (isspace(c)) {
            if (!space) {
                result += ' ';
                space = true;
            }
        } else {
            result += c;
            space = false;
        }
    }
    if (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }
    bool newWord = true;
    for (size_t i = 0; i < result.length(); i++) {
        if (result[i] == ' ') {
            newWord = true;
        } else {
            if (newWord) {
                result[i] = toupper(result[i]);
                newWord = false;
            } else {
                result[i] = tolower(result[i]);
            }
        }
    }
    return result;
}

bool isValidDate(string dateStr) {
    if (dateStr.length() != 10) return false;
    if (dateStr[2] != '/' || dateStr[5] != '/') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(dateStr[i])) return false;
    }
    int d = stoi(dateStr.substr(0, 2));
    int m = stoi(dateStr.substr(3, 2));
    int y = stoi(dateStr.substr(6, 4));
    
    if (y < 1900 || y > 2100) return false;
    if (m < 1 || m > 12) return false;
    
    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) {
        daysInMonth[2] = 29;
    }
    
    if (d < 1 || d > daysInMonth[m]) return false;
    return true;
}

bool isValidGender(string& genderStr) {
    string g = "";
    for (char c : genderStr) {
        if (!isspace(c)) g += tolower(c);
    }
    if (g == "nam") {
        genderStr = "Nam";
        return true;
    }
    if (g == "nu" || g == "nữ" || g == "nữ" || g == "nu'") {
        genderStr = "Nu";
        return true;
    }
    return false;
}

string normalizeExamId(string id) {
    string res = "";
    for (char c : id) {
        if (!isspace(c)) res += toupper(c);
    }
    return res;
}