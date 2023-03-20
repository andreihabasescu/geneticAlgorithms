#include "bitstring.h"
#include "genetic.h"
#include <iostream>
#include <conio.h>
#include <ctime>
using namespace std;


float dejong_rastrigin_limit = 5.12;

float schwefel_limit = 500;

int main()
{
    bitstring::function = "schwefel";
    bitstring::dimensions = 5;
    genetic::iterations = 1000;
    genetic::pop_size = 100;

    float range=0;
    float min = 0;

    if (bitstring::function == "dejong") {
        bitstring::lower_limit = -dejong_rastrigin_limit;
        bitstring::upper_limit = dejong_rastrigin_limit;
    }
    else if (bitstring::function == "schwefel") {
        bitstring::lower_limit = -schwefel_limit;
        bitstring::upper_limit = schwefel_limit;
        min = -419*bitstring::dimensions;
    }
    else if (bitstring::function == "rastrigin") {
        bitstring::lower_limit = -dejong_rastrigin_limit;
        bitstring::upper_limit = dejong_rastrigin_limit;
    }
    else if (bitstring::function == "michalewicz") {
        bitstring::lower_limit = 0;
        bitstring::upper_limit = 3.141597;
        min = -bitstring::dimensions;
    }
    
    bitstring::range = bitstring::upper_limit - bitstring::lower_limit;
    bitstring::bit_count = ceilf(log2((bitstring::range)*pow(10, 5)));
    bitstring::length = bitstring::bit_count * bitstring::dimensions;

    int aux = 1;
    for (int i = 0; i <= bitstring::bit_count; i++) {
        bitstring::pow2.push_back(aux);
        aux *= 2;
    }

    time_t start, end;
    time(&start);

    genetic p;
    genetic q;
    for (int j=0; j<30;j++) {
        p.population(min);
        q.population(min);
        for (int i = 0; i < genetic::iterations; i++) {
            p.selection(q);
            q.mutation(); 
            q.crossover();
            q.selection(p);
            p.mutation();   
            p.crossover();
        }
        p.best().print();
    }
    time(&end);
    int nothing;
    double time_taken = double(end - start);
    cout << "time taken by program:" << time_taken << endl;

    _getch();
}
