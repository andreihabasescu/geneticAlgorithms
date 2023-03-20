#include "bitstring.h"
#include <cstdlib>
    #include <vector>
#include <iostream>
#include <cmath>

using namespace std;

float pi = 3.14159;

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

 

int bitstring::convertDecimal(int x) {
    int aux = 0;
    for (int i = x * bitstring::bit_count; i< (x+1)*bitstring::bit_count; i++) {
        aux = aux * 2 + binary[i];
    }
    
    return aux;
};

void bitstring::generate_binary() {
	for (int i = 0; i < bitstring::length; i++) {
		int aux=generator();
		bitstring::binary.push_back(aux % 2);
	}
}

void bitstring::generate_decimal() {
    for (int i = 0; i < dimensions; i++) {
        
        decimal.push_back(convertDecimal(i));
    }
}

bitstring::bitstring() {
    bitstring::generate_binary();
    
    bitstring::generate_decimal();
};

bitstring::bitstring(const bitstring& aux) {
    for (int i = 0; i < length; i++) {
        bitstring::binary.push_back(aux.binary[i]);
    }

    for (int i = 0; i < dimensions; i++) {
        bitstring::decimal.push_back(aux.decimal[i]);
    }
};

void bitstring::operator=(const bitstring& aux) {
    for (int i = 0; i < length; i++) {
        bitstring::binary[i] = aux.binary[i];
    }

    for (int i = 0; i < dimensions; i++) {
        bitstring::decimal[i] = aux.decimal[i];
    }
}

void bitstring::neighbour(int i) {
    bitstring::binary[i] = !bitstring::binary[i];
    int m = i / bitstring::bit_count;
    int position = bitstring::bit_count - (i % bit_count);

    if (bitstring::binary[i]) { 
        bitstring::decimal[m] += bitstring::pow2[position - 1]; 
    }
    else {
        bitstring::decimal[m] -= pow2[position - 1];
    }
};

bitstring bitstring::firstImprovement() {
    bitstring best(*this);
    float bestValue = best.eval(bitstring::function);
    for (int i = 0; i < length; i++) {
        best.neighbour(i);

        if (best.eval(bitstring::function) < bestValue) {
            break;
        }
        best.neighbour(i);
    }
    return best;
};

bitstring bitstring::bestImprovement() {
    bitstring candidate(*this);
    bitstring best(*this);

    float best_value = best.eval(bitstring::function);

    for (int i = 0; i < length; i++) {
        candidate.neighbour(i);
        float cand_value = candidate.eval(bitstring::function);

        if (cand_value < best_value) {
            best = candidate;
            best_value = cand_value;
        }
        candidate.neighbour(i);
    }
    return best;
};

bitstring bitstring::worstImprovement() {
    bitstring candidate(*this);
    bitstring best(*this);

    float best_value = best.eval(bitstring::function);

    for (int i = 0; i < length; i++) {
        int aux;
        aux = generator() % length;
        candidate.neighbour(aux);
        float cand_value = candidate.eval(bitstring::function);

        if (cand_value < best_value) {
            best = candidate;
            best_value = cand_value;
        }
        candidate.neighbour(i);
    }
    return best;
};


float bitstring::eval(string func) {
    vector<float> aux;
    
    for (int i = 0; i < bitstring::dimensions; i++) {
        aux.push_back(bitstring::lower_limit + decimal[i] / float(pow2[bit_count] - 1) * range);
        //cout << decimal[i] / float(pow2[bit_count] - 1) << endl;
    }
    
    if (func == "dejong") {
        return deJong(aux);
    }
    else if (func == "schwefel") {
        return schwefel(aux);
    }
    else if (func == "rastrigin") {
        return rastrigin(aux);
    }
    else if (func == "michalewicz") {
        return michalewicz(aux);
    }
};

void bitstring::hillClimbing(string function, string imp) {
    float best_solution = this->eval(function);

    for (int i = 0; i < bitstring::iterations; i++) {
        bitstring v_c;
        while(true) {
            bitstring candidate;
            if (imp == "first") {
                candidate = v_c.firstImprovement();
            }
            else if (imp == "best") {
                candidate = v_c.bestImprovement();
            }
            else if (imp == "worst") {
                candidate = v_c.worstImprovement();
            }

            if (candidate.eval(function) < v_c.eval(function)) {
                v_c = candidate;
            }
            else break;
        }

        float aux = v_c.eval(bitstring::function);
        if (aux < best_solution) {
            *this = v_c;
            best_solution = aux;
        };
    }
};
void bitstring::annealing(string alg, int iterations) {
    float temp = 1000;
    bitstring v_c;

    while (true) {
        float current_sol = v_c.eval(bitstring::function);

        for (int i = 0; i < iterations; i++) {
            int aux = generator() % bitstring::length;

            v_c.neighbour(aux);
            bitstring candidate(v_c);
            v_c.neighbour(aux);

            if (candidate.eval(bitstring::function) < v_c.eval(bitstring::function)) {
                v_c = candidate;
                
            } else if (float(generator() % 100000) / 100000 < expf(-fabsf(candidate.eval(function) - v_c.eval(function)) / temp)) {
                v_c = candidate;
            }
        }
        if ((current_sol - v_c.eval(bitstring::function)) == 0) {
            break;
        }
        temp = temp * 0.95;
    }
    *this = v_c;
};

void bitstring::printBinary() {
    for (int i = 0; i < bitstring::bit_count; i++) {
        cout << bitstring::binary[i];
    }
}

void bitstring::print() {
    std::cout << std::fixed << std::setprecision(5) << this->eval(bitstring::function) << "\n";
}

std::mt19937 bitstring::generator(std::chrono::system_clock::now().time_since_epoch().count());
int bitstring::iterations, bitstring::dimensions, bitstring::dimensionSize, bitstring::bit_count, bitstring::length;
float bitstring::range, bitstring::lower_limit, bitstring::upper_limit;
string bitstring::algorithm, bitstring::improvement, bitstring::function;
