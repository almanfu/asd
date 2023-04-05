#include <fstream>
using namespace std;

int main() {
  int N, M;

  // apro il file "input.txt" come input stream e leggo due interi
  ifstream in("input.txt");
  in >> N >> M;

  // apro il file "output.txt" come output stream e scrivo la somma
  ofstream out("output.txt");
  out << N+M <<"\n";

  return 0;
}
