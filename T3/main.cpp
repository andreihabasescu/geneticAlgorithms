#include <vector>
#include <chrono>
#include <random>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>

using namespace std;
using namespace std::chrono;

class Cromozom {
  public:
  std::vector<int> cromozom;
  float path_cost;
public:
  static std::map<std::pair<int, int>, float> cost;
  static std::vector<int> numbers;
  static int size;
  static std::mt19937 generator;
  Cromozom() {
    for(int i = 0; i < size; i++){
      int n = generator() % (size - i);
      cromozom.push_back(numbers[n]);
      std::swap(numbers[n], numbers[size - i - 1]);
    }
    ChangePathCost();
  }

  Cromozom(const Cromozom& c){
    for(int i = 0; i < size; i++){
      cromozom.push_back(c.cromozom[i]);
    }
    path_cost = c.path_cost;
  }
  void ChangePathCost(){
    path_cost = 0;
    for(int i = 0; i < size-1; i++){
      path_cost += cost[std::make_pair(cromozom[i], cromozom[i+1])];
    }
    path_cost += cost[std::make_pair(cromozom[0], cromozom[size-1])];
  }
  float GetPathCost(){
    return path_cost;
  }
  void Print(){
    for(int i = 0; i < cromozom.size(); i++){
      std::cout << " " << cromozom[i];
    }
    std::cout << "\n";
  }
  int Position(int &city){
    for(int i = 0; i<cromozom.size(); i++){
      if(cromozom[i] == city) return i;
    }
  }
  void Crossover(Cromozom &c){
    std::vector<int> n;
    float new_path_cost = 0;
    int city = generator() % size;
    n.push_back(city);
    int city1, city2, city3, city4;
    for(int i = 0; i<size-1; i++){
      int size1 = cromozom.size();
      int size2 = c.cromozom.size();
      std::map<int, float> neighbour;
      city1 = (Position(city) + size1 + 1) % size1;
      city2 = (c.Position(city) + size2 + 1) % size2;
      city3 = (city1 + size1 - 2) % size1;
      city4 = (city2 + size2 - 2) % size2;
      neighbour[cromozom[city1]] = cost[std::make_pair(city, cromozom[city1])];
      neighbour[c.cromozom[city2]] = cost[std::make_pair(city, c.cromozom[city2])];
      neighbour[cromozom[city3]] = cost[std::make_pair(city, cromozom[city3])];
      neighbour[c.cromozom[city4]] = cost[std::make_pair(city, c.cromozom[city4])];
      cromozom.erase(std::remove(cromozom.begin(), cromozom.end(), city));
      c.cromozom.erase(std::remove(c.cromozom.begin(), c.cromozom.end(), city));
      city = neighbour.begin()->first;
      for(auto it : neighbour){
        if(it.second < neighbour[city])
          city = it.first;
      }
      n.push_back(city);
      new_path_cost += neighbour[city];
    }
    cromozom.erase(std::remove(cromozom.begin(), cromozom.end(), city));
    c.cromozom.erase(std::remove(c.cromozom.begin(), c.cromozom.end(), city));
    for(int i = 0; i<size; i++){
      cromozom.push_back(n[i]);
      c.cromozom.push_back(n[i]);
    }
    path_cost = new_path_cost;
    path_cost += cost[std::make_pair(cromozom[0], city)];
    c.path_cost = path_cost;
  }
  void Mutation(){
    int f = generator() % size;
    int s = generator() % size;
    int city = cromozom[f];
    path_cost = path_cost - cost[std::make_pair(cromozom[(f-1+size)%size], cromozom[f])] - cost[std::make_pair(cromozom[(f+1)%size], cromozom[f])] + cost[std::make_pair(cromozom[(f-1+size)%size], cromozom[(f+1)%size])];
    cromozom.erase(cromozom.begin()+f);
    cromozom.insert(cromozom.begin()+s, city);
    path_cost = path_cost - cost[std::make_pair(cromozom[(s-1+size)%size], cromozom[(s+1)%size])] + cost[std::make_pair(cromozom[(s-1+size)%size], cromozom[s])] + cost[std::make_pair(cromozom[(s+1)%size], cromozom[s])];
  }
  void Swap(){
    int f = generator() % size;
    int s = generator() % size;
    path_cost = path_cost - cost[std::make_pair(cromozom[(f-1+size)%size], cromozom[f])] - cost[std::make_pair(cromozom[(f+1)%size], cromozom[f])];
    path_cost = path_cost - cost[std::make_pair(cromozom[(s-1+size)%size], cromozom[s])] - cost[std::make_pair(cromozom[(s+1)%size], cromozom[s])];
    std::swap(cromozom[f], cromozom[s]);
    path_cost = path_cost + cost[std::make_pair(cromozom[(f-1+size)%size], cromozom[f])] + cost[std::make_pair(cromozom[(f+1)%size], cromozom[f])];
    path_cost = path_cost + cost[std::make_pair(cromozom[(s-1+size)%size], cromozom[s])] + cost[std::make_pair(cromozom[(s+1)%size], cromozom[s])];
  }
  void Inverse(){
    int f = generator() % size;
    int s = generator() % size;
    if(s > f) std::swap(s, f);
    for(int i = s; i <= f; i++){
      path_cost = path_cost - cost[std::make_pair(cromozom[(i-1+size)%size], cromozom[i])];
    } 
    path_cost = path_cost - cost[std::make_pair(cromozom[f], cromozom[(f+1)%size])];
    std::vector<int> inv;
    inv.insert(inv.begin(), cromozom.begin()+s, cromozom.begin()+f+1);
    cromozom.erase(cromozom.begin()+s, cromozom.begin()+f+1);
    cromozom.insert(cromozom.begin()+s, inv.rbegin(), inv.rend());
    for(int i = s; i <= f; i++){
      path_cost = path_cost + cost[std::make_pair(cromozom[(i-1+size)%size], cromozom[i])];
    }
    path_cost = path_cost + cost[std::make_pair(cromozom[f], cromozom[(f+1)%size])];
  }
  void Simulated(){
    float temperature = 1000;
    while(true){
      float cur_solution = path_cost;
      for(int i = 0; i < 1000; i++){
        float load;
        Cromozom mu(*this);
        Cromozom inv(*this);
        Cromozom sw(*this);
        mu.Mutation();
        inv.Inverse();
        sw.Swap();
        float min = mu.path_cost;
        if(min > inv.path_cost) min = inv.path_cost;
        //if(min > sw.path_cost) min = sw.path_cost;
        if(min < path_cost){
          this->path_cost = min;
        }
        else if(float(generator() % 100000) / 100000 < expf(-fabsf(min - path_cost) / temperature)){
          this->path_cost = min;
        }
        if(path_cost == mu.path_cost) *this = mu;
        else if(path_cost == inv.path_cost) *this = inv;
        else if(path_cost == sw.path_cost) *this = sw;

        //std::cout<<cur_solution<<std::endl;
        //load = (float)i/10;
        //cout<<load<<'%'<<endl;
      }
      if(temperature <= 0.00001) break;
      temperature *= 0.97;
      cout<<temperature<<endl;
    }
  }
};

std::map<std::pair<int, int>, float> Cromozom::cost;
std::vector<int> Cromozom::numbers;
int Cromozom::size;
std::mt19937 Cromozom::generator(std::chrono::system_clock::now().time_since_epoch().count());

class Populatie {
  std::vector<Cromozom> P;
  float max_fitness = 0.01;
public:
  static float min;
  Populatie(int size){
    for(int i = 0; i < size; i++){
      P.push_back(Cromozom());
    }
  }
  float fitness(const float &cost){
    return 1/(cost-min+0.00001);
  }
  Cromozom best(){
    int best = 0;
    for(int i = 1; i < P.size(); i++){
      if(fitness(P[i].GetPathCost()) > fitness(P[best].GetPathCost())){
        best = i;
      }
    }
    return P[best];
  }

  void Selection(Populatie &next){
    std::vector<float> eval;
    float T = 0;
    std::vector<float> prob;
    std::vector<float> q;
    for(int i = 0; i < P.size(); i++){
      eval.push_back(fitness(P[i].GetPathCost()));
      if(eval[i] > max_fitness) max_fitness = eval[i];
      T += eval[i];
    }
    for(int i = 0; i < P.size(); i++){
      prob.push_back(eval[i]/T);
    }
    q.push_back(0);
    for(int i = 0; i < P.size(); i++){
      q.push_back(q[i] + prob[i]);
    }

    int elitism = 15;

    next.P[0] = best();
    for (int i=1; i<elitism; i++) {
      next.P[i] = next.P[0];
    }
    

    for(int i = elitism+1; i < P.size(); i++){
      float r = float(Cromozom::generator() % 100000) / 100000;
      for(int j = 0; j < q.size() ; j++) {
        if(q[i] < r && r <= q[j+1]) {
          next.P[i] = P[j];
          break;
        }
      }
    }
  }

  float chance(float a) {
    float b = a*1000;
	  return b*b;
  }

  void Mutation(){
    for (int i = 1; i < P.size(); i++) {
        float limit = 0.75;
        //float limitA = fabs(chance(fitness(P[i].path_cost)));
        //std::cout <<"fitness "<< fitness(P[i].path_cost) << "\n";
        if (float(Cromozom::generator() % 100000) / 100000 < (limit) /** (1-(fitness(P[i].path_cost)/max_fitness))*/) {
          P[i].Inverse();
        }
    }
  }

  void Crossover(){
    for (int i = 1; i < P.size() - 1; i = i + 2) {
      if (float(Cromozom::generator() % 100000) / 100000 < 0.8) {
        P[i].Crossover(P[i + 1]);
        P[i+1].Inverse();
      }
    }
  }

};

float Populatie::min;

int main(int argc, char* argv[]) {
  std::cout<<argv[1]<<std::endl;
  std::ifstream file;
  file.open("rl5934.tsp");
  file >> Cromozom::size;

  if (file) {
    std::cout<<"working"<<std::endl;
  } else {
    std::cout<<"not working"<<std::endl;
  }

  std::map<int, std::pair<float, float>> coord;
  for(int i = 0; i < Cromozom::size; i++){
    float x, y;
    Cromozom::numbers.push_back(i);
    file >> x;
    file >> x >> y;
    coord[i] = std::make_pair(x, y);
  }
  Populatie::min = sqrt(pow(coord[0].first-coord[1].first, 2) + pow(coord[0].second-coord[1].second, 2));
  for(int i = 0; i < Cromozom::size-1; i++){
    for(int j = i+1; j < Cromozom::size; j++){
      Cromozom::cost[std::make_pair(i, j)] = sqrt(pow(coord[i].first-coord[j].first, 2) + pow(coord[i].second-coord[j].second, 2));
      Cromozom::cost[std::make_pair(j, i)] = Cromozom::cost[std::make_pair(i, j)];
      if(Populatie::min < Cromozom::cost[std::make_pair(i, j)]){
        Populatie::min = Cromozom::cost[std::make_pair(i, j)];
      }
    }
  }

  auto start = high_resolution_clock::now();
  float load;
  /*for(int i = 0; i < 1; i++){
    Populatie P1(100);
    Populatie P2(100);
    for(int j = 0; j < 1000; j++){
      P1.Selection(P2);
      P2.Mutation();
      P2.Crossover();
      P2.Selection(P1);
      P1.Mutation();
      P1.Crossover();
      //std::cout << P1.best().GetPathCost() << "\n";
      load = (float)j/10;
      cout<<load<<'%'<<endl;

    }
    std::cout << std::endl << std::endl  << P1.best().GetPathCost() << "\n";
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);

    cout << endl << duration.count() << "s" << endl;
  }*/


for(int i = 0; i < 1; i++){
    Cromozom c;
    c.Simulated();
    std::cout << c.path_cost << "\n";
 }
 auto stop = high_resolution_clock::now();
  auto duration = duration_cast<seconds>(stop - start);

    cout << endl << duration.count() << "s" << endl;
  // return 0;
}