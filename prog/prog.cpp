#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <set>

#include <algorithm>

using namespace std;

const bool LOCAL = false;
ofstream out("output.txt");


/*

*/

class Graph{
  public:
  typedef int Node;
  Node i, s, f;

  struct Edge{
    Node node;
    int weight;
    bool isVent;
    int minWeight, maxWeight;
    Edge(Node n, int w){
      node = n;
      weight = w;
      isVent = false;
    }
    Edge(Node n, int minW, int maxW){
      node = n;
      weight = minWeight = minW;
      maxWeight = maxW;
      isVent = true;
    }
    friend bool operator<(const Edge e1, const Edge e2){
      return e1.weight < e2.weight;
    }
  };
  list<Edge*> vents;
  enum EdgeDirection {DIRECTED, UNDIRECTED};
  enum EdgeWeight {WEIGHTED, UNWEIGHTED};
  EdgeDirection edgeDirection;
  EdgeWeight edgeWeight;
  private:
  vector<Node> V;
  vector<list<Edge*>> adj;
  int n;
  public:
  Graph(int nodes, EdgeDirection eDir, EdgeWeight eWeight){
    edgeDirection = eDir;
    edgeWeight = eWeight;
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
  bool undirected(){
    return edgeDirection == UNDIRECTED;
  }
  bool unweighted(){
    return edgeWeight == UNWEIGHTED;
  }
  void insertEdge(Node u, Node v, int weight=1){
    if(unweighted())
      weight  = 1;
    if(has(u) && has(v) && u != v && weight >= 1){
      adj[u].push_front(new Edge(v, weight));
      if(undirected())
        adj[v].push_front(new Edge(u, weight));
    }
  }
  void insertEdge(Node u, Node v, int minWeight, int maxWeight){
    if(unweighted() || undirected())
      throw exception();
    if(has(u) && has(v) && u != v && minWeight >= 1 && maxWeight >= 1){
      Edge *e = new Edge(v, minWeight, maxWeight);
      adj[u].push_front(e);
      vents.push_front(e);
    }
  }
  // Utility Functions
  void print_adj(){
    for(Node u=0; u < n; u++){
      cout << u << ':';
      if(unweighted()){
        for(Edge *e: adj[u])
          cout << e->node << ' ';
      }else{
        for(Edge *e: adj[u])
          cout << '(' << e->node << ',' << e->weight << ')' << ' ';
      }
      cout << endl;
    }
  }

  void bfs(Node r){
    queue<Node> q;
    bool *visited = new bool[n]{};
    if(has(r)){
      q.push(r);
      visited[r] = true;
    }
    while(!q.empty()){
      Node u = q.front(); q.pop();
      cout << u << ' ';
      for(Edge *e: adj[u]){
        if(!visited[e->node]){
          q.push(e->node);
          visited[e->node] = true;
        }
      }
    }
    cout << endl;
    delete[] visited;
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
    vector<int> bestDistanceImp(n);
    vector<int> bestDistanceStud(n);
    list<int> bestVentsWeights;
    vector<list<Node>> bestPredsImp(n);

    bool allMaxed=false;
    bool firstRun=true;


    vector<list<Node>> predsStud(n);

    while(!allMaxed){

      
      //set max weights to Student path
      if(!firstRun){
        allMaxed=true;
        //cout << "one check done" << endl;

        //cursor is a node in the least cost path s->f
        Node cursor = f;
        while(cursor != s){
          //(cursorPred, cursor) is an edge in the least cost path
          // s->f
          Node cursorPred = *predsStud[cursor].begin();

          //Finding Edge object
          auto iter = adj[cursorPred].begin();
          auto iterend = adj[cursorPred].end();
          auto e = *iter;
          while(iter != iterend && e->node != cursor){
            iter++;
            e = *iter;
          }
          //Found Edge (There must be one)
          if((*iter)->isVent && (*iter)->weight != (*iter)->maxWeight){
            //cout << "found " << cursorPred << ", " << e->node << endl;
            allMaxed = false;
            (*iter)->weight = (*iter)->maxWeight;
          }
          cursor = cursorPred;
        }
      }


      vector<int> distanceImp(n);
      vector<list<Node>> predsImp(n);
      vector<int> distanceStud(n);
      predsStud.clear(); 

      ///MST FROM IMPOSTOR
      for(Node u: V)
        distanceImp[u] = INF;
      distanceImp[i] = 0;
      q.push(Li(0, i));
      //Dijkstra 
      while(!q.empty()){
        Li li = q.top(); q.pop();
        Node u = li.second;
        for(Edge *e: adj[u]){
          int altdistance = e->weight+distanceImp[u];
          if(distanceImp[e->node] > altdistance){
            //Rimuovo vecchi lati
            predsImp[e->node].clear();
            predsImp[e->node].push_front(u);
            distanceImp[e->node] = altdistance;
            q.push(Li(distanceImp[e->node], e->node));
          }else if (distanceImp[e->node] == altdistance){
            //Aggiungo nuovo lato
            predsImp[e->node].push_front(u);
          }
        }
      }

      ///MST FROM STUDENT
      for(Node u: V)
        distanceStud[u] = INF;
      distanceStud[s] = 0;
      q.push(Li(0, s));
      //Dijkstra 
      while(!q.empty()){
        Li li = q.top(); q.pop();
        Node u = li.second;
        for(Edge *e: adj[u]){
          int altdistance = e->weight+distanceStud[u];
          if(distanceStud[e->node] > altdistance){
            //Rimuovo vecchi lati
            predsStud[e->node].clear();
            predsStud[e->node].push_front(u);
            distanceStud[e->node] = altdistance;
            q.push(Li(distanceStud[e->node], e->node));
          }else if (distanceStud[e->node] == altdistance){
            //Aggiungo nuovo lato
            predsStud[e->node].push_front(u);
          }
        }
      }

      
      if(firstRun){
        bestRes = getRes(distanceImp[f], distanceStud[f]);
        bestDistanceImp = distanceImp;
        bestDistanceStud = distanceStud;
        for(Edge *e : vents)
          bestVentsWeights.push_front(e->weight);
        bestPredsImp = predsImp;
        //cout << "first run!" << endl;
        firstRun=false;
      }
      else{
        int currRes = getRes(distanceImp[f], distanceStud[f]);
        if(isBetter(currRes, bestRes)){
          //change bestRes
          bestRes = currRes;
          bestDistanceImp = distanceImp;
          bestDistanceStud = distanceStud;
          bestVentsWeights.clear();
          for(Edge *e : vents)
            bestVentsWeights.push_front(e->weight);
          bestPredsImp = predsImp;
        }
      }

    };
    //Best settings FOUND


    // First output line
    out << bestRes << endl;

    //Second output line
    out << bestDistanceImp[f] << ' ' << bestDistanceStud[f] << endl;

    //Third output line
    for(int weight : bestVentsWeights){
      out << weight << ' ';
    }
    out << endl;
    
    //Fourth output line
    int R=1;
    Node cursor = f;
    while(cursor != i){
      R++;
      cursor = *bestPredsImp[cursor].begin();
    }
    out << R << endl;

    //Fifth output line
    cursor = f;
    list<Node> pathTofab;
    while(cursor != i){
      pathTofab.push_front(cursor);
      cursor = *bestPredsImp[cursor].begin();
    }
    pathTofab.push_front(i);

    for(Node n: pathTofab)
      out << n << ' ';
    out << endl;

    return;
  }

};

int main(int argc, char *argv[]){
  ifstream in("input.txt");

  Graph::Node i, s, f;
  int n, m, k;
  in >> n >> m >> k;
  in >> i >> s >> f;
  
  Graph g(n, Graph::DIRECTED, Graph::WEIGHTED);

  g.i = i;
  g.s = s;
  g.f = f;

  for(int i=0; i < m; i++){
    Graph::Node u, v;
    int weight;
    in >> u >> v >> weight;
    g.insertEdge(u, v, weight);
  }

  for(int i=0; i < k; i++){
    Graph::Node u, v;
    int minWeight, maxWeight;
    in >> u >> v >> minWeight >> maxWeight;
    g.insertEdge(u, v, minWeight, maxWeight);
  }

  g.dijkstra1();

  in.close();
  out.close();
  return 0;
}
