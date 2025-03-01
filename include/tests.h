#ifndef TESTS_H
#define TESTS_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>


bool compareFiles(FILE* file1, FILE* file2);
void executeTests(int commandCount, const char* expectedOutputFolder, const char* actualOutputFolder);

#endif