#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>

#include <cstdlib>

using namespace std;

/*

*/

int main(int argc, char *argv[]){
  ofstream in("input.txt");
  const int N = 300000;
  in << N << endl;

  // random weights
  for (int u = 0; u < N; u++){
    in << rand() << ' ';
  }

  in << endl;

  //random edges
  for (int u = 0; u < N; u++)
  {
    in << u << ' ' << (u + 1) << endl;
  }

  in.close();
  return 0;
}
