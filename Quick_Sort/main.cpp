#include <iostream>
#include "Profiler.h"

Profiler prof("Hs vs Qs");
enum Cases {worst, best, average};

/*
 *  QuickSort is based on pivoting. It is extremely effective in average cases, in which the array is
 *  as random as possible. It is a divide et impera method, thus the complexity rounds up to O(nlog(n)).
 *  However, it performs poorly in ASCENDING or DESCENDING cases, for it
 *  chooses as the pivot the rightmost or the leftmost element, creating two unbalanced arrays. It gets to a O(n^2)
 *  complexity.
 *
 *  Quick Select is similar to QuickSort, though it returns the ith element of the unsorted array, without actually
 *  sorting it. It gets to a complexity if O(n)
 *
 *  A recursive Selection Sort takes much more to solve than an iterative one, because of its function calls which
 *  are stored in the stack. It increases the space complexity. Several recursive calls slow down the process for
 *  larger arrays.
 *
 */

int partition(int *a, int left, int right, Operation qsComp, Operation qsAssig) {
    int p = a[right];

    qsAssig.count();
    int i = left - 1;

    for (int j = left; j <= right; j++) {
        qsComp.count();

        if (a[j] < p) {
            i++;

            qsAssig.count(3);
            std::swap(a[j], a[i]);
        }
    }
    qsAssig.count(3);
    std::swap(a[i + 1], a[right]);

    return i + 1;
}

int quickSelect(int *a, int left, int right, int n, int element) {
    Operation qselectComp = prof.createOperation("quicksel_comp", n);
    Operation qselectAssig = prof.createOperation("quicksel_assig", n);
    int index;

    if (right > left) {
        int part = partition(a, left, right, qselectComp, qselectAssig);

        if (part == element)
            return a[part];
        else if (element < part)
            return quickSelect(a, left, part - 1, n, element);
        else if (element > part)
            return quickSelect(a, part + 1, right, n, element);
    }
}

void quickSort(int *a, int left, int right, int n) {
    Operation qsComp = prof.createOperation("quick_comp", n);
    Operation qsAssig = prof.createOperation("quick_assig", n);

    if (right > left) {
        int part = partition(a, left, right, qsComp, qsAssig);

        quickSort(a, left, part - 1, n);
        quickSort(a, part + 1, right, n);
    }
}

void heapify(int a[], int n, int node, Operation heapify_comp, Operation heapify_ins) {
    int left = 2 * node, right = left + 1;
    bool ok = false;

    while (!ok) {
        ok = true;

        int max_child = node;

        heapify_comp.count(); //
        if (left < n && a[max_child] < a[left])
            max_child = left;

        heapify_comp.count(); //
        if (right < n && a[max_child] < a[right])
            max_child = right;

        if (node != max_child) {
            std::swap(a[node], a[max_child]);
            heapify_ins.count(3); //

            ok = false;
            node = max_child;
            left = 2 * node, right = left + 1;
        }
    }
}

void bottom_up(int a[], int n, Operation heapify_comp, Operation heapify_ins) {

    for (int i = n / 2; i >= 1; i--)
        heapify(a, n, i, heapify_comp, heapify_ins);
}

void heap_sort(int a[], int n) {
    Operation heapsort_comp = prof.createOperation("heapsort_comp", n);
    Operation heapsort_ins = prof.createOperation("heapsort_ins", n);

    bottom_up(a, n, heapsort_comp, heapsort_ins);
    int i = n - 1;

    while (i > 1) {
        std::swap(a[i], a[1]);

        heapify(a, i , 1, heapsort_comp, heapsort_ins);
        i--;
    }
}

void selectionSort(int* a, int n) {
    int i = 0, min;

    Operation s_comp = prof.createOperation("sel_comp", n);
    Operation s_assig = prof.createOperation("sel_assig", n);

    for (int i = 0; i < n - 1; i++) {
        min = i;

        for (int j = i + 1; j < n; j++) {
            s_comp.count(1);

            if (a[min] > a[j])
                min = j;
        }

        if (min != i) {
            std::swap(a[i], a[min]);
            s_assig.count(3);
        }
    }
}

int min(int *a, int i, int j, Operation opComp, Operation opAssig) {
    if (i == j) return i;

    int inter = min(a, i + 1, j, opComp, opAssig);

    if (a[inter] > a[i]) {
        opComp.count();
        return i;
    }

    else {
        opComp.count();
        return inter;
    }

}

void selectionSort_rec(int *a, int n, int i, Operation selrec_comp, Operation selrec_assig) {

    if (i == n)
        return;

    int inter = min(a, i, n - 1, selrec_comp, selrec_assig);

    if (inter != i) {
        std::swap(a[inter], a[i]);
        selrec_assig.count(3);
    }

    selectionSort_rec(a, n, i + 1, selrec_comp, selrec_assig);
}

void qsVhs(Cases my_type) {
    int starting_size = 100;
    int maximum_size = 10000;

    int increment = 100;
    int a[10001], dummy[10001];

    int x = 5;

    if (my_type == average) {
        for (int i = 0; i < x; i++) {
            for (int n = starting_size; n <= maximum_size; n += increment) {
                FillRandomArray(a, n, 10, 10000, false, UNSORTED);
                CopyArray(dummy, a, n);

                heap_sort(a, n);
                quickSort(dummy, 0, n - 1, n);
            }
        }
        prof.divideValues("quick_comp", x);
        prof.divideValues("quick_assig", x);

        prof.divideValues("heapsort_comp", x);
        prof.divideValues("heapsort_ins", x);
    }

    prof.addSeries("op_quick", "quick_comp", "quick_assig");
    prof.addSeries("op_heapsort", "heapsort_comp", "heapsort_ins");

    prof.createGroup("op_all", "op_heapsort", "op_quick");
    prof.createGroup("comp_all", "heapsort_comp", "quick_comp");
    prof.createGroup("assig_all", "heapsort_ins", "quick_assig");
}

void qsDes() {
    int starting_size = 100;
    int maximum_size = 10000;

    int increment = 100;
    int a[10001], dummy[10001];

    for (int n = starting_size; n <= maximum_size; n += increment) {
        FillRandomArray(dummy, n, 10, 10000, false, DESCENDING);

        quickSort(dummy, 0, n - 1, n);

    }

    prof.addSeries("op_quick", "quick_comp", "quick_assig");

    //prof.showReport();

}

void qsAsc() {
    int starting_size = 100;
    int maximum_size = 10000;

    int increment = 100;
    int a[10001], dummy[10001];

    for (int n = starting_size; n <= maximum_size; n += increment) {
        FillRandomArray(dummy, n, 10, 10000, false, ASCENDING);

        quickSort(dummy, 0, n - 1, n);

    }

    prof.addSeries("op_quick", "quick_comp", "quick_assig");

    //prof.showReport();

}

void selOp(Cases my_type) {

    int starting_size = 100;
    int maximum_size = 10000;

    int increment = 100;
    int a[10001], dummy[10001];

    int x = 5;

    if (my_type == average) {

        for (int i = 0; i < x; i++) {
            for (int n = starting_size; n <= maximum_size; n += increment) {

                Operation selrec_comp = prof.createOperation("selrec_comp", n);
                Operation selrec_assig = prof.createOperation("selrec_assig", n);

                FillRandomArray(a, n, 10, 10000, false, UNSORTED);
                CopyArray(dummy, a, n);

                selectionSort(a, n);
                selectionSort_rec(dummy, n, 0, selrec_comp, selrec_assig);
            }
        }

        prof.divideValues("sel_comp", x);
        prof.divideValues("sel_assig", x);

        prof.divideValues("selrec_comp", x);
        prof.divideValues("selrec_ins", x);

    }

    prof.addSeries("op_sel", "sel_comp", "sel_assig");
    prof.addSeries("op_selrec", "selrec_comp", "selrec_assig");

    prof.createGroup("op_all", "op_sel", "op_selrec");
    prof.createGroup("comp_all", "sel_comp", "selrec_comp");
    prof.createGroup("assig_all", "sel_assig", "selrec_assig");

}

void selRun() {
    int starting_size = 100;
    int maximum_size = 10000;

    int increment = 100;
    int a[10001], dummy[10001];

    int x = 1000;

    for (int n = starting_size; n <= maximum_size; n += increment) {
        prof.startTimer("Selection Sort", n);
        for (int i = 0; i < x; i++) {
            FillRandomArray(a, n, 10, 10000, false, UNSORTED);
            selectionSort(a, n);
        }
        prof.stopTimer("Selection Sort", n);
    }

    for (int n = starting_size; n <= maximum_size; n += increment) {
        prof.startTimer("Recursive Selection Sort", n);
        for (int i = 0; i < x; i++) {
            Operation selrec_comp = prof.createOperation("selrec_comp", n);
            Operation selrec_assig = prof.createOperation("selrec_assig", n);

            FillRandomArray(a, n, 10, 10000, false, UNSORTED);
            selectionSort_rec(a, n, 0, selrec_comp, selrec_assig);
        }
        prof.stopTimer("Recursive Selection Sort", n);
    }

    prof.showReport();

}

void printArray(int *a, int n) {
    for (int i = 0; i < n; i++)
        std::cout << a[i] << " ";
    std::cout << std::endl;
}
void qsel() {
    int starting_size = 100;
    int maximum_size = 10000;

    int increment = 100;
    int a[10001], dummy[10001];

    int x = 5;
    for (int i = 0; i < x; i++) {
        for (int n = starting_size; n <= maximum_size; n += increment) {
            FillRandomArray(dummy, n, 10, 10000, false, UNSORTED);

            quickSelect(dummy, 0, n - 1, n, n / 2);

        }
    }
    prof.divideValues("quicksel_comp", x);
    prof.divideValues("quicksel_assig", x);

    prof.addSeries("op_quicksel", "quicksel_comp", "quicksel_assig");

    prof.showReport();
}

void show_all() {
    qsVhs(average);
    prof.reset();

    qsDes();
    prof.reset();

    qsAsc();
    prof.reset();

    selOp(average);
    prof.reset();

    selRun();
    prof.reset();
}

void demo() {
    int a[] = {57,   40,   90,   56,   49,   54,   68,   80,   52,   88,   55,   15,   10,   21,   7, 7, 20,   78,   60,   33,   50,   89,   73,  100,   98};
    int n = sizeof(a) / sizeof(a[0]);

    int b[n];
    CopyArray(b, a, n);

    std::cout << "Unsorted Array: ";
    printArray(a, n);
    quickSort(a, 0, n - 1, n);

    std::cout << "Quick Sort: ";
    printArray(a, n);

    std::cout << "Tenth Element in Array: " << quickSelect(b, 0, n - 1, n, 9);
    std::cout << std::endl << std::endl;
}

void demo2() {
    int a[] = {57,   40,   90,   56,   49,   54,   68,   80,   52,   88,   55,   15,   10,   21,   7, 7, 20,   78,   60,   33,   50,   89,   73,  100,   98};
    int n = sizeof(a) / sizeof(a[0]);

    Operation srec_comp = prof.createOperation("selrec_comp", n);
    Operation srec_assig = prof.createOperation("selrec_assig", n);

    int b[n];
    CopyArray(b, a, n);

    std::cout << "Unsorted Array: ";
    printArray(a, n);
    selectionSort(a, n);
    selectionSort_rec(b, n, 0, srec_comp, srec_assig);

    std::cout << "Iterative version: ";
    printArray(a, n);

    std::cout << "Recursive version: ";
    printArray(b, n);
}

int main() {
    demo();
    demo2();
    //show_all();

    return 0;
}