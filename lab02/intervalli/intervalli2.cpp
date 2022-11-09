#include <iostream>
#include <fstream>

#include <bitset>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <map>
#include <stack>
#include <set>

#include <string>
#include <cctype>

#include <utility>
#include <functional>
#include <algorithm>

using namespace std;

/*
O(n^2)
*/

class Range{
  public:
  int s, e, len;
  Range(){
    s = e = len = 0;
  }
  Range(int s, int e){
    if(s<=e){
      this->s = s;
      this->e = e;
      this->len = e - s;
    }else{
      this->s = 0;
      this->e = 0;
      this->len = 0;
    }
  }
  bool empty(){
    return s == 0 && e == 0;
  }
  Range overlap(Range target){
    int a, b;
    a = max(s, target.s);
    b = min(e, target.e);
    return Range(a, b);
  }
  bool disjoint(Range target){
    return overlap(target).empty();
  }
  bool contains(Range target){
    Range ol = overlap(target);
    return ol == target;
  }
  bool subtract(Range target){
    if(!contains(target) && !target.contains(*this) &&
        !overlap(target).empty()){
      if(s<target.s){
        e = target.s;
        len = e-s;
      }else{
        s = target.e;
        len = e-s;
      }
      return true;
    }
    else
      return false;
  }
  friend bool operator==(Range r1, Range r2){
    return r1.s == r2.s && r1.e == r2.e;    
  }
  friend bool operator!=(Range r1, Range r2){
    return !(r1 == r2);    
  }
  friend bool operator<=(Range r1, Range r2){
    return r1.s <= r2.s;    
  }
  friend bool operator>=(Range r1, Range r2){
    return r1.s >= r2.s;    
  }
  friend bool operator<(Range r1, Range r2){
    return r1.s < r2.s;    
  }
  friend bool operator>(Range r1, Range r2){
    return r1.s > r2.s;
  }
  friend ostream& operator<<(ostream& os, Range r){
    if(!r.empty())
      return os << r.s << " " << r.e;
    else
      return os << "0";
  }
  friend istream& operator>>(istream& is, Range& r){
    int a, b;
    is >> a >> b;
    r = Range(a, b);
    return is;
  }
};

int main(int argc, char *argv[]){
  ifstream in("input.txt");
  ofstream out("output.txt");

  int n;
  in >> n;

  Range maxGap = Range();
  list<Range>* S = new list<Range>;

  for(int i=0; i < n; i++){
    Range r;
    in >> r;
    S->push_front(r);
  }

  S->sort();
  if(S->size() >= 1){
    Range r1 = S->front();S->pop_front();
    Range r2 = Range();
    while(!S->empty()){
      r1 = r1.contains(r2)?r1:r2;
      r2 = S->front();S->pop_front();
      if(r1.disjoint(r2)){
        Range newGap(r1.e, r2.s);
        if(newGap.len > maxGap.len){
          maxGap = newGap;
        }else if(newGap.len == maxGap.len && newGap.s < maxGap.s){
          maxGap = newGap;
        }
      }
    }
  }

  out << maxGap;
  delete S;
  in.close();
  out.close();
  return 0;
}