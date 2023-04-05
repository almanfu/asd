#include <fstream>
using namespace std;

int R, C;

// Matrice con i valori
int A[1001][1001];

// Matrice di appoggio. S[i][j] é uguale alla somma della riga i fino
// all'elemento j
int S[1001][1001];

int main(void) {
  // i/o
  ifstream in("input.txt");
  ofstream out("output.txt");

  // La prima riga dell'input contiene due interi, R e C, rispettivamente il
  // numero di righe e di colonne della matrice.
  in >> R >> C;

  for(int i=1; i<=R; i++) {
    //mantiene la somma della riga corrente
    int sumr = 0;

    for(int j=1; j<=C; j++) {
      in >> A[i][j];
      sumr += A[i][j];
      S[i][j] = sumr;
    }
  }
  int sol = -1;

  // per ogni coppia di colonne
  for(int c1=1; c1<=C; c1++)
    for(int c2=c1; c2<=C; c2++) {
      int tot = 0;

      // visito la colonna con l'algoritmo lineare per la sottosequenza
      for(int r=1; r<=R; r++) {
        int cur = S[r][c2] - S[r][c1-1];
        tot = max(cur, cur+tot);
        sol = max(sol, tot);
      }
    }

  out << sol <<endl;
  return 0;
}
