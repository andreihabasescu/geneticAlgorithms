#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "bitstring.h"
using namespace std;
using namespace std::chrono;

float dejong_rastrigin_limit = 5.12;

float schwefel_limit = 500;

int main()
{

    string function = "michalewicz";
    string algorithm = "hillclimbing";
    string improvement = "first";

    bitstring::dimensions = 30;
    bitstring::iterations = 1000;

    bitstring::algorithm = algorithm;
    bitstring::improvement = improvement;
    bitstring::function = function;

    if (function == "dejong") {
        bitstring::lower_limit = -dejong_rastrigin_limit;
        bitstring::upper_limit = dejong_rastrigin_limit;
    }
    else if (function == "schwefel") {
        bitstring::lower_limit = -schwefel_limit;
        bitstring::upper_limit = schwefel_limit;
    }
    else if (function == "rastrigin") {
        bitstring::lower_limit = -dejong_rastrigin_limit;
        bitstring::upper_limit = dejong_rastrigin_limit;
    }
    else if (function == "michalewicz") {
        bitstring::lower_limit = 0;
        bitstring::upper_limit = 3.141597;
    }

    bitstring::range = bitstring::upper_limit - bitstring::lower_limit;
    bitstring::bit_count = ceilf(log2((bitstring::range)*pow(10,5)));
    bitstring::dimensionSize = ceilf(log2((bitstring::upper_limit - bitstring::lower_limit) * pow(10, 5)));
    bitstring::length = bitstring::bit_count * bitstring::dimensions;
    int aux = 1;
    for (int i = 0; i <= bitstring::bit_count; i++) {
        bitstring::pow2.push_back(aux);
        aux *= 2;
    }
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 30; i++) {
        bitstring aux;

        if (algorithm == "hillclimbing") {
            if (improvement == "best") {
                aux.hillClimbing(function, improvement);
            }
            else if (improvement == "worst") {
                aux.hillClimbing(function, improvement);
            }
            else if (improvement == "first") {
                aux.hillClimbing(function,improvement);
            }
        }
        else if (algorithm == "annealing") {
            aux.annealing(bitstring::function,5000);
        }
        aux.print(); 
        
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<seconds>(stop - start);
    cout << duration.count() << endl;
    string c;
    cin >> c;
}
