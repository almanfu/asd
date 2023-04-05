#include <fstream>
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

#ifdef EVAL
ifstream in("input.txt");
ofstream out("output.txt");
#else
istream &in(cin);
ostream &out(cout);
#endif


#define MX 1000000000

struct edge{
  int dest;
  int weight;
  int start;
  int up;
  int down;

  int nextActive(int t) {
    if (up < weight)
      return MX+1;
    if (t < start)
      return start;
    int cycle = up + down;
    int pos = (t - start) % cycle;
    if (pos < up && up - pos >= weight)
      return t;
    else
      return t + cycle - pos;    
  }
};

struct nodo {
  vector<edge> adj;
  bool visited=false;
  int distance = MX;
  int pred = -1;
};

int N, M;
vector<nodo> grafo;

void read() {
  in>>N>>M;
  grafo.resize(N);
  for(int i=0;i<M;i++){
    int f,t,w,s,u,d;
    in>>f>>t>>w>>s>>u>>d;
    edge e;
    e.dest = t;
    e.weight = w;
    e.start = s;
    e.up = u;
    e.down = d;
    grafo[f].adj.push_back(e);
    e.dest = f;
    grafo[t].adj.push_back(e);
  }
}

int solve() {
  priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int,int>>> q;
  q.push(make_pair(0, 0));
  grafo[0].distance = 0;
  while(!q.empty()){
    auto p=q.top();
    q.pop();
    int n = p.second;
    if(!grafo[n].visited){
      grafo[n].visited=true;
      for(edge& e:grafo[n].adj){
        int d2 = e.nextActive(grafo[n].distance) + e.weight;
        if(d2 < grafo[e.dest].distance){
          grafo[e.dest].pred = n;
          grafo[e.dest].distance = d2;
          q.push(make_pair(d2, e.dest));
        }
      }
    }
  }
  if(grafo[N-1].distance == MX)
    return -1;
  else
    return grafo[N-1].distance;
}

int main(void) {
  read();
  int res = solve();
  out<<res<<endl;
  if (res == -1)
    return 0;
  vector<int> v;
  int cur = N-1;
  while(cur!=0) {
    v.push_back(cur);
    cur = grafo[cur].pred;
  }
  v.push_back(0);
  for(int i=v.size()-1;i>=0;i--)
    out<<v[i]<<" "<<endl;
  return 0;
}

