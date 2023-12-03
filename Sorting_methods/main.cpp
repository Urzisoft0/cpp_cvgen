
/*
 *  Iordache Paul-Cristian
 *  Group 30424
 *  Sorting Methods (Bubble Sort, Selection Sort, Insertion Sort + Binary Insertion Sort)
 */

#include <iostream>
#include "Profiler.h"

Profiler prof("Sorting Methods");
enum Cases { worst, average, best };

/*
 * Insertion Sort separates the array into 2 parts: source and destination.
 * The source is the sorted, while as the destination is unsorted.
 * Take the first member of the destination and search swap it with each bigger number from the source.
 * Stop when it encounters a smaller number.
 * This algorithm uses a Sentinel approach. We set a[0] to be the smallest 32-bit integer.
 * We avoid using two comparisons inside the while (j >= 0 && a[j] > itm).
 */

void insertionSort(int *a, int n) {
    a[0] = INT_MIN;

    Operation ins_comp = prof.createOperation("ins_comp", n);
    Operation ins_assig = prof.createOperation("ins_assig", n);

    for (int i = 2; i < n; i++) {
        bool ok = false;
        int j = i - 1, itm = a[i];

        while (a[j] > itm) {
            ins_comp.count(1);
            ins_assig.count();

            a[j + 1] = a[j];
            j--;
            ok = true;
        }
        ins_comp.count(1);

        if (ok == true) {
            ins_assig.count(1);
            a[j + 1] = itm;
        }
    }
}

/*
 *  Binary Insertion Sort is similar to its predecessor, using only a binary search to find where to stop in the source array.
 *  Searching bin is O(log(n)) instead of O(n).
 *  For this reason, best case performs more poorly than the prior sort, for it searches for bin logarithmically instead of linearly.
 */

int binarySearch(int *a, int left, int right, int x, Operation &comp) {
    if (left >= right) {
        comp.count(1);
        if (a[left] < x)
            return left + 1;
        else
            return left;
    }

    int mid = (left + right) / 2;

    comp.count(1);
    if (a[mid] == x)
        return mid + 1;

    comp.count(1);
    if (a[mid] < x)
        return binarySearch(a, mid + 1, right, x, comp);
    else
        return binarySearch(a, left, mid - 1, x, comp);
}

void binaryInsertionSort(int *a, int n) {
    Operation bins_comp = prof.createOperation("binsert_comp", n);
    Operation bins_assig = prof.createOperation("binsert_assig", n);

    for (int i = 1; i < n; i++) {
        bool ok = false;
        int j = i - 1, itm = a[i];

        int bin = binarySearch(a, 0, j, itm, bins_comp);

        bins_comp.count();
            while (j >= bin && a[j + 1] != a[j]) {
                bins_assig.count(1);
                a[j + 1] = a[j];
                j--;
                ok = true;
            }

        if (ok == true) {
            bins_assig.count(1);
            a[j + 1] = itm;
        }
    }
}

/*  Bubble sort swaps adjacent elements in the array. It is mostly useful in cases where the array is almost sorted.
 *  It stops when there hasn't been any permutation.
 *  It performs well in the best case, being O(n).
 *  O(n²) in the rest of cases.
 *  It is stable because it doesn't swap elements if they are equal.
 */


void bubbleSort(int *a, int n) {
    Operation b_comp = prof.createOperation("bu_comp", n);
    Operation b_assig = prof.createOperation("bu_assig", n);

    bool ok;
    do {
        ok = true;
        n--;

        for (int i = 0; i < n; i++) {
            b_comp.count(1);
            if (a[i] > a[i + 1]) {
                std::swap(a[i], a[i + 1]);
                b_assig.count(3);
                ok = false;
            }
        }
    }while(ok == false);
}

/*  Selection Sort is the most inefficient algorithm when it comes to the best case.
 *  Its complexity is always O(n²), because it is not adaptive in any way.
 *  It's not stable for this very reason.
 */

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

void perf(Cases my_type) {
    int starting_size = 100;
    int maximum_size = 10000;
    int increment = 100;
    int a[10001], b[10001], bubble[10001], bins[10001], ins[10001], sel[10001];

    int x = 5;
    if (my_type == average) {
        for (int i = 0; i < x; i++) {

            for (int n = starting_size; n <= maximum_size; n += increment) {

                FillRandomArray(a, n, 100, 10000, false, UNSORTED);
                CopyArray(b, a, n);

                selectionSort(b, n);
                CopyArray(b, a, n);

                bubbleSort(b, n);
                CopyArray(b, a, n);

                insertionSort(b, n);
                CopyArray(b, a, n);

                binaryInsertionSort(b, n);
            }
        }
        prof.divideValues("sel_comp", x);
        prof.divideValues("sel_assig", x);

        prof.divideValues("bu_comp", x);
        prof.divideValues("bu_sel", x);

        prof.divideValues("ins_comp", x);
        prof.divideValues("ins_sel", x);

        prof.divideValues("binsert_comp", x);
        prof.divideValues("binsert_sel", x);
    }
    else if (my_type == worst) {
        for (int n = starting_size; n <= maximum_size; n += increment) {
            FillRandomArray(bubble, n, 100, 10000, false, DESCENDING);
            FillRandomArray(sel, n, 100, 10000, false, DESCENDING);

            FillRandomArray(ins, n, 100, 10000, false, DESCENDING);
            FillRandomArray(bins, n, 100, 10000, false, DESCENDING);

            selectionSort(sel, n);
            bubbleSort(bubble, n);

            insertionSort(ins, n);
            binaryInsertionSort(bins, n);
        }
    }
    else if (my_type == best) {
        for (int n = starting_size; n <= maximum_size; n += increment) {
            FillRandomArray(bubble, n, 100, 10000, false, ASCENDING);
            FillRandomArray(sel, n, 100, 10000, false, ASCENDING);

            FillRandomArray(ins, n, 100, 10000, false, ASCENDING);
            FillRandomArray(bins, n, 100, 10000, false, ASCENDING);

            selectionSort(sel, n);
            bubbleSort(bubble, n);

            insertionSort(ins, n);
            binaryInsertionSort(bins, n);
        }
    }
    prof.addSeries("op_bubble", "bu_comp", "bu_assig");
    prof.addSeries("op_ins", "ins_comp", "ins_assig");
    prof.addSeries("op_sel", "sel_comp", "sel_assig");
    prof.addSeries("op_binsert", "binsert_comp", "binsert_assig");

    prof.createGroup("op_all", "op_bubble", "op_sel", "op_ins", "op_binsert");
    prof.createGroup("comp_all", "bu_comp", "sel_comp", "ins_comp", "binsert_comp");
    prof.createGroup("assig", "bu_assig", "sel_assig", "ins_assig", "binsert_assig");
}

void show_all() {

    perf(worst);
    prof.reset();
    perf(best);
    prof.reset();
    perf(average);
    prof.reset();

}

void test() {
    int a[11] = {1, 5, 99 , 1, 8, 29, 3, 90, 4, 1, 9};
    int b[11] = {1, 5, 99 , 1, 8, 29, 3, 90, 4, 1, 9};
    int c[11] = {1, 5, 99 , 1, 8, 29, 3, 90, 4, 1, 9};
    int d[11] = {1, 5, 99 , 1, 8, 29, 3, 90, 4, 1, 9};
    bubbleSort(a, 11);
    for (int i = 1; i < 11; i++)
        std::cout << a[i] << " ";
    std::cout << "Bubble Sort: " << std::endl;

    insertionSort(b, 11);
    for (int i = 1; i < 11; i++)
        std::cout << b[i] << " ";
    std::cout << "Insertion Sort" << std::endl;

    binaryInsertionSort(c, 11);
    for (int i = 1; i < 11; i++)
        std::cout << c[i] << " ";
    std::cout << "Binary Insertion Sort" << std::endl;

    selectionSort(d, 11);
    for (int i = 0; i < 11; i++)
        std::cout << d[i] << " ";
    std::cout << "Selection Sort" << std::endl;

}

int main() {
    //show_all();
    test();

    return 0;
}

