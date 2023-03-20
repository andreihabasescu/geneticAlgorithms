#include "bitstring.h"
#include <iostream>
#include <random>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <utility>

using namespace std;

float deJong(const std::vector<float>& x) {
    float fx = 0;
    for (int i = 0; i < x.size(); i++) {
        fx += powf(x[i], 2);
    }
    return fx;
}
float schwefel(const std::vector<float>& x) {
    float fx = 0;
    for (int i = 0; i < x.size(); i++) {
        fx += (-x[i]) * sinf(sqrtf(fabs(x[i])));
    }
    return fx;
}
float rastrigin(const std::vector<float>& x) {
    float fx = 10 * bitstring::dimensions;

    const float pi = 3.14159;
    for (int i = 0; i < x.size(); i++) {
        fx += (x[i] * x[i] - 10 * cosf(2.0f * pi * x[i]));
    }
    return fx;
}
float michalewicz(const std::vector<float>& x) {
    float fx = 0;

    int m = 10;
    for (int i = 0; i < x.size(); i++) {
        fx += sinf(x[i]) * powf(sinf((i + 1) * x[i] * x[i] / 3.14159), 20);
    }
    return fx * (-1);
}

int bitstring::toDecimal(int p) {
    int sum = 0;
    
    for (int i = 0; i < bit_count; i++) {
        sum += binary[(p - 1) * bit_count + i] * pow2[bit_count - i];
    }
return sum/2;
}

void bitstring::generate() {
	for (int i = 0; i < bitstring::length; i++) {
		bitstring::binary.push_back(generator() % 2);
	}
    for (int i = 1; i <= dimensions; i++) {
        bitstring::decimal.push_back(toDecimal(i));
    }
};

bitstring::bitstring() {
	generate();
};

void bitstring::print() {
    cout << this->eval() << endl;
}

void bitstring::swap(int a, bitstring &aux) {
	bool num = bitstring::binary.at(a);
	bitstring::binary.at(a) = aux.binary.at(a);
	aux.binary.at(a) = num;
}

void bitstring::mutate(int loc) {
    bitstring::binary[loc] = !bitstring::binary[loc];
    int m = loc / bit_count;
    int p = bit_count - (loc % bit_count);
 
    if (bitstring::binary[loc]) {
        decimal[m] += pow2[p - 1]; 
    } else decimal[m] -= pow2[p - 1];
}

void bitstring::cross_over(bitstring &aux) {
	int locus = generator() % bitstring::length;
	
	for (int i = locus; i < length; i++) {
		swap(i, aux);
	}
   
    for (int i = 0; i < dimensions; i++) {
        decimal[i] = toDecimal(i+1);        
        aux.decimal[i] = aux.toDecimal(i+1);
    }
}

float bitstring::eval() {
    vector<float> aux;

    for (int i = 0; i < bitstring::dimensions; i++) {
        aux.push_back(bitstring::lower_limit + decimal[i] / float(pow2[bit_count] - 1) * range);
    }

    if (bitstring::function == "dejong") {
        return deJong(aux);
    }
    else if (bitstring::function == "schwefel") {
        return schwefel(aux);
    }
    else if (bitstring::function == "rastrigin") {
        return rastrigin(aux);
    }
    else if (bitstring::function == "michalewicz") {
        return michalewicz(aux);
    }
};

void bitstring::operator=(const bitstring& v) {
    for (int i = 0; i < length; i++) {
        binary[i] = v.binary[i];
    }
    for (int i = 0; i < dimensions; i++) {
        decimal[i] = v.decimal[i];
    }
}

std::mt19937_64 bitstring::generator(std::chrono::system_clock::now().time_since_epoch().count());

int bitstring::dimensions, bitstring::bit_count, bitstring::length;
float bitstring::range, bitstring::lower_limit, bitstring::upper_limit;
string bitstring::function;
vector<int>bitstring::pow2;