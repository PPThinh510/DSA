#include "file.h"

#include <fstream>
#include <sstream>

using namespace std;

void saveToFile(HashTable table, string filename) {

    ofstream outFile(filename);

    for (int i = 0; i < TABLE_SIZE; i++) {

        Node* current = table.bucket[i];

        while (current != nullptr) {

            outFile << current->data.examId << "|"
                    << current->data.fullName << "|"
                    << current->data.gender << "|"
                    << current->data.birthDate << "|"
                    << current->data.hometown << endl;

            current = current->next;
        }
    }

    outFile.close();
}

void loadFromFile(HashTable& table, string filename) {

    ifstream inFile(filename);

    if (!inFile.is_open())
        return;

    string line;

    while (getline(inFile, line)) {

        stringstream ss(line);

        Candidate c;

        getline(ss, c.examId, '|');
        getline(ss, c.fullName, '|');
        getline(ss, c.gender, '|');
        getline(ss, c.birthDate, '|');
        getline(ss, c.hometown);

        insertCandidate(table, c);
    }

    inFile.close();
}