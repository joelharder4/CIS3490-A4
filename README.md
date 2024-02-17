# Assignment 4
> Course Code: **CIS\*3490**<br>
> Author: **Joel Harder**<br>
> Last Edited: **March 28, 2023**

## Description

#### Problem 1 - Word Frequency Search
> A dynamic and a greedy algorithm which will first prompt for a file to read from, then will prompt for a word to find.

`P11.c` - Dynamic programming optimal binary tree for word frequency search<br>
`P12.c` - Greedy algorithm for assembling an optimal binary tree for word frequency search

You may use the file `data_P1.txt` to test since it contains a lot of text so you can search through a larger volume of words.

#### Problem 2 - Stable Marriage Problem
> The goal of the Stable Marriage Problem is to find a stable match between two equally sized sets of elements (often referred to as "men" and "women") given a preference list for each element representing how much they would like to be married to each other person. A match is considered stable if there are no two elements who would both rather have each other than their current partners. The program will prompt for a file to read from which must be in the correct format, and then it will calculate all stable marriages and print them.

`P21.c` - Solves the Stable Marriage Problem given a valid file

An example for a valid file can be found in `data_P21.txt`. The first number is how many marriages there will be, and next is two `n x n` arrays of integers (first for men, second for women) showing their preferences. Each row represents one person, and each column is their most prefered to least preferred from left to right.

## Compiling
To compile all programs type
```
make all
```

<br>To compile individual programs type
```
make P<problem number>
```

## Running
To run any program type
```
./P<problem number>
```
