#pragma once
#include <vector>
#include <chrono>
#include <iomanip>
#include <random>

using namespace std;
class bitstring
{
public:
	vector<bool> binary;
	vector<int> decimal;
	vector<float> select;
	
	
	static vector<int> pow2;
	static float range;
	static int dimensions;
	static int bit_count;
	static int length;
	static string function;
	static std::mt19937_64 generator;

	static float lower_limit;
	static float upper_limit;

	float acc_selectionProb;

	int toDecimal(int i);
	void generate();
	void swap(int a, bitstring &aux);

	void mutate(int loc);
	void cross_over(bitstring &aux);
	float eval();
	float fitness(float g);

	bitstring();

	void operator=(const bitstring& v);

	void print();

	
};

