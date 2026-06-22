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
    node->next = table.bucket[index];

    table.bucket[index] = node;
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

    Candidate oldData = node->data;

    cout << "Ho ten moi: ";
    getline(cin >> ws, node->data.fullName);

    cout << "Gioi tinh moi: ";
    getline(cin, node->data.gender);

    cout << "Ngay sinh moi: ";
    getline(cin, node->data.birthDate);

    cout << "Que quan moi: ";
    getline(cin, node->data.hometown);

    int oldIndex = hashFunction(oldData.fullName);
    int newIndex = hashFunction(node->data.fullName);

    if (oldIndex != newIndex) {

        Candidate temp = node->data;

        deleteCandidate(table, examId);

        insertCandidate(table, temp);
    }

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