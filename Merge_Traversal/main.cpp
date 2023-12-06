#include <iostream>
#include "Profiler.h"
#include <vector>
#define THRESHOLD 32
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

int partition(int *a, int left, int right) {
    int p = a[right];
    int j = left;

    for (int i = left; i <= right; i++) {
        if (a[i] < p) {
            std::swap(a[j], a[i]);
            j++;
        }
    }
    std::swap(a[j], a[right]);
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

void insertionSort(int a[], int left, int right) {
    for (int i = left + 1; i < right + 1; i++) {
        int val = a[i];
        int j = i - 1;

        while (j >= left && a[j] > val) {
            a[j + 1] = a[j];
            j = j - 1;
        }
        a[j + 1] = val;
    }
}

void quickSort(int *a, int left, int right, int n, Operation qsComp, Operation qsAssig) {
    while (left < right) {
        int p= partition(a, left, right, qsAssig, qsComp);

        if (p - left < right - p) {
            quickSort(a, left, p - 1, n, qsComp, qsAssig);
            left = p + 1;
        } else {
            quickSort(a, p + 1, right, n, qsComp, qsAssig);
            right = p - 1;
        }
    }
}

void quickSort(int *a, int left, int right, int n) {
    while (left < right) {
        int p= partition(a, left, right);

        if (p - left < right - p) {
            quickSort(a, left, p - 1, n);
            left = p + 1;
        } else {
            quickSort(a, p + 1, right, n);
            right = p - 1;
        }
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
            if (p - left < right - p) {
                hybrid(arr, left, p - 1, sel, comp);
                left = p + 1;
            } else {
                hybrid(arr, p + 1, right, sel, comp);
                right = p - 1;
            }
        }
    }
}

void hybrid(int arr[], int left, int right) {
    while (left < right) {
        if (right - left + 1 < THRESHOLD) {
            insertionSort(arr, left, right);
            break;
        }
        else {
            int p = partition(arr, left, right);
            if (p - left < right - p) {
                hybrid(arr, left, p - 1);
                left = p + 1;
            } else {
                hybrid(arr, p + 1, right);
                right = p - 1;
            }
        }
    }
}

void print(int *a, int n) {
    for (int i = 0; i < n; i++)
        std::cout << a[i] << " "; 
}
        
void test() {
    int a[200000];
    int n = 10000;
    FillRandomArray(a, n, 0, 20032, true, UNSORTED);
    hybrid(a, 0, n - 1);
    print(a, n);
}

void perf() {
    int starting_size = 100;
    int final_size = 10000;

    for (int n = starting_size; n <= final_size; n += 100) {
        for (int i = 1; i <= 10; i++) {
            int *a = (int *) malloc(sizeof(int) * 200000);
            int *b = (int *) malloc(sizeof(int) * 200000);

            FillRandomArray(a, n, 0, 10000, true, UNSORTED);
            CopyArray(b, a, n);

            Operation sel = prof.createOperation("hybrid_assig", n);
            Operation comp = prof.createOperation("hybrid_comp", n);
            Operation qsSel = prof.createOperation("quick_assig", n);
            Operation qsComp = prof.createOperation("quick_comp", n);

            hybrid(a, 0, n - 1, sel, comp);
            quickSort(b, 0, n - 1, n, qsSel, qsComp);

            free(a);
            free(b);
        }
    }
    prof.divideValues("hybrid_comp", 5);
    prof.divideValues("hybrid_assig", 5);

    prof.divideValues("quick_assig", 5);
    prof.divideValues("quick_comp", 5);

    prof.addSeries("Hybrid Sort", "hybrid_assig", "hybrid_comp");
    prof.addSeries("Quick Sort", "quick_comp", "quick_assig");

    prof.createGroup("Comparison", "hybrid_comp", "quick_comp");
    prof.createGroup("Assignment", "hybrid_assig", "quick_assig");
    prof.createGroup("Operation", "Hybrid Sort", "Quick Sort");

    for (int n = starting_size; n <= final_size; n += 100) {
        prof.startTimer("Run Time HYBRID", n);
        for (int i = 1; i <= 100; i++) {
            int *a = (int *) malloc(sizeof(int) * 200000);
            FillRandomArray(a, n, 0, 10000, true, UNSORTED);
            hybrid(a, 0, n - 1);
            free(a);
        }
        prof.stopTimer("Run Time HYBRID", n);
    }

    for (int n = starting_size; n <= final_size; n += 100) {
        prof.startTimer("Run Time QUICK", n);
        for (int i = 1; i <= 100; i++) {
            int *a = (int *) malloc(sizeof(int) * 200000);
            FillRandomArray(a, n, 0, 10000, true, UNSORTED);
            quickSort(a, 0, n - 1, n);
            free(a);
        }
        prof.stopTimer("Run Time QUICK", n);
    }

    prof.createGroup("Runtime", "Run Time QUICK", "Run Time HYBRID");
    prof.showReport();
}

int main() {
    //test();
    perf();
    return 0;
}
