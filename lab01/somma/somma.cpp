#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <queue>
#include <stack>

using namespace std;

/*

*/

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  int n, m;
  in >> n >> m;
  out << (n+m);

  in.close();
  out.close();
  return 0;
}

