import matplotlib.pyplot as plt
import networkx as nx
import random
import os

def colors(name):
  if name == "BLUE":
    return "#b0e0e6"
  elif name == "GREEN":
    return "#98fb98"
  elif name == "RED":
    return "#f08080"

num_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/lab03/powarts/.graphs/num", "r")
num = int(num_file.readline())+1
num_file.close()
num_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/lab03/powarts/.graphs/num", "w+")
num_file.write(str(num))
num_file.close()

input_file = open("/mnt/50A65EC5A65EAB6C/lab/asd1/lab03/powarts/input.txt", "w+")
n = random.randint(5, 15)
m = random.randint(int(1.4*n), 2*n)
r = random.randint(0, n-1)

c='n'
while c !='y':
  plt.close()
  G = nx.gnm_random_graph(n, m)
  nx.draw_kamada_kawai(G, node_color=[colors("GREEN") if n==r else colors("BLUE") for n in list(G)])
  plt.savefig(".graphs/randgraph"+str(num))
  os.system("code .graphs/randgraph"+str(num)+".png")
  c = input()
  if c == 'q':
    raise Exception


input_file.write(str(n)+" "+str(m)+" "+str(r)+"\n")
for (u, v, data) in G.edges(data=True):
  input_file.write(str(u)+" "+str(v)+" "+str(random.randint(1, 9))+"\n")
input_file.close()
os.system("cp input.txt .graphs/input"+str(num)+".txt")

print("===================")
