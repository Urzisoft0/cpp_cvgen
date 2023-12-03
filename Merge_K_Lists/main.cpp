#include <iostream>
#include <list>
#include "Profiler.h"

Profiler prof("Sorting Methods");

typedef struct Lists {
    std::list<int> l;
}Lists;

void heapify(std::pair<int, int> a[], int n, int node, Operation opComp, Operation opAssig) {
    int left = 2 * node + 1, right = left + 1;
    bool ok = false;

    while (!ok) {
        ok = true;
        int max_child = node;

        if (left < n && a[max_child].first > a[left].first) {
            max_child = left;
            opComp.count();
        }


        if (right < n && a[max_child].first > a[right].first) {
            max_child = right;
            opComp.count();
        }


        if (node != max_child) {
            std::swap(a[node], a[max_child]);

            opAssig.count(3);
            ok = false;

            node = max_child;
            left = 2 * node + 1, right = left + 2;
        }
    }
}

void bottom_up(std::pair<int, int> a[], int n, Operation opComp, Operation opAssig) {

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(a, n, i, opComp, opAssig);
}

void merge(Lists b[], int c[], int m, int k) {
    std::pair<int, int> a[m];

    for(int i = 0; i < m; i++) {
        a[i].first = b[i].l.front();
        a[i].second = i;

        b[i].l.remove(b[i].l.front());
    }


    Operation merge_comp = prof.createOperation("merge_comp", k);
    Operation merge_assig = prof.createOperation("merge_assig", k);

    merge_assig.count(2 * m);
    bottom_up(a, m, merge_comp, merge_assig);

    int n = m;
    int j = 0;

    while (n != 0) {
        int pair = a[0].second;
        c[j] = a[0].first;
        j++;

        std::swap(a[0], a[n - 1]);

        if (b[pair].l.size() == 0) {
            n--;
        }
        else {
            a[n - 1].first = b[pair].l.front();
            a[n - 1].second = pair;
            b[pair].l.remove(b[pair].l.front());

            merge_assig.count(2);
        }

        bottom_up(a, n, merge_comp, merge_assig);
    }
}

void FillRandomList(Lists a[], int m, int n) {
    int v[n];

    int j = 0;
    while (j < m) {
        FillRandomArray(v, n, 100, 10000, false, ASCENDING);
        int i = 0;

        while (i < n) {
            a[j].l.push_back(v[i]);
            i++;
        }

        j++;
    }
}

void FillRandomList3(Lists a[], int m, int n) {
    int j = 0;

    while (j < m) {
        int v[n / m];
        FillRandomArray(v, n / m, 1, 10000, false, ASCENDING);
        a[j].l.insert(a[j].l.begin(), v, v + sizeof(v) / sizeof(int));

        j++;
    }

    if (n % m != 0) {
        for (int i = 0; i < n % m; i++)
            a[i].l.push_back(a[i].l.back() + 29);
    }
}

void perf_n() {
    int n;
    int starting_size = 100;
    int *c = (int *)malloc(sizeof(int) * INT_MAX);

    int final_size = 10000;
    int increment = 100;

    for (n = starting_size; n <= final_size; n += increment) {
        for (int i = 0; i < 10; i++) {
            Lists a[5];

            FillRandomList3(a, 5, n);
            merge(a, c, 5, n);
        }
    }

    prof.divideValues("merge_comp", 10);
    prof.divideValues("merge_assig", 10);
    prof.addSeries("merge_5", "merge_comp", "merge_assig");

    for (n = starting_size; n <= final_size; n += increment) {
        for (int i = 0; i < 10; i++) {
            Lists a[10];

            FillRandomList3(a, 10, n);
            merge(a, c, 10, n);
        }
    }

    prof.divideValues("merge_comp", 10);
    prof.divideValues("merge_assig", 10);
    prof.addSeries("merge_10", "merge_comp", "merge_assig");


    for (n = starting_size; n <= final_size; n += increment) {
        for (int i = 0; i < 10; i++) {
            Lists a[100];

            FillRandomList3(a, 100, n);
            merge(a, c, 100, n);
        }
    }

    prof.divideValues("merge_comp", 10);
    prof.divideValues("merge_assig", 10);
    prof.addSeries("merge_100", "merge_comp", "merge_assig");

    prof.createGroup("merge", "merge_5", "merge_10", "merge_100");
}



void perf_k() {
    int k, n = 10000;
    int starting_size = 10;

    int final_size = 500;
    int increment = 10;
    int *c = (int *)malloc(sizeof(int) * INT_MAX);

    for (int i = 1; i <= 10 ; i++) {
        for (k = starting_size; k <= final_size; k += increment) {
            Lists a[k];

            FillRandomList3(a, k, n);
            merge(a, c, k, k);
        }
    }

    prof.divideValues("merge_comp", 10);
    prof.divideValues("merge_assig", 10);

    prof.addSeries("op_merge", "merge_comp", "merge_assig");


}

void demo() {
    int m = 5;

    Lists b[m];
    int c[100000];
    b[0].l = {9, 10, 31, 34};
    b[1].l = {1, 5, 67, 89, 100};
    b[2].l = {9, 30, 54};
    b[3].l = {3};
    b[4].l = {4, 13, 22, 100, 230, 1000, 10001};

    int sum = 0;
    merge(b, c, m, 20);

    for(int i = 0; i < 20; i++)
        std::cout << c[i] << " ";
}

void demo2() {
    int m = 4;
    int n = 10;

    Lists b[m];
    int c[10000];
    FillRandomList3(b, m, n);

    for(int i = 0; i < m; i++) {
        for (int j : b[i].l)
            std::cout << j << " ";
        std::cout << "\n";
    }


    merge(b, c, m, n);

    for(int i = 0; i < n; i++)
        std::cout << c[i] << " ";
}

void demo3() {
    int m = 4;
    int n = 10;

    Lists b[m];
    int c[10000];

    b[0].l = {1034, 4099, 4128};
    b[1].l = {2610, 4215, 4244};
    b[2].l = {1833, 2977};
    b[3].l = {391, 673};
    merge(b, c, m, n);

    for(int i = 0; i < n; i++)
        std::cout << c[i] << " ";
}

void show_all() {
    perf_n();
    prof.reset();
//k
    perf_k();
    prof.reset();

}

int main() {
    demo2();

    show_all();
    return 0;
}
