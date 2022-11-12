import matplotlib.pyplot as plt
import networkx as nx
import random
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
num = int(num_file.readline())+1
num_file.close()
num_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/"+spath+".graphs/num", "w+")
num_file.write(str(num))
num_file.close()

input_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/"+spath+"input.txt", "w+")

n = random.randint(5, 15)
m = random.randint(int(1.4*n), 2*n)

c='n'
while c !='y':
  plt.close()
  G = nx.gnm_random_graph(n, m)
  nx.draw_kamada_kawai(G, node_color=[colors("GREEN") for n in list(G)])
  plt.savefig(".graphs/randgraph"+str(num))
  os.system("code .graphs/randgraph"+str(num)+".png")
  c = input()
  if c == 'q':
    raise Exception


input_file.write(str(n)+" "+str(m)+"\n")
for (u, v, data) in G.edges(data=True):
  input_file.write(str(u)+" "+str(v)+" "+"\n")


input_file.close()
os.system("cp input.txt .graphs/randgraph"+str(num)+".txt")
print("===================")
