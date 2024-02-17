/* Assignment 4
 * Name: Joel Harder
 * ID:   1191485
 * Date: March 28, 2023
 * BST by Greedy Technique
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFFSIZE 4096
#define WORD_LEN 64

typedef struct word {
    char* str;
    unsigned int count;
    double p; // probability of word
} word;


char* readFile(char* fileName);
word* findOccurences(char* string, int* numWords);
void calcProbabilities(word* words, int length, int wordCount);
void addWord(char* str, word** words, int* length);
void getWord(char* buffer);
void freeAll(word* words, double** C, int** R, int length);
int greedySearch(char* key, word* words, int left, int right);

// this is a comparison function to be used in the qsort() function
int compareStr(const void* a, const void* b) {
    word* word1 = (word*)a;
    word* word2 = (word*)b;
    return strcmp(word1->str, word2->str);
}


int main( void ) {
    int numKeys = 0;
    char* input = malloc(sizeof(char) * WORD_LEN);

    char* fileStr = readFile("data_A4_Q1.txt");

    word* occurs = findOccurences(fileStr, &numKeys);

    // sort the array of occurences
    qsort(occurs, numKeys, sizeof(word), compareStr);

    getWord(input);
    greedySearch(input, occurs, 0, numKeys-1);

    // free everything
    for (int i = 0; i < numKeys; i++) {
        free(occurs[i].str);
    }
    free(occurs);
    free(fileStr);
    free(input);
    return 0;
}


// recursive function that finds the highest probability in a tree
// and moves left/right until it finds the key or exhausts all searches
int greedySearch(char* key, word* words, int left, int right) {
    int max = left;

    // find the max
    for (int i = left; i <= right; i++) {
        if (words[i].p > words[max].p) {
            max = i;
        }
    }

    int compare = strcmp( words[max].str, key );

    if ( left >= right && compare != 0 ) {
        printf("Not found.\n");
        return -1;
    }

    printf("compared with '%s' (%.3f), ", words[max].str, words[max].p);

    if (compare == 0) {
        printf("found.\n");
        return 1;
    } else {
        if (compare > 0) {
            printf("going left subtree.\n");
            return greedySearch(key, words, left, max-1);
        } else {
            printf("going right subtree.\n");
            return greedySearch(key, words, max+1, right);
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