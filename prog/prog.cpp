#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <set>

#include <algorithm>

using namespace std;

ofstream out("output.txt");

/*

*/

class Graph{
  public:
  typedef int Node;
  Node i, s, f;

  struct Edge{
    Node me;
    Node node;
    int weight;
    bool isVent;
    bool isMaxed;
    int minWeight, maxWeight;
    Edge(Node u, Node v, int w){
      me = u;
      node = v;
      weight = w;
      isVent = isMaxed = false;
    }
    Edge(Node u, Node v, int minW, int maxW){
      me = u;
      node = v;
      weight = minWeight = minW;
      maxWeight = maxW;
      isVent = true;
      isMaxed = false;
    }
    friend bool operator<(const Edge e1, const Edge e2){
      return e1.weight < e2.weight;
    }
  };
  list<Edge*> vents;
  private:
  vector<Node> V;
  vector<list<Edge*>> adj;
  int n;
  public:
  Graph(int nodes){
    n = nodes;
    adj = vector<list<Edge*>>(n);
    V = vector<Node>(n);
    for(Node u=0; u < n; u++)
      V[u] = u;
  }
  ~Graph(){
    for(Node u=0; u < n; u++){
      for(Edge *e: adj[u])
        delete e;
    }
  }
  bool has(Node u){
    return 0<=u && u < n;
  }
  void insertEdge(Node u, Node v, int weight=1){
    if(has(u) && has(v) && u != v && weight >= 1)
      adj[u].push_front(new Edge(u, v, weight));
  }
  void insertEdge(Node u, Node v, int minWeight, int maxWeight){
    if(has(u) && has(v) && u != v && minWeight >= 1 && maxWeight >= 1){
      Edge *e = new Edge(u, v, minWeight, maxWeight);
      adj[u].push_front(e);
      vents.push_front(e);
    }
  }
  // Utility Functions
  void print_adj(){
    for(Node u=0; u < n; u++){
      cout << u << ':';
      for(Edge *e: adj[u])
        cout << '(' << e->node << ',' << e->weight << ')' << ' ';
      cout << endl;
    }
  }

  //Strictly better
  bool isBetter(int res1, int res2){
    if(res1==1 && res2!=1)
      return true;
    else if(res1==0 && res2==2)
      return true;
    else
      return false;
  }

  int getRes(int dimp, int dstud){
    if(dimp < dstud)
      return 1;
    else if(dimp==dstud)
      return 0;
    else
      return 2;
  }

  void dijkstra1(){
    typedef pair<int, Node> Li;
    priority_queue<Li, vector<Li>, greater<Li>> q;
    const int INF = INT32_MAX;

    int bestRes;
    vector<int> bestDistance(n);
    vector<Node> bestPreds(n);
    list<int> bestVentsWeights;

    
    
    vector<Edge*> predsEdge(n);
    
    bool allMaxed=false;
    bool firstRun=true;


    while(!allMaxed){
      //set max weights to Student path
      if(!firstRun){
        allMaxed=true;
        //cout << "one check done" << endl;

        Edge* pred = predsEdge[s];
        while(allMaxed && pred->me != f){
          //pred is an edge in the least cost path f->s
          if(pred->isVent && !pred->isMaxed){
            //cout << "found " << pred->me << ", " << pred->node << endl;
            allMaxed = false;
            pred->weight = pred->maxWeight;
            pred->isMaxed = true;
          }
          pred = predsEdge[pred->me];                
        }// => pred->me == f

        if(allMaxed && pred->isVent && !pred->isMaxed){
            allMaxed = false;
            pred->weight = pred->maxWeight;
            pred->isMaxed = true;
        }
      }

      vector<int> distance(n);
      vector<Node> preds(n); 
      predsEdge.clear();

      ///MST FROM FABLAB
      for(Node u: V)
        distance[u] = INF;
      distance[f] = 0;
      q.push(Li(0, f));
      //Dijkstra 
      while(!q.empty()){
        Li li = q.top(); q.pop();
        Node u = li.second;
        for(Edge *e: adj[u]){
          int altdistance = e->weight+distance[u];
          if(distance[e->node] > altdistance){
            //Rimuovo vecchio lato
            preds[e->node]= u;
            predsEdge[e->node]= e;
            distance[e->node] = altdistance;
            q.push(Li(distance[e->node], e->node));
          }
        }
      }
      
      if(firstRun){
        bestRes = getRes(distance[i], distance[s]);
        bestDistance = distance;
        bestPreds = preds;

        for(Edge *e : vents)
          bestVentsWeights.push_front(e->weight);
        
        firstRun=false;
      }
      else{
        int currRes = getRes(distance[i], distance[s]);
        if(isBetter(currRes, bestRes)){
          bestRes = currRes;
          bestDistance = distance;
          bestPreds = preds;

          bestVentsWeights.clear();
          for(Edge *e : vents)
            bestVentsWeights.push_front(e->weight);
          
        }
      }

    };
    ///Best settings FOUND

    // First output line
    out << bestRes << '\n';

    //Second output line
    out << bestDistance[i] << ' ' << bestDistance[s] << '\n';

    //Third output line
    for(int weight : bestVentsWeights){
      out << weight << ' ';
    }
    out << '\n';
    
    //Fourth output line
    int R=1;
    Node cursor = i;
    while(cursor != f){
      R++;
      cursor = bestPreds[cursor];
    }
    out << R << '\n';

    //Fifth output line
    cursor = i;
    while(cursor != f){
      out << cursor << ' ';
      cursor = bestPreds[cursor];
    }
    out << f << '\n';
    return;
  }

};

int main(int argc, char *argv[]){
  ifstream in("input.txt");

  Graph::Node i, s, f;
  int n, m, k;
  in >> n >> m >> k;
  in >> i >> s >> f;
  
  Graph g(n);

  g.i = i;
  g.s = s;
  g.f = f;

  for(int i=0; i < m; i++){
    Graph::Node u, v;
    int weight;
    in >> u >> v >> weight;
    // using transposed graph
    g.insertEdge(v, u, weight);
  }

  for(int i=0; i < k; i++){
    Graph::Node u, v;
    int minWeight, maxWeight;
    in >> u >> v >> minWeight >> maxWeight;
    g.insertEdge(v, u, minWeight, maxWeight);
  }

  g.dijkstra1();

  in.close();
  out.close();
  return 0;
}
