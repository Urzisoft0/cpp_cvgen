#include <iostream>
#include <iomanip>
#include <vector>
#include "Profiler.h"
using namespace std;


/*  Hash Tables are, by definition, a sort of dictionary. Elements are added by encryption (hash function).
 *  Based on the hash function, the search, removal and insert times are expected to be O(1), with a maximum of O(n).
 *
 */

template<int N>
class HashTable {
private:
    vector<int *> table;
public:
    HashTable() { table.resize(N); }

    int hashFunction(int x) {
//        x = ((x >> 16) ^ x) * 0x45d9f3b;
//        x = ((x >> 16) ^ x) * 0x45d9f3b;
//        x = (x >> 16) ^ x;

        return x;
    }

    // Tests if the key exists by going through every element
    bool keyExists(int key) {
        for (int i = 0; i < N; i++) {
            if (table[i])
                if (*table[i] == key) return true;
        }
        return false;
    }

    bool isFull() {
        for (int i = 0; i < N; i++)
            if (table[i] == nullptr) return false;
        return true;
    }

    //Insert function. If it goes back and forth, it returns -1
    bool insert(int key) {
        if (isFull() || keyExists(key)) {
            cout << "NO\n";
            return false;
        }
        int x = 1;

        int keyHash = hashFunction(key) % N;
        int index = keyHash;

        while (table[index] != nullptr) {
            index = hashFunction(keyHash + quadProbing(x)) % N;
            x++;

            if (x == 2*N)
                return false;
        }

        table[index] = static_cast<int *>(malloc(sizeof(int *)));
        *table[index] = key;

        return true;
    }

    bool isEmpty() {
        for (int i = 0; i < N; i++)
            if (table[i] != nullptr)
                return false;

        return true;
    }

    static int quadProbing(int iterator) {
        return 2*iterator * iterator + 6;
    }

    void printTable() {

        for(int i = 0; i < table.size(); i++) {
            if (table[i]) cout << i << ": " << *table[i] << "\n";
            else cout << i << ": NULL\n";
        }
        cout << "\n\n";
    }

    //Search function. If it encounters a nullptr, it means that the key is non-existent.
    int searchKey(int key, int &iterator) {
        int x = 1;
        iterator = x;
        int keyHash = hashFunction(key) % N;
        int index = keyHash;

        while(true) {
            if(table[index] == nullptr)
                return -1;
            else
            if (table[index] != nullptr && *table[index] == key)
                return index;

            index = hashFunction(keyHash + quadProbing(iterator)) % N;
            x++;
            iterator = x;

            if (x == N)
                return -1;
        }
    }

    //Remove function using Search.
    bool removeKey(int key) {
        if (isEmpty()) {
            //cout << "[ERROR] Table is empty!\n";
            return false;
        }
        int s;
        int index = searchKey(key, s);

        if (index == -1) {
            //cout << "[WARNING] Trying to remove a non existent key! (" << key << ")"<< endl;
            return false;
        }

        else {
            free(table[index]);
            table[index] = nullptr;
            //cout << "[UPDATE] Removed value " << key << endl;
            return true;
        }

    }

    //Remove function by index.
    bool removeIndex(int index) {
        if (isEmpty()) {
            //cout << "[ERROR] Table is empty!\n";
            return false;
        }

        if (table[index] == nullptr) {
            //cout << "[ERROR] Cannot remove nullptr cell\n";
            return false;
        }
        free(table[index]);

        table[index] = nullptr;
        return true;
    }

    int size() { return N; }
    int getValue(int index) { return table[index] == nullptr ? -1 : *table[index]; };
};


/*  It gets the percentage of the size. Then it assigns randomly values to the table.
 *  To search uniformly, it sections the array by 1500. The elements of the remainder array are taken one
 *  by one and added to one of the sections.
 *  Then it will randomly get an element of each section, incrementing by (X%size) / 1500.
 *  Additionally, the function searches 1500 non-existent.
*/
void found_notFound(double percent) {
    HashTable<10007>b;

    int n = percent * b.size();

    int *v = (int *)malloc(sizeof(int) * (n + 1500));
    FillRandomArray(v, n + 1500, 1, 10000000, true, UNSORTED);

    for (int i = 0; i < n; i++)
        b.insert(v[i]);

    float s2 = 0;
    float final_sum_avg = 0;
    for (int j = 1; j <= 5; j++) {
        int d = n - n%1500;

        for(int i = 0; i < n - n%1500; i += n / 1500) {
            int sum;
            if (d < n) {
                int c[n / 1500 + 1];
                FillRandomArray(c, n / 1500, i, i + n / 1500 - 1, true, UNSORTED);
                c[n / 1500] = d;
                int a[1];

                FillRandomArray(a, 1, 0, n / 1500, true, UNSORTED);
                b.searchKey(v[i + a[0]], sum);
                final_sum_avg += sum;
                d++;
            }
            else {
                int a[1];
                FillRandomArray(a, 1, 0, n / 1500 - 1, true, UNSORTED);
                //cout << "i: " << i << " " <<i + a[0] << endl;
                b.searchKey(v[i + a[0]], sum);
                final_sum_avg += sum;
            }
        }
        final_sum_avg /= 1500;
        s2 += final_sum_avg;
    }
    cout << setw(5) << percent * 100 << "%" << setw(26) << setprecision(4) << s2 / 5 << setw(20);

    s2 = 0;
    for (int j = 1; j <= 5; j++) {
        int d = n - n%1500;
        for (int i = 0; i < n - n%1500; i += n / 1500) {
            int sum;

            if (d < n) {
                int c[n / 1500 + 1];
                FillRandomArray(c, n / 1500, i, i + n / 1500 - 1, true, UNSORTED);
                c[n / 1500] = d;
                int a[1];
                FillRandomArray(a, 1, 0, n / 1500, true, UNSORTED);
                b.searchKey(v[i + a[0]], sum);
                s2 += sum;
                d++;
            }
            else {
                int a[1];
                FillRandomArray(a, 1, 0, n / 1500 - 1, true, UNSORTED);
                //cout << "i: " << i << " " <<i + a[0] << endl;
                b.searchKey(v[i + a[0]], sum);
                s2 += sum;
            }
//            int a[1];
//            FillRandomArray(a, 1, 1, n / 1500, true, UNSORTED);
//
//            a[0] %= n / 1500;
//            //cout << "i: " << i << " " <<i + a[0] << endl;
//            b.searchKey(v[i + a[0]], sum);
//            s2 += sum;
        }
    }
    cout << (int)s2 / 5 << setw(22);

    final_sum_avg = 0;
    for (int i = n; i < n + 1500; i ++) {
        int sum = 1;
        b.searchKey(v[i], sum);
        final_sum_avg += sum;
    }

    final_sum_avg /= 1500;
    cout << setprecision(4) << (int) final_sum_avg << setw(24);

    s2 = 0;
    for (int i = n; i < n + 1500; i ++) {
        int sum;
        b.searchKey(v[i], sum);
        s2 += sum;
    }
    free(v);
    cout << (int)s2 << endl;
}

//  Function to get the index of an element, since vector<> does not have such a functionality.
int getIndex(vector<int> v, int K) {
    auto it = find(v.begin(), v.end(), K);

    if (it != v.end()) {
        int index = it - v.begin();
        return index;
    }
    return -1;

}

/*  The same as found_notFound, although it removes elements randomly until it reaches 80% of size,
 *  being initially filled by a factor of 99%.
 */

void removeSearch() {
    HashTable<10007> b;
    int n = 0.99 * b.size();

    int *v = (int *)malloc(sizeof(int) * (n + 1500));
    vector<int> v1;
    v1.resize(n + 1500);
    FillRandomArray(v, n + 1500, 1, 10000000, true, UNSORTED);

    for (int i = 0; i < n + 1050; i++)
        v1[i] = v[i];

    //cout << v1.size();

    for (int i = 0; i < n; i++)
        b.insert(v[i]);
    //b.printTable();

    int stop = 0.8 * b.size();
    while(n != stop) {
        //cout << "!!!!!" << n << " " << stop << endl;
        int a[1];
        FillRandomArray(a, 1, 0, b.size() - 1, true, UNSORTED);
        int k = b.getValue(a[0]);

        if (b.removeIndex(a[0])) {
            //printf("REMOVED table[%d]\n", a[0]);
            int q = getIndex(v1, k);
            v1.erase(v1.begin() + q);
            n--;
        }
    }

    float s2 = 0;
    float final_sum_avg = 0;
    for (int j = 1; j <= 5; j++) {
        int d = n - n%1500;
        for (int i = 0; i < n - n%1500; i += n / 1500) {
            int sum;
            if (d < n) {
                int c[n / 1500 + 1];
                FillRandomArray(c, n / 1500, i, i + n / 1500 - 1, true, UNSORTED);
                c[n / 1500] = d;
                int a[1];

                FillRandomArray(a, 1, 0, n / 1500, true, UNSORTED);
                b.searchKey(v1[i + a[0]], sum);
                final_sum_avg += sum;
                d++;
            }
            else {
                int a[1];
                FillRandomArray(a, 1, 0, n / 1500 - 1, true, UNSORTED);
                //cout << "i: " << i << " " <<i + a[0] << endl;
                b.searchKey(v1[i + a[0]], sum);
                final_sum_avg += sum;
            }
        }
        final_sum_avg /= 1500;
        s2 += final_sum_avg;
    }
    cout << " " << "99%-80%" << setw(24) << setprecision(4) << s2 / 5  << setw(20);

    final_sum_avg = 0;
    for (int j = 1; j <= 5; j++) {
        int d = n - n%1500;

        for (int i = 0; i < n - n%1500; i += n / 1500) {
            int sum;
            if (d < n) {
                int c[n / 1500 + 1];
                FillRandomArray(c, n / 1500, i, i + n / 1500 - 1, true, UNSORTED);
                c[n / 1500] = d;
                int a[1];

                FillRandomArray(a, 1, 0, n / 1500, true, UNSORTED);
                b.searchKey(v1[i + a[0]], sum);
                final_sum_avg += sum;
                d++;
            }
            else {
                int a[1];
                FillRandomArray(a, 1, 0, n / 1500 - 1, true, UNSORTED);
                //cout << "i: " << i << " " <<i + a[0] << endl;
                b.searchKey(v1[i + a[0]], sum);
                final_sum_avg += sum;
            }
        }
    }
    cout << (int)final_sum_avg / 5 << setw(22);

    final_sum_avg = 0;
    for (int i = n; i < n + 1500; i ++) {
        int sum = 0;
        b.searchKey(v1[i], sum);
        final_sum_avg += sum;
    }
    final_sum_avg /= 1500;
    cout << setprecision(4) << (int) final_sum_avg  << setw(24);

    final_sum_avg = 0;
    for (int i = n; i < n + 1500; i ++) {
        int sum;
        b.searchKey(v1[i], sum);
        final_sum_avg += sum;
    }
    cout << (int)final_sum_avg << endl;
}

void demo() {
    HashTable<20> a;
    int n = 15;
    int *b = (int *)malloc(sizeof(int) * n);

    FillRandomArray(b, n, 0, 100000, true, UNSORTED);
    for (int i = 0; i < n; i++)
        a.insert(b[i]);

    int s;
    a.printTable();
    cout << "Index of " << b[4] << " is " << a.searchKey(b[4], s) << endl;
    a.removeKey(b[4]);

    cout <<"Removed " << b[4] <<
         endl << "Index of " << b[4] << " is " << a.searchKey(b[4], s) << endl;
    a.printTable();

    cout<<"Added " << b[4] % 10 << " and " << b[4] << "\n\n";
    a.insert(b[4] % 10);
    a.insert(b[4]);

    a.printTable();
    cout << "\nIndex of " << b[4] << " is " << a.searchKey(b[4], s) << endl;
}

void demo2() {
    HashTable<10> a;
    int s;
    a.insert(14);
    a.insert(4);
    a.printTable();
    a.removeKey(14);
    a.printTable();
    cout << "Index of 4: " << a.searchKey(4, s) << endl << "Collisions: " << s << endl;
    a.insert(14);
    a.printTable();
}

int main() {
    cout << "Filling Factor" << setw(20) << "Avg Found" << setw(20) << "Max Found" << setw(25) << "Avg Not_Found"
         << setw(25) << "Max Not_Found\n";
    found_notFound(0.8);
    found_notFound(0.85);
    found_notFound(0.9);
    found_notFound(0.95);
    found_notFound(0.99);
    removeSearch();

    //demo();
    //demo2();
}
