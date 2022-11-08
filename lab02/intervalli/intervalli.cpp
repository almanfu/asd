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
    return r1.len <= r2.len;    
  }
  friend bool operator>=(Range r1, Range r2){
    return r1.len >= r2.len;    
  }
  friend bool operator<(Range r1, Range r2){
    return r1.len < r2.len;    
  }
  friend bool operator>(Range r1, Range r2){
    return r1.len > r2.len;
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

  Range I;
  list<Range>* S = new list<Range>;

  for(int i=0; i < n; i++){
    Range r;
    in >> r;
    if(r.contains(I) || I.empty()){
      I = r;
      S->clear();
    }
    else if(!I.disjoint(r)){
      //#Update all s in S
      list<Range>* S1 = new list<Range>;
      for(Range s: *S){
        if(r.contains(s)){
          // delete from S
        }else if(s.contains(r)){
          //split in two
          S1->push_front(Range(s.s, r.s));
          S1->push_front(Range(r.e, s.e));
        }else{
          //remove overlap from s
          s.subtract(r);
          S1->push_front(s);
        }
      }
      delete S;
      S = S1;
      //#Update I
      if(!I.contains(r)){
        if(I.e < r.e){
        // I ---|
        // r   ----|
          I.e = r.e;
        }else{
        // I   |---
        // r |---
          I.s = r.s;
        }
      }
    }
    else{
      //#Update I, add s
      if(I.e < r.e){
      // I ---|
      // r       |----|
        S->push_front(Range(I.e, r.s));
        I.e = r.e;
      }else{
      // I       |---
      // r |---| 
        S->push_front(Range(r.e, I.s));
        I.s = r.s;
      }
    }
  }

  S->push_front(Range());

  Range maxRange = *max_element(S->begin(), S->end());

  out << maxRange;

  delete S;
  in.close();
  out.close();
  return 0;
}