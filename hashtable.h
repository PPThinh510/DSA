#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "candidate.h"

const int TABLE_SIZE = 26;

struct Node {
    Candidate data;
    Node* next;
};

struct HashTable {
    Node* bucket[TABLE_SIZE];
};

void initTable(HashTable& table);

int hashFunction(string fullName);

void insertCandidate(HashTable& table, Candidate c);

void displayAll(HashTable table);

void distributeToRooms(HashTable table, int numRooms);

Node* findCandidate(HashTable& table, string examId);

bool deleteCandidate(HashTable& table, string examId);

bool updateCandidate(HashTable& table, string examId);

void searchByFullName(HashTable table, string fullName);
void searchByHometown(HashTable table, string hometown);
string generateExamId(HashTable table);
void thongKeGioiTinh(HashTable table, int numRooms);

// Ham tien ich chuan hoa va kiem tra
string normalizeName(string name);
bool isValidDate(string dateStr);
bool isValidGender(string& genderStr);
string normalizeExamId(string id);

#endif // HASHTABLE_H
