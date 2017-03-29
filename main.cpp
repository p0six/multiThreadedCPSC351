#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <chrono>
#include <iostream>
#include <vector>
#include <thread>

using namespace std;

#define PRIME 4
#define NOPRIME 2
#define SIZE 21

//put your info here or you won't get points
#define STUDENT_NAME "Michael L. Romero"
#define STUDENT_CWID "890228026"

//you may not change code in this section:
void print_name(void) {
    printf("\n%s\n", STUDENT_NAME);
    printf("%s\n", STUDENT_CWID);
}

int is_prime(long long num)
{
    if (num <= 1) return 0;
    if (num % 2 == 0 && num > 2) return 0;
    for(long long i = 3; i < num / 2; i+= 2)
    {
        if (num % i == 0)
            return 0;
    }
    return 1;
}
//end of section
//you may add you code below this line:

void * is_primer (void *s) {
    long long *t = (long long*) s; // i've tried to make this simpler...
    long long num = *t; // but it's kind of ridiculous, all this referencing and dereferencing.  java > c
    bool *ret = new bool;
    *ret = (bool) is_prime(num);
    pthread_exit(ret);
}

int main(int argc, char ** argv)
{
    unsigned int n = std::thread::hardware_concurrency(); // on my rMBP, 8

    //don't change this name print section or you won't get points:
    if (argc == 2 && strncmp(argv[1], "-n", 2) == 0) {
        print_name();
        return 0;
    }
    //end of name print section

    auto start = chrono::high_resolution_clock::now();
    vector<long long> v;

    char buffer[SIZE];
    int i = 0;
    int c = getchar();
    while (c != EOF) {
        if (isspace(c)) {
            buffer[i] = '\0';
            i = 0;
        } else if (isdigit(c)) {
            buffer[i] = c;
            i++;
        }

        if (i == SIZE - 1) {
            buffer[i] = '\0';
            i = 0;
        }

        if (i == 0 && strnlen(buffer, SIZE) > 0) {
            long long num = atoll(buffer);
            v.push_back(num);
        }
        c = getchar();
    }

    pthread_t tid [n];
    long long y[n];
    while (v.size() != 0) {
        int m = n;
        for (int i = 0; i < n; i++) {
            if (v.size() != 0) { // possible during final while iteration
                y[i] = v.front();
                v.erase(v.begin());
                pthread_create(&tid[i], NULL, is_primer, (void *) &y[i]);
            } else {
                m = i; // used when we join threads
                break;
            }
        }
        for (int i = 0; i < m; i++) {
            bool *resultPointer;
            pthread_join(tid[i], (void **) &resultPointer);
            bool result = *resultPointer; // get our actual value back out...
            if (result) {
                printf("prime found: %d\n", y[i]);
                printf("return value: %d\n", PRIME);
                auto end = chrono::high_resolution_clock::now();
                std::chrono::duration<double> diff = end-start;
                cout << "duration: " << diff.count() << endl;
                return PRIME;
            }
        }
    }

    printf("return value: %d\n", NOPRIME);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end-start;
    cout << "duration: " << diff.count() << endl;
    return NOPRIME;
}
