import matplotlib.pyplot as plt
import networkx as nx
import random
import os
"""
import colorsys
h,s,l = random.random(), 0.5 + random.random()/2.0, 0.4 + random.random()/5.0
r,g,b = [int(256*i) for i in colorsys.hls_to_rgb(h,l,s)]
"""
spath="<where?>"

def colors(name):
  if name == "BLUE":
    return "#b0e0e6"
  elif name == "GREEN":
    return "#98fb98"
  elif name == "RED":
    return "#f08080"
  elif name == "GRAY":
    return "#212121"
  elif name == "RAND":
    return rand_col()
  elif str.isdigit(name):
    return rand_col(int(name))
  elif name == "-1":
    return rand_col(-1)

def rand_col(seed=random.randint(1, 50)):
  return '#{:02x}{:02x}{:02x}'.format(seed%6*40, seed%10*20, seed%4*60)

num_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/"+spath+".graphs/num", "r")
num = int(num_file.readline())
num_file.close()

input_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/"+spath+"input.txt")
plt_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/"+spath+"plt.txt")

n, m = [int(x) for x in input_file.readline().split()]

G = nx.Graph()
G.add_nodes_from(range(0, n))

for _ in range(0, m):
  u, v = [int(x) for x in input_file.readline().split()]
  G.add_edge(u, v)

node_color_name = ["BLUE" for n in list(G)]

G1 = nx.Graph()
G1.add_nodes_from(range(0, n))
u, v = [int(n) for n in plt_file.readline().split()]
while u != v:
  G1.add_edge(u, v)
  u, v = [int(n) for n in plt_file.readline().split()]

a_pos = nx.kamada_kawai_layout(G)
plt.figure(figsize=(5, 5), dpi=300)
nx.draw(G, pos=a_pos, with_labels=True,
  node_color=[colors(x) for x in node_color_name],
  arrows=True,
  width=1,
  edge_color=colors("GRAY"),
  font_size=10,
  width=0.5,
  arrowsize=5,
  node_size=200)

plt.savefig(".graphs/randgraph"+str(num))
os.system("code .graphs/randgraph"+str(num)+".png")
plt.close()

#nx.strongly_connected_components(G)
#nx.all_topological_sorts(G)

input_file.close()
plt_file.close()
print("===================")