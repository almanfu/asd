#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <unordered_map>

using namespace std;

/*

*/

vector<int> icecreamParlor(int m, vector<int> arr) {
    unordered_map<int, int> d = unordered_map<int, int>();
    for(int i=0; i < (int)arr.size(); i++){
        d.insert(pair<int, int>(arr[i], i));
    }
    for(int i=0; i < (int)arr.size(); i++){
        auto j = d.find(arr[i]);
        if(j != d.end()){
          vector<int> v(2);
            v[0] = i; v[1] = j->second;
            return v;
        }
    }
    return {0, 0};
}

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");


  in.close();
  out.close();
  return 0;
}
