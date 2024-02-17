/* Assignment 4
 * Name: Joel Harder
 * ID:   1191485
 * Date: March 28, 2023
 * Stable Marriage Problem
*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define INPUT_LEN 512

typedef struct rating {
    int mRating;
    int wRating;
} rating;

int readFile(char* fileName, int*** men, int*** women);
void stableMarriage(int** men, int** women, int*** marriages, rating** preferences, int nPairs);
void makeRankingTable(int** men, int** women, rating*** preferences, int nPairs);
void getFileName(char* buffer);
void freeAll(int** men, int** women, rating** prefs, int** marriages, int length);

int main(void) {
    int** men = NULL;
    int** women = NULL;
    rating** preferences = NULL;
    int** marriages = NULL;
    char fileName[INPUT_LEN];

    getFileName(fileName);
    int nPairs = readFile(fileName, &men, &women);

    if (nPairs <= 0) {
        return 1;
    }
    
    makeRankingTable(men, women, &preferences, nPairs);

    // allocate marriages array
    marriages = malloc(sizeof(int*) * nPairs);
    for (int i = 0; i < nPairs; i++) {
        marriages[i] = calloc(nPairs, sizeof(int));
    }

    stableMarriage(men, women, &marriages, preferences, nPairs);

    //printf("match table:\n");
    for (int i = 0; i < nPairs; i++) {
        for (int j = 0; j < nPairs; j++) {
            printf("%d ", marriages[i][j]);
        }
        printf("\n");
    }
    
    freeAll(men, women, preferences, marriages, nPairs);
    return 0;
}



void freeAll(int** men, int** women, rating** prefs, int** marriages, int length) {
    for (int i = 0; i < length; i++) {
        free(men[i]);
        free(women[i]);
        free(prefs[i]);
        free(marriages[i]);
    }
    free(men);
    free(women);
    free(prefs);
    free(marriages);
}




void makeRankingTable(int** men, int** women, rating*** preferences, int nPairs) {
    int match;

    // allocate the 2d array
    (*preferences) = malloc(sizeof(rating*) * (nPairs+1));
    for (int i = 0; i < nPairs; i++) {
        (*preferences)[i] = malloc(sizeof(rating) * (nPairs+1));
    }

    for (int i = 0; i < nPairs; i++) {
        for (int j = 0; j < nPairs; j++) {
            match = men[i][j];
            (*preferences)[i][match-1].mRating = j + 1;
        }
    }

    for (int i = 0; i < nPairs; i++) {
        for (int j = 0; j < nPairs; j++) {
            match = women[i][j];
            (*preferences)[match-1][i].wRating = j + 1;
        }
    }
}


void stableMarriage(int** men, int** women, int*** marriages, rating** preferences, int nPairs) {
    int freeMen[nPairs];
    int freeWomen[nPairs];
    int numFreeMen = nPairs;
    int response, man, woman, otherMan;

    // initialize freeMen and freeWomen
    for (int i = 0; i < nPairs; i++) {
        freeMen[i] = 1;
        freeWomen[i] = 1;
    }

    while (numFreeMen > 0) {
        for (int i = 0; i < nPairs; i++) {
            if (freeMen[i] == 1) {
                // get the index of the current free man to propose
                man = i;
                break;
            }
        }

        response = 0;
        for (int i = 0; i < nPairs; i++) {
            woman = men[man][i] - 1;

            if (freeWomen[woman] == 1) {
                response = 1;
                numFreeMen--;
            } else {
                // get the index of the man she is with right now
                for (int j = 0; j < nPairs; j++) {
                    if ((*marriages)[j][woman] == 1) {
                        otherMan = j;
                        break;
                    }
                }

                // if she likes the new man more
                if (preferences[man][woman].wRating < preferences[otherMan][woman].wRating) {
                    response = 1;
                    freeMen[otherMan] = 1;
                    (*marriages)[otherMan][woman] = 0;
                } else {
                    response = 0;
                }
            }

            if (response == 1) {
                freeMen[man] = 0;
                freeWomen[woman] = 0;
                (*marriages)[man][woman] = 1;
                break;
            }
        }
    }
}



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



int readFile(char* fileName, int*** men, int*** women) {
    int n = 0;
    FILE* fd = fopen(fileName, "r");

    if (fd == NULL) {
        fprintf(stderr, "could not open file\n");
        return -1;
    }

    fscanf(fd, "%d\n", &n);

    // mens preferences
    (*men) = malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++) {

        (*men)[i] = malloc(sizeof(int) * n);
        for (int j = 0; j < n; j++) {
            fscanf(fd, "%d ", &((*men)[i][j]));
        }
    }

    // womens preferences
    (*women) = malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++) {

        (*women)[i] = malloc(sizeof(int) * n);
        for (int j = 0; j < n; j++) {
            fscanf(fd, "%d ", &((*women)[i][j]));
        }
    }

    fclose(fd);
    return n;
}