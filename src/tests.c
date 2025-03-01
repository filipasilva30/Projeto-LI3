#include "tests.h"

bool compareFiles(FILE* file1, FILE* file2) {
    char line1[1024];
    char line2[1024];
    int lineNum = 1;

    if (fgets(line1, sizeof(line1), file1) == NULL && fgets(line2, sizeof(line2), file2) == NULL) {
        return true;
    }

    rewind(file1);
    rewind(file2);

    while (fgets(line1, sizeof(line1), file1) != NULL) {
        if (fgets(line2, sizeof(line2), file2) == NULL) {
            printf("Os arquivos são diferentes na %d: o arquivo criado continua para além do esperado.\n", lineNum);
            return false;
        }

        if (strcmp(line1, line2) != 0) {
            // Linhas são diferentes
            printf("Os arquivos são diferentes na linha %d.\n", lineNum);
            printf("Obtido: %sEsperado: %s\n", line1, line2);
            return false;
        }

        lineNum++;
    }

    if (fgets(line2, sizeof(line2), file2) != NULL) {
        printf("Os arquivos são diferentes na linha %d: o arquivo criado termina mais cedo que o esperado.\n", lineNum);
        return false;
    }

    return true; // Os arquivos são iguais
}

void executeTests(int commandCount, const char* expectedOutputFolder, const char* actualOutputFolder) {
    for (int i = 1; i <= commandCount; i++) {
        char outputFilePath[1024];
        char expectedFilePath[1024];
        sprintf(outputFilePath, "%s/command%d_output.txt", actualOutputFolder, i);
        sprintf(expectedFilePath, "%s/command%d_output.txt", expectedOutputFolder, i);

        FILE* outputFile = fopen(outputFilePath, "r");
        FILE* expectedFile = fopen(expectedFilePath, "r");

        if (!outputFile || !expectedFile) {
            if (outputFile) fclose(outputFile);
            if (expectedFile) fclose(expectedFile);
            continue;
        }

        bool isSame = compareFiles(outputFile, expectedFile);
        if (isSame) {
            printf("Teste %d passou.\n\n", i);
        } else {
            printf("Teste %d falhou.\n\n", i);
        }

        fclose(outputFile);
        fclose(expectedFile);
    }
}