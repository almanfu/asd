import matplotlib.pyplot as plt
import networkx as nx
import os
import random

SPATH="lab03/batman/"

def colors(name):
  if name == "BLUE":
    return "#b0e0e6"
  elif name == "GREEN":
    return "#98fb98"
  elif name == "RED":
    return "#f08080"
  elif name == "GRAY":
    return "#212121"
  elif str.isnumeric(name):
    return rand_col(int(name))
  elif int(name) == -1:
    return rand_col(-1)
  else:
    return rand_col

def rand_col(seed=random.randint(1, 50)):
  return '#{:02x}{:02x}{:02x}'.format(seed%6*40, seed%10*20, seed%4*60)

num_file = open("/home/manfucci/GitRepos/asd"+SPATH+".graphs/num", "r")
num = int(num_file.readline())
num_file.close()

input_file = open("/home/manfucci/GitRepos/asd"+SPATH+"input.txt")
plt_file = open("/home/manfucci/GitRepos/asd"+SPATH+"plt.txt")

n, m, s, d = [int(x) for x in input_file.readline().split()]

G = nx.DiGraph()
G.add_nodes_from(range(0, n))

for _ in range(0, m):
  u, v = [int(x) for x in input_file.readline().split()]
  G.add_edge(u, v)

ccs = {}
for line in plt_file.readlines():
  u, color = [int(x) for x in line.split()]
  ccs[u] = color

node_color_name = ["GREEN" if ccs[u] == ccs[s] else ("RED" if ccs[u] == ccs[d] else str(ccs[u])) for u in list(G)]


a_pos = nx.kamada_kawai_layout(G)
plt.figure(figsize=(5, 5), dpi=300)
nx.draw(G, pos=a_pos, with_labels=True, font_size=10, node_color=[colors(x) for x in node_color_name], arrows=True, width=0.5, arrowsize=5, node_size=200, edge_color=colors("GRAY"))

plt.savefig(".graphs/randgraph"+str(num))
os.system("code .graphs/randgraph"+str(num)+".png")
plt.close()
sccs = nx.strongly_connected_components(G)
for scc in sccs:
  print(scc)

input_file.close()
plt_file.close()
print("===================")