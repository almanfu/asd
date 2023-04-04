import matplotlib.pyplot as plt
import networkx as nx
import random
import os
"""
import colorsys
h,s,l = random.random(), 0.5 + random.random()/2.0, 0.4 + random.random()/5.0
r,g,b = [int(256*i) for i in colorsys.hls_to_rgb(h,l,s)]
"""
SPATH="lab06/lich/"

def colors(name):
  if name == "BLUE":
    return "#b0e0e6"
  elif name == "GREEN":
    return "#98fb98"
  elif name == "RED":
    return "#f08080"
  elif name == "GRAY":
    return "#CCCCCC"
  elif name == "BLACK":
    return "#000000"
  elif name == "RAND":
    return rand_col()
  elif str.isdigit(name):
    return rand_col(int(name))
  elif name == "-1":
    return rand_col(-1)

def rand_col(seed=random.randint(1, 50)):
  return '#{:02x}{:02x}{:02x}'.format(seed%6*40, seed%10*20, seed%4*60)

num_file = open("/home/manfucci/GitRepos/asd/"+SPATH+".graphs/num", "r")
num = int(num_file.readline())
num_file.close()

input_file = open("/home/manfucci/GitRepos/asd/"+SPATH+"input.txt")
plt_file = open("/home/manfucci/GitRepos/asd/"+SPATH+"plt.txt")

edge_labels = {}
node_labels = {}

# use input_file
[n] = [int(x) for x in input_file.readline().split()]
m = n-1

G = nx.Graph()
G.add_nodes_from(range(0, n))

for _ in range(0, m):
  u, v, w = [int(x) for x in input_file.readline().split()]
  G.add_edge(u, v)
  edge_labels[u, v] = str(w)

edge_color_name = ["GRAY" for _ in G.edges()]
node_color_name = ["BLUE" for _ in range(n)]

# use plt_file
for u in range(n):
  [type, u2i, maxleaf] = [str(x) for x in plt_file.readline().split()]
  node_color_name[u] = "BLUE" if type=="nlf" else ("GREEN" if type=="lf" else "RED")
  node_labels[u] = f"{u}({u2i}°:{maxleaf})"

# plotting
a_pos = nx.kamada_kawai_layout(G)

plt.figure(figsize=(10, 4), dpi=700)

nx.draw(G, pos=a_pos,
  node_color=[colors(x) for x in node_color_name],
  edge_color=[colors(x) for x in edge_color_name],
  font_size=10,#1
  arrows=True,
  width=0.5,
  arrowsize=5,#1
  node_size=200)#10

nx.draw_networkx_edge_labels(G, pos=a_pos, 
  edge_labels=edge_labels,
  font_size=10)#1

nx.draw_networkx_labels(G, pos=a_pos,
  labels=node_labels,
  font_size=10)#1

plt.savefig(".graphs/randgraph"+str(num))
os.system("code .graphs/randgraph"+str(num)+".png")
plt.close()

#nx.strongly_connected_components(G)
#nx.all_topological_sorts(G)

input_file.close()
print("===================")