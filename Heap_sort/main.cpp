#include <iostream>
#include "Profiler.h"

//  Iordache Paul-Cristian, group 30424

Profiler prof("Sorting Methods");
enum Cases { worst, average};

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

void swim(int a[], int node, Operation swim_comp, Operation swim_ins) {
    bool ok = false;

    while (node > 1 && !ok) {
        ok = true;
        int parent = node / 2;

        swim_comp.count();
        if (a[node] > a[parent]) {
            std::swap(a[node], a[parent]);
            swim_ins.count(3);

            node = parent;
            ok = false;
        }
    }
}

void bottom_up(int a[], int n) {
    Operation heapify_comp = prof.createOperation("heapify_comp", n);
    Operation heapify_ins = prof.createOperation("heapify_ins", n);

    for (int i = n / 2; i >= 1; i--)
        heapify(a, n, i, heapify_comp, heapify_ins);
}

void top_down(int a[], int n) {
    Operation swim_comp = prof.createOperation("swim_comp", n);
    Operation swim_ins = prof.createOperation("swim_ins", n);

    for (int i = 2; i < n; i++)
        swim(a, i, swim_comp, swim_ins);
}

void heap_sort(int a[], int n) {
    Operation heapsort_comp = prof.createOperation("heapsort_comp", n);
    Operation heapsort_ins = prof.createOperation("heapsort_ins", n);

    bottom_up(a, n);
    int i = n - 1;

    while (i > 1) {
        std::swap(a[i], a[1]);

        heapify(a, i , 1, heapsort_comp, heapsort_ins);
        i--;
    }
}

void demo() {
    std::cout << "Heap Sort: ";

    int a[] = {0, 40, 60, 1, 200, 9, 83, 17, 81, 7, 43, 54, 67, 11, 23, 60};
    heap_sort(a, 16);

    for (int i = 1; i < 16; i++)
        std::cout << a[i] << " ";

    std::cout << std::endl;
}

//  Demonstration of Heap-Sort

void demo2() {
    int b[] = {0, 9, 3, 10, 5, 1, 4, 21, 2, 8, 11};
    int c[] = {0, 9, 3, 10, 5, 1, 4, 21, 2, 8, 11};

    bottom_up(b, 11);
    top_down(c, 11);

    std::cout << "Bottom Up: ";
    for (int i = 1; i < 11; i++)
        std::cout << b[i] << " ";

    std::cout << std::endl;

    std::cout << "Top Down: ";
    for (int i = 1; i < 11; i++)
        std::cout << c[i] << " ";

    std::cout << std::endl;
}

void perf(Cases my_type) {
    int starting_size = 100;
    int maximum_size = 50000;

    int increment = 100;
    int a[50001], dummy[50001];

    int x = 5;

    if (my_type == average) {
        for (int i = 0; i < x; i++) {
            for (int n = starting_size; n <= maximum_size; n += increment) {
                FillRandomArray(a, n, 10, 50000, false, UNSORTED);
                CopyArray(dummy, a, n);

                top_down(dummy, n);
                CopyArray(dummy, a, n);

                bottom_up(dummy, n);    
            }
        }
        prof.divideValues("heapify_comp", x);
        prof.divideValues("heapify_ins", x);

        prof.divideValues("swim_comp", x);
        prof.divideValues("swim_ins", x);
    }
    else if (my_type == worst) {
        for (int n = starting_size; n <= maximum_size; n += increment) {
            FillRandomArray(a, n, 10, 50000, false, ASCENDING);
            CopyArray(dummy, a, n);

            top_down(a, n);
            bottom_up(dummy, n);
        }
    }
    prof.addSeries("op_swim", "swim_comp", "swim_ins");
    prof.addSeries("op_heapify", "heapify_comp", "heapify_ins");

    prof.createGroup("op_all", "op_heapify", "op_swim");
    prof.createGroup("comp_all", "heapify_comp", "swim_comp");
    prof.createGroup("ins_all", "heapify_ins", "swim_ins");
}

void show_all() {
    perf(average);
    prof.reset();

    perf(worst);
    prof.reset();
}

int main() {
    demo();
    demo2();
    show_all();

    return 0;
}
