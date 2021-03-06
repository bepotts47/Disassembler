//
// Created by brandon on 11/1/15.
//

#include "symbolTable.h"
#include "fileProcessing.h"
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_SIZE 150


/***
 * Creates a valid SymbolData
 */
void symbolInit(Symbol* symbol){

    symbol->name = (char *)calloc(100, sizeof(char *));
    symbol->type = (char *)calloc(100, sizeof(char *));
    symbol->value = (char *)calloc(100, sizeof(char *));
    symbol->address = (char *)calloc(100, sizeof(char *));
    symbol->printed = false;

}

void symbolFree(Symbol* symbol){

    free(symbol->type);
    free(symbol->name);
    free(symbol->value);
    free(symbol->address);
}

void symbolReset(Symbol* symbol){

    memset(symbol->name, 0 , sizeof(symbol->name));
    memset(symbol->type, 0 , sizeof(symbol->type));
    memset(symbol->value, 0 , sizeof(symbol->value));
    memset(symbol->address, 0 , sizeof(symbol->address));
    symbol->printed = false;
}


void symbolTableInit(SymbolTable* symbolTable){

    symbolTable->table = (Symbol *)malloc(100 * sizeof(Symbol *));
    symbolTable->size = 0;
}

/***
 * Inserts a value to the table
 */
void insertValueToTable(char* value, char* address, SymbolTable* symbolTable){

    // increment to the next empty value
    for(int i = 0; i < symbolTable->size; i++){
        symbolTable->table++;
    }


    symbolTable->table->address = (char *)calloc(100, sizeof(char *));
    symbolTable->table->value = (char *)calloc(100, sizeof(char *));
    symbolTable->table->type = (char *)calloc(100, sizeof(char *));
    symbolTable->table->name = (char *)calloc(100, sizeof(char *));
    symbolTable->table->printed = false;

    // handles the naming
    char* tempNameArray = (char *)calloc(100, sizeof(char *));
    symbolTable->size++;
    strcpy(symbolTable->table->name, "V0");
    sprintf(tempNameArray, "%d", symbolTable->size);
    strcat(symbolTable->table->name, tempNameArray);

    // copies over the type
    strcpy(symbolTable->table->type, ".word");

    // copies over the value
    strcpy(symbolTable->table->value, value);

    // copies over the address
    strcpy(symbolTable->table->address, address);

    free(tempNameArray);


}



void copySymbolToTable(Symbol* symbol, SymbolTable* symbolTable){

    symbolInit(symbolTable->table);
    strcpy(symbolTable->table->name, symbol->name);
    strcpy(symbolTable->table->value, symbol->value);
    strcpy(symbolTable->table->type, symbol->type);
    strcpy(symbolTable->table->address, symbol->address);
    symbolTable->table->printed = false;
}


void getSymbolByAddress(char* address, SymbolTable* symbolTable, Symbol* symbol){

    // Iterates over the entire symbol array
    for(int i = 0; i < symbolTable->size; i++){

        // will only be true if the addresses are equal
        if(strcmp(symbolTable->table->address, address) == 0){

            symbolCopy(symbol, symbolTable->table);
            symbolTable->table->printed = true;
            break;
        }
        symbolTable->table++;
    }
}


void symbolCopy(Symbol* destSymbol, Symbol* copySymbol){

    strcpy(destSymbol->type, copySymbol->type);
    strcpy(destSymbol->address, copySymbol->address);
    strcpy(destSymbol->name, copySymbol->name);
    strcpy(destSymbol->value, copySymbol->value);
    destSymbol->printed = false;
}


void printSymbolTable(FILE* outputFile, SymbolTable* symbolTable, bool variablesPrinted){

    char* pLine = (char *)calloc(100, sizeof(char *));
    Symbol symbolAtIndex;
    bool space = true;

    // prints the .data text
    printToOutputFile(false, ".data\n", outputFile);

    // Iterates over the entire symbolArray
    for(unsigned int i = 0; i < symbolTable->size; i++){

        symbolAtIndex = *symbolTable->table;

        // will be true when variables have been refrenced
        if(symbolAtIndex.printed == true) {
            // builds and prints each line of the '.data' section
            strcat(pLine, symbolAtIndex.name);
            strcat(pLine, ":      ");
            strcat(pLine, symbolAtIndex.type);
            strcat(pLine, " ");
            // add an extra space just for zero values
            if (strcmp(symbolAtIndex.value, "0") == 0) {
                strcat(pLine, " ");
            }
            space = false;
        } else {
            space = true;
        }
        strcat(pLine, symbolAtIndex.value);
        strcat(pLine, "\n");
        printToOutputFile(space, pLine, outputFile);
        symbolTable->table++;
        memset(pLine, '\0', sizeof(pLine));
    }

    free(pLine);

}


void freeSymbolTable(SymbolTable* symbolTable){

    Symbol* symbol;
    Symbol* beginningSymbol = *&symbolTable->table;
    // iterates over the entire array deallocating values
    for(int i = 0; i < symbolTable->size; i++){

        free(symbolTable->table->name);
        free(symbolTable->table->address);
        free(symbolTable->table->type);
        free(symbolTable->table->value);
        symbolTable->table++;
    }

    symbolTable->table = *&beginningSymbol;
    free(symbolTable->table);
}