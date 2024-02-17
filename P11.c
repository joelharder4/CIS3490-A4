/* Assignment 4
 * Name: Joel Harder
 * ID:   1191485
 * Date: March 28, 2023
 * BST by Dynamic Programming
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFFSIZE 4096
#define WORD_LEN 64
#define INPUT_LEN 512

typedef struct word {
    char* str;
    unsigned int count;
    double p; // probability of word
} word;


char* readFile(char* fileName);
void dynamicBST(word* words, int nKeys, double** C, int** R);
word* findOccurences(char* string, int* numWords);
void calcProbabilities(word* words, int length, int wordCount);
void addWord(char* str, word** words, int* length);
void searchTree(char* input, double** C, int** R, word* words, int nKeys);
void getWord(char* buffer);
void freeAll(word* words, double** C, int** R, int length);
void getFileName(char* buffer);


// this is a comparison function to be used in the qsort() function
int compareStr(const void* a, const void* b) {
    word* word1 = (word*)a;
    word* word2 = (word*)b;
    return strcmp(word1->str, word2->str);
}


int main( void ) {
    int numKeys = 0;
    double** C = NULL;
    int** R = NULL;
    char* input = malloc(sizeof(char) * WORD_LEN);
    char fileName[INPUT_LEN];

    getFileName(fileName);
    char* fileStr = readFile(fileName);

    word* occurs = findOccurences(fileStr, &numKeys);

    // sort the list of occurences
    qsort(occurs, numKeys, sizeof(word), compareStr);

    C = malloc(sizeof(double*) * (numKeys+2));
    R = malloc(sizeof(int*) * (numKeys+2));
    for (int i = 0; i <= numKeys+1; i++) {
        C[i] = calloc((numKeys+1), sizeof(double));
        R[i] = calloc((numKeys+1), sizeof(int));
    }
    
    dynamicBST(occurs, numKeys, C, R);

    getWord(input);
    searchTree(input, C, R, occurs, numKeys);

    free(fileStr);
    free(input);
    freeAll(occurs, C, R, numKeys);
    return 0;
}


// frees all of the 2d arrays used in this solution
void freeAll(word* words, double** C, int** R, int length) {
    for (int i = 0; i <= length+1; i++) {
        if (i < length) free(words[i].str);
        free(C[i]);
        free(R[i]);
    }
    
    free(words);
    free(C);
    free(R);
}


// searches for any given word in the BST using the tables made in dynamicBST
void searchTree(char* input, double** C, int** R, word* words, int nKeys) {
    int i = 1;
    int j = nKeys;
    int curRoot;
    int comparison;

    while (1) {
        curRoot = R[i][j];

        comparison = strcmp(input, words[curRoot-1].str);
        printf("compared with '%s' (%.3f), ", words[curRoot-1].str, C[i][j]);

        if (comparison == 0) {
            printf("found.\n");
            break;
        } else if (comparison > 0) {
            printf("go right subtree.\n");
            i += (curRoot - i) + 1;
        } else {
            printf("go left subtree.\n");
            j -= (j - curRoot) + 1;
        }

        if (i >= j+1) {
            printf("match not found.\n");
            break;
        }
    }
}


// generates tables for each subproblem
void dynamicBST(word* words, int nKeys, double** C, int** R) {
    int j = 0;
    double sum;
    double minVal, curVal, kmin;

    for (int i = 1; i <= nKeys; i++) {
        C[i][i-1] = 0;
        C[i][i] = (words[i-1]).p;
        R[i][i] = i;
    }

    // start counting diagonally
    for (int d = 1; d < nKeys; d++) {
        for (int i = 1; i <= nKeys - d; i++) {
            j = i + d;
            minVal = 9999999.0;

            for (int k = i; k <= j; k++) {
                curVal = C[i][k-1] + C[k+1][j];
                if (curVal < minVal) {
                    minVal = curVal;
                    kmin = k;
                }
            }
            R[i][j] = kmin;

            sum = 0;
            for(int s = i-1; s < j; s++) {
                sum += (words[s]).p;
            }
            C[i][j] = minVal + sum;
        }
    }
}


// if the given string is already a word in words, it increases the count
// if it is not, it increases the length of words array and the new word
void addWord(char* str, word** words, int* length) {
    for (int i = 0; i < *length-1; i++) {
        if ( strcmp(str, ((*words)[i]).str) == 0 ) {
            ((*words)[i].count)++;
            return;
        }
    }

    word newWord;
    newWord.str = malloc(sizeof(char) * WORD_LEN);    
    strcpy(newWord.str, str);
    newWord.count = 1;

    *words = realloc(*words, sizeof(word) * (*length+1));
    (*words)[(*length)-1] = newWord;

    (*length)++;
}


// calculate the p value in each word struct
void calcProbabilities(word* words, int length, int wordCount) {
    for (int i = 0; i < length; i++) {
        (words[i]).p = (words[i]).count / (wordCount * 1.0);
    }
}


// finds all words in the given string and converts it into
// a word array to represent how common each word is
word* findOccurences(char* string, int* numWords) {
    char* delims = { " \n" };
    char* token;
    word* words = malloc(sizeof(word));
    int length = 1;
    int wordCount = 0;

    token = strtok(string, delims);

    while (token != NULL) {
        addWord(token, &words, &length);
        wordCount++;
        token = strtok(NULL, delims);
    }

    calcProbabilities(words, length-1, wordCount);

    *numWords = length-1;
    return words;
}


// get the user to input a search key
void getWord(char* buffer) {
    write(STDIN_FILENO, "Enter a word: ", sizeof(char)*15);
    
    int length = read(STDIN_FILENO, buffer, WORD_LEN);

    // if the buffer did not run out of room
    if (length < WORD_LEN && length > 1) {
        buffer[length-1] = '\0';
    } else {
        write(STDIN_FILENO, "Word is invalid. Please try again.\n\n", sizeof(char)*37);
        getWord(buffer);
    }
}

// prompt user for file name
void getFileName(char* buffer) {
    write(STDIN_FILENO, "Enter a file name: ", sizeof(char)*20);
    
    int length = read(STDIN_FILENO, buffer, INPUT_LEN);

    // if the buffer did not run out of room
    if (length < INPUT_LEN && length > 1) {
        buffer[length-1] = '\0';
    } else {
        write(STDIN_FILENO, "File name is invalid. Please try again.\n\n", sizeof(char)*42);
        getFileName(buffer);
    }
}


// reads a text file into a string
char* readFile( char* fileName ) {
    int fd = open(fileName, O_RDONLY);
    char* complete = NULL;
    int length = 1;
    int size;

    if ((size = lseek(fd, 0, SEEK_END)) > 0) {
        complete = malloc(sizeof(char) * (size+1));
    } else {
        printf("could not determine file length\n");
        return " ";
    }

    lseek(fd, 0, SEEK_SET);

    length = read(fd, complete, size);
    complete[length] = '\0';

    close(fd);
    return complete;
}


