#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>

using namespace std;

bool INFO = false;

/*

*/

void info(){
  if (!INFO)
    return void();

  ofstream info("info.txt");

  info.close();
}

/*

*/

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  if(argv[1] == "INFO")
    INFO = true;

  info();
  in.close();
  out.close();
  return 0;
}
