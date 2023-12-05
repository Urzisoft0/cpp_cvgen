#include <iostream>
#include "Profiler.h"
#include <vector>
#define THRESHOLD 100
Profiler prof("Hybrid Quick Sort");

int partition(int *a, int left, int right, Operation sel, Operation comp) {
    int p = a[right];
    sel.count();
    int j = left;

    for (int i = left; i <= right; i++) {
        if (a[i] < p) {
            comp.count();
            std::swap(a[j], a[i]);
            sel.count(3);
            j++;
        }
    }
    std::swap(a[j], a[right]);
    sel.count(3);

    return j;
}

void insertionSort(int a[], int left, int right, Operation sel, Operation comp) {
    for (int i = left + 1; i < right + 1; i++) {
        int val = a[i];
        sel.count();
        int j = i - 1;

        while (j >= left && a[j] > val) {
            comp.count();
            a[j + 1] = a[j];
            sel.count();
            j = j - 1;
        }
        comp.count();

        a[j + 1] = val;
        sel.count();
    }
}

void quickSort(int *a, int left, int right, int n) {
    Operation qsComp = prof.createOperation("quick_comp", n);
    Operation qsAssig = prof.createOperation("quick_assig", n);

    if (right > left) {
        int part = partition(a, left, right, qsAssig, qsComp);

        quickSort(a, left, part - 1, n);
        quickSort(a, part + 1, right, n);
    }
}

void hybrid(int arr[], int left, int right, Operation sel, Operation comp) {
    while (left < right) {
        if (right - left + 1 < THRESHOLD) {
            insertionSort(arr, left, right, sel, comp);
            break;
        }
        else {
            int p = partition(arr, left, right, sel, comp);
            hybrid(arr, left, p - 1, sel, comp);
            left = p + 1;
            hybrid(arr, p + 1, right, sel, comp);
            right = p - 1;
        }
    }
}

void print(int *a, int n) {
    for (int i = 0; i < n; i++)
        std::cout << a[i] << " "; 
}
        
//void test() {
//    int a[50];
//    int n = 10;
//    FillRandomArray(a, n, 0, 20032, true, UNSORTED);
//    hybrid(a, 0, n - 1);
//    print(a, n);
//}

void perf() {
    int starting_size = 100;
    int final_size = 10000;

    for (int i = 1; i <= 5; i++) {
        for (int n = starting_size; n <= final_size; n += 100) {
            int *a = (int *) malloc(sizeof(int) * 200000);
            int *b = (int *) malloc(sizeof(int) * 200000);

            FillRandomArray(a, n, 0, 10000, true, UNSORTED);
            CopyArray(b, a, n);

            Operation sel = prof.createOperation("select", n);
            Operation comp = prof.createOperation("comp", n);

            hybrid(a, 0, n - 1, sel, comp);
            quickSort(b, 0, n - 1, n);
        }
    }

    prof.divideValues("select", 5);
    prof.divideValues("comp", 5);
    prof.divideValues("quick_assig", 5);

    prof.divideValues("quick_comp", 5);
    prof.createGroup("COMP", "comp", "quick_comp");
    prof.createGroup("SELECT", "select", "quick_assig");

    prof.showReport();
}

int main() {
    //test();
    perf();
    return 0;
}
