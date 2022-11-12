#include <iostream>
#include <fstream>

#include <vector>
#include <list>
#include <queue>
#include <stack>

#include <algorithm>

using namespace std;

const bool LOCAL = true;
const bool OLD_GRAPH = true?LOCAL:false;
ofstream plt;
ofstream out("output.txt");

/*

*/

class Graph{
  public:
  typedef int Node;
  struct Edge{
    Node node;
    int weight;
    Edge(Node n, int w){
      node = n;
      weight = w;
    }
  };
  enum EdgeDirection {DIRECTED, UNDIRECTED};
  enum EdgeWeight {WEIGHTED, UNWEIGHTED};
  EdgeDirection edgeDirection;
  EdgeWeight edgeWeight;
  private:
  vector<Node> V;
  vector<list<Edge>> adj;
  int n;
  public:
  Graph(int nodes, EdgeDirection eDir, EdgeWeight eWeight){
    edgeDirection = eDir;
    edgeWeight = eWeight;
    n = nodes;
    adj = vector<list<Edge>>(n);
    V = vector<Node>(n);
    for(Node u=0; u < n; u++)
      V[u] = u;
  }
  ~Graph(){}
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
      adj[u].push_front(Edge(v, weight));
      if(undirected())
        adj[v].push_front(Edge(u, weight));
    }
  }
  // Utility Functions
  void print_adj(){
    for(Node u=0; u < n; u++){
      cout << u << ':';
      if(unweighted()){
        for(Edge e: adj[u])
          cout << e.node << ' ';
      }else{
        for(Edge e: adj[u])
          cout << '(' << e.node << ',' << e.weight << ')' << ' ';
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
      for(Edge e: adj[u]){
        if(!visited[e.node]){
          q.push(e.node);
          visited[e.node] = true;
        }
      }
    }
    cout << endl;
    delete[] visited;
  }

  void powarts(Node r){
    if(LOCAL)
      plt = ofstream("plt.txt");
    queue<Node> q;
    vector<int> distance(n);

    vector<Node> pred(n);
    vector<list<Node>> preds(n);

    for(Node u=0; u < n; u++){
      distance[u] = -1;
      pred[u] = -1;
    }
    if(has(r)){
      distance[r] = 0;
      q.push(r);
    }

    //Diijstra 
    while(!q.empty()){
      Node u = q.front(); q.pop();
      for(Edge e: adj[u]){
        if(distance[e.node] == -1){
          if(OLD_GRAPH)
            preds[e.node].push_front(u);
          pred[e.node]=u;
          distance[e.node] = e.weight+distance[u];
          q.push(e.node);
        }else if(distance[e.node] > e.weight+distance[u]){
          //Rimuovo vecchi lati
          if(OLD_GRAPH){
            preds[e.node].clear();
            preds[e.node].push_front(u);
          }
          pred[e.node] = u;
          distance[e.node] = e.weight+distance[u];
        }else if (distance[e.node] == e.weight+distance[u]){
          //Aggiungo nuovo lato
          if(OLD_GRAPH)
            preds[e.node].push_front(u);
          if(u != r){//Removing Cross Edges
            list<Node> path_u;
            path_u.push_front(u);
            while(path_u.front() != r)
              path_u.push_front(pred[path_u.front()]);
            list<Node> path_e;
            path_e.push_front(e.node);
            while(path_e.front() != r)
              path_e.push_front(pred[path_e.front()]);
            if(*(++path_u.begin())!=*(++path_e.begin()))
              pred[e.node] = r;
          }else{//Removing possible Cross Edge/ Forward Edge
            pred[e.node]=u;
          }
        }
      }
    }
    //Creo grafo cammini costo minimi
    Graph G(n, Graph::DIRECTED, Graph::UNWEIGHTED);
    for(Node v: G.V)
      G.insertEdge(pred[v], v);

    if(LOCAL){
      if(!OLD_GRAPH){
        for(Node v: G.V){
          for(Edge e: G.adj[v])
            plt << v << ' ' << e.node << '\n';
        }
      }else{
        for(Node v: G.V){
          for(Node u: preds[v])
            plt << u << ' ' << v << '\n';
        }
      }
      plt << "0 0";
    }

    {//NEW
    struct sNode{
      Node child;
      Node parent;
      list<Graph::Edge>::iterator iter; 
      sNode(Node c, Node p, list<Graph::Edge>::iterator i){
        child = c;
        parent = p;
        iter = i;
      }
    };
    stack<sNode*> s;
    s.push(new sNode(r, r, G.adj[r].begin()));
    int *dt = new int[n]{};
    int *ft = new int[n]{};
    int *damage = new int[n]{};
    Node bestAttack=-1;
    int maxDamage=0;
    int time=0;
    while(!s.empty()){
      sNode* snode = s.top(); 
      Node& u = snode->child;
      Node& parent = snode->parent;
      time++;
      if(ft[u] != 0 && u != r){
        damage[u]=1;
        for(Edge e: G.adj[u])
          damage[u] += damage[e.node];
        if(damage[u] > maxDamage){
          bestAttack = u;
          maxDamage = damage[u];
        }
        delete snode;
        s.pop();
      }else if(ft[u] == 0){
        if(dt[u] == 0)
          dt[u] = time;
        auto& iter = snode->iter;
        auto iterEnd = G.adj[u].end();
        while(iter!=iterEnd){
          Edge e = *iter;
          if(dt[e.node] == 0){//Tree Edge
            iter++;
            s.push(new sNode(e.node, u, G.adj[e.node].begin()));
            break;
          }else if(dt[e.node] > dt[u]){//Forward Edge => u != r
            iter = G.adj[u].erase(iter);
          }else{//Cross Edge => There's a common ancestor != r
            iter = G.adj[u].erase(iter);
          }
        }
        if(iter == iterEnd){
          ft[u] = dt[u]==time?time+1:time;
        }
      }else if(ft[u] != 0 && u == r){
        delete snode;
        s.pop();
      }
    }
    delete[] dt;
    delete[] ft;
    delete[] damage;
    
    bool *visited = new bool[n]{};
    stack<Node> s1;
    list<Node> disabled;
    if(has(bestAttack)){
      disabled.push_front(bestAttack);
      s1.push(bestAttack);
    }
    while(!s1.empty()){
      Node u = s1.top(); s1.pop();
      for(Edge e: G.adj[u]){
        if(!visited[e.node]){
          s1.push(e.node);
          disabled.push_front(e.node);
          visited[e.node] = true;
        }
      }
    }
    delete[] visited;
    if(!LOCAL){
      out << maxDamage << '\n';
      for(Node u: disabled)
        out << u << '\n';
      return;
    }else{
      cout << "---NEW---" << '\n';
      cout << "bestAttack=" << bestAttack << '\n';
      cout << "maxDamage=" << maxDamage << '\n';
      for(Node u: disabled)
        cout << u << ' ';
      cout << '\n';      
    }
    }

    {//OLD
    //Rimuovo ogni nodo e vedo la grandezza della cc di r in G 
    int minPowarts = n;
    list<Node> bestAttacks;
    list<Node> disabled;
    
    bool *visited = new bool[n]{};
    for(Node attacked: G.V){
      if(attacked != r && !G.adj[attacked].empty()){
        // Modified BFS
        for(int i=0; i < n; i++)
          visited[i] = false;
        queue<Node> q;
        q.push(r);
        visited[r] = true;
        int numPowarts=1;
        while(!q.empty()){
          Node u = q.front(); q.pop();
          for(Edge e: G.adj[u]){
            if(e.node != attacked && !visited[e.node]){
              q.push(e.node);
              visited[e.node] = true;
              numPowarts++;
            }
          }
        }
        if(numPowarts<minPowarts){
          minPowarts = numPowarts;
          disabled.clear();
          for(Node u: G.V){
            if(visited[u] == false)
              disabled.push_front(u);
          }
          if(LOCAL){
            bestAttacks.clear();
            bestAttacks.push_front(attacked);
          }
        }else if(numPowarts == minPowarts){
          if(LOCAL)
            bestAttacks.push_front(attacked);
        }
      }
    }
    delete[] visited;
    if(LOCAL){
      cout << "---OLD---" << '\n';
      cout << "maxDamage=" << (n-minPowarts) << '\n';
      for(Node u: bestAttacks)
        out << u << ' '; 
      plt.close();
    }
    }
    
  }
};

/*

*/

int main(int argc, char *argv[]){
  ifstream in("./dataset_powarts/powarts/input/input13.txt");

  Graph::Node r;
  int n, m;
  in >> n >> m >> r;
  
  Graph g(n, Graph::UNDIRECTED, Graph::WEIGHTED);

  for(int i=0; i < m; i++){
    Graph::Node u, v;
    int weight;
    in >> u >> v >> weight;
    g.insertEdge(u, v, weight);
  }

  g.powarts(r);

  if(LOCAL)
    cout << "===================" << '\n';
  in.close();
  out.close();
  return 0;
}
