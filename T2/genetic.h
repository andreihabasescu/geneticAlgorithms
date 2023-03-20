#pragma once
#include <vector>
#include "bitstring.h"
using namespace std;
class genetic
{
public:
	static std::mt19937_64 generator;

	vector<bitstring> pop;  //population
	float min;
	static int iterations;
	static int pop_size;

	void population(float min);
	float fitness(int p);
	bitstring best();
	void selection(genetic &aux);
	void mutation();
	void crossover();
};

