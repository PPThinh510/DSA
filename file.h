#ifndef FILE_H
#define FILE_H

#include "hashtable.h"

void saveToFile(HashTable table, string filename);

void loadFromFile(HashTable& table, string filename);

#endif