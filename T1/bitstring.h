#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>


using namespace std;

class bitstring
{
public:
	vector<bool> binary;
	vector<int> decimal;

	static std::mt19937 generator;
	static float lower_limit;
	static float upper_limit;
	static float range;

	static string algorithm, improvement,function;

	static int bit_count,length,dimensionSize;
	static int dimensions, iterations;

	static vector<int> pow2;

	bitstring();
	bitstring(const bitstring& x);

	void operator=(const bitstring&);

	int convertDecimal(int x);
	void generate_binary();
	void generate_decimal();

	void printBinary();
	void print();

	void neighbour(int i);

	bitstring firstImprovement();
	bitstring bestImprovement();
	bitstring worstImprovement();

	float eval(string alg);

	void hillClimbing(string alg, string type);
	void annealing(string alg, int iter);
};

