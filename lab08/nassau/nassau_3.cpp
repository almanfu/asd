#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <unordered_map>
#include <iomanip>
#include <random>

using namespace std;
/*
nassau_3 montecarlo simulation

accurate reasonably up to 10^-3
*/

// la probabilità di colpire vascello, vascellino o fregata NON è uniforme ma dipende da quanti ci sono

int main(int argc, char *argv[])
{
  ifstream in("input.txt");
  ofstream out("output.txt");
  int V, F, M;
  std::random_device rd;
  std::mt19937 gen(rd());

  in >> V >> F >> M;

  long double res = 0;
  long double pres = 0;
  for (int i = 1; i <= 100000; i++)
  {
    // costs O(M)<=5000
    int V1 = V, v1 = 0, F1 = F, M1 = M;

    while (M1 > 0)
    {
      std::uniform_int_distribution<> distrib(0, V1 + v1 + F1 - 1);
      int boat = distrib(gen);
      if (boat < V1)
      {
        V1--;
        v1++;
        M1--;
      }
      else if (boat < V1 + v1)
      {
        v1--;
        M1--;
      }
      else if (boat < V1 + v1 + F1)
      {
        F1--;
        M1--;
      }
    }
    pres += (long double)(V1 + v1) * (long double)F1;
    if (i % 1000 == 0)
    {
      res += pres / 1000.0;
      pres = 0;
    }
  }
  res /= 100.0;

  out << scientific << setprecision(10)
      << res << endl;

  in.close();
  out.close();
  return 0;
}