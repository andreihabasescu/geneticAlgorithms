#include "genetic.h"
#include <iostream>
using namespace std;

void genetic::population(float min) {
	for (int i = 0; i <= genetic::pop_size; i++) {
		pop.push_back(bitstring());
	}
	this->min = min;
};
float genetic::fitness(int i) {
	return 1/(genetic::pop[i].eval() + 0.00001 - min);
};

bitstring genetic::best() {
	int best = 0;
	for (int i = 1; i < genetic::pop_size; i++) {
		if (fitness(i) > fitness(best)) best = i;
	}
	return genetic::pop[best];
}

void genetic::selection(genetic &aux) {
	
	vector<float> sel_prob;
	vector<float> eval;
	vector<float> q;
	float T = 0;
	
	for (int i = 0; i < genetic::pop_size; i++) { //evaluate + total fitness
		eval.push_back(fitness(i));
		T += eval[i];
	} 
	for (int i = 0; i < genetic::pop_size; i++) { //individual sel. prob.
		sel_prob.push_back(eval[i] / T);
	}
	q.push_back(0);
	for (int i = 0; i < genetic::pop_size; i++) { //accumulated sel. prob.
		q.push_back(q[i] + sel_prob[i]);
	}

	for (int i = 0; i < 7; i++) {
		aux.pop[i] = genetic::best();
	}
	//cout << "best - "<<  aux.pop[0].eval() << endl;

	for (int i = 1; i < genetic::pop_size; i++) {
		float r = (generator() % 100000) / (float)100000;
		//cout << "random = "<< r << endl;
		for (int j = 0; j < genetic::pop_size; j++) {
			if (q[j] < r && r <= q[j + 1]) {
				aux.pop[i] = genetic::pop[j];
				break;
			}
		}
	}
}

float chance(float a) {
	return a*a*0.8*(bitstring::dimensions/30);
}

void genetic::mutation() {
	float limitA = 0;
	float limit = 0;
	if (bitstring::function == "rastrigin") {
		limitA = 0.003;
	}
	else limitA = 0.0005;

	for (int i = 0; i < genetic::pop_size; i++) {

		if (bitstring::function == "rastrigin") limit = fabs(chance(genetic::fitness(i)));

		for (int j = 0; j < bitstring::length; j++) {
			float aux = (generator() % 100000) / (float)100000;

			if (aux < limitA-limit) {
				genetic::pop[i].mutate(j);
			}
		}
	}
	//cout << "lim = " << limit << endl;
}

void genetic::crossover() {
	float limit = 0;
	for (int i = 1; i < genetic::pop_size; i++) {
		float aux = (generator() % 100000) / (float)100000;

		//limit = chance(genetic::fitness(i));
		
		if (aux < 0.4+limit) {
			//cout << "limit = " << limit << endl;
			float pos = generator() % (pop_size);
			pos++;
			genetic::pop[i].cross_over(pop[pos]);
		}
	}
}

int genetic::iterations, genetic::pop_size;
std::mt19937_64 genetic::generator(std::chrono::system_clock::now().time_since_epoch().count());
