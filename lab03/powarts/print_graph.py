import matplotlib.pyplot as plt
import networkx as nx
import os

num_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/lab03/powarts/.graphs/num", "r")
num = int(num_file.readline())
num_file.close()


input_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/lab03/powarts/input.txt")
output_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/lab03/powarts/output.txt")
plt_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/lab03/powarts/plt.txt")

def colors(name):
  if name == "BLUE":
    return "#b0e0e6"
  elif name == "GREEN":
    return "#98fb98"
  elif name == "RED":
    return "#f08080"

n, m, r = map(lambda x:int(x), input_file.readline().split())

G = nx.Graph()
G.add_nodes_from(range(0, n))

for _ in range(0, m):
  u, v, w = map(lambda x:int(x), input_file.readline().split())
  G.add_edge(u, v)

attacked = list(map(lambda x: int(x),output_file.readline().split()))
node_colors = ["GREEN" if n==r else( "RED" if (n in attacked) else "BLUE" ) for n in list(G)]

G1 = nx.DiGraph()
G1.add_nodes_from(range(0, n))
u, v = [int(n) for n in plt_file.readline().split()]
while u != v:
  G1.add_edge(u, v)
  u, v = [int(n) for n in plt_file.readline().split()]

a_pos = nx.kamada_kawai_layout(G)
nx.draw(G, pos=a_pos, with_labels=True, node_color=list(map(colors, node_colors)), arrows=True)
nx.draw_networkx_edges(G1, pos=a_pos, edge_color=colors("GREEN"), width=2)
#Per grafi orientati

plt.savefig(".graphs/randgraph"+str(num))
os.system("code .graphs/randgraph"+str(num)+".png")
plt.close()
input_file.close()
output_file.close()
plt_file.close()

for u in nx.topological_sort(G1):
  print(u, end=" ")
print()

print("===================")