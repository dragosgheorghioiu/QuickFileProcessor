#ifndef  HELPERS_H_INCLUDED
#define HELPERS_H_INCLUDED

#include <math.h>
#include "lists.h"

// search if there exists a base so that base^exponent = number using binary search
int binarySearchBase(int n, int l, int r, int power) {
    if (r < l) {
        return -1;
    }

    int mid = l + (r - l) / 2;

    // found the base
    if (pow(mid, power) == n) {
        return mid;
    }

    // check left half for base
    if (pow(mid, power) > n) {
        return binarySearchBase(n, l, mid - 1, power);
    }

    // check right half for base
    if (pow(mid, power) < n) {
        return binarySearchBase(n, mid + 1, r, power);
    }

    return -1;
}

// search if a number is a perfect power by using binary search on every possible exponent
void checkIfPerfectPower(int n, int reducerNumber, int id, node* perfectPowers[1000][1000]) {
    for (int i = 2; i <= reducerNumber + 1; i++) {
        int base = binarySearchBase(n, 1, n, i);
        // if base is found, add the number to the list at position i - 2 to not have exponents starting from 0
        if (base != -1) {
            append(&perfectPowers[id][i - 2], n);
        }
    }
}

#endif