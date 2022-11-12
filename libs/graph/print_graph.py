import matplotlib.pyplot as plt
import networkx as nx
import os

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

num_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/"+spath+".graphs/num", "r")
num = int(num_file.readline())
num_file.close()

input_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/"+spath+"input.txt")
plt_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/"+spath+"powarts/plt.txt")

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
nx.draw(G, pos=a_pos, with_labels=True, node_color=[colors(x) for x in node_color_name], arrows=True, width=1, edge_color=colors("GRAY"))

plt.savefig(".graphs/randgraph"+str(num))
os.system("code .graphs/randgraph"+str(num)+".png")
plt.close()

for u in nx.topological_sort(G1):
  print(u, end=" ")
print()


input_file.close()
plt_file.close()
print("===================")