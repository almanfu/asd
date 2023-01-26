class Node:
  def __init__(self, val):
    self.value = val
    self.left = None
    self.right = None

def print_visual(t, p=0):
  if t is not None:
    p += 1
    print_visual(t.right, p)
    print(" " * (p * 4) + str(t.value))
    print_visual(t.left, p)

def maxTree(A):
    s = [] # stack
    for i in range(len(A)):
        current_node = Node(A[i])
        while len(s) != 0 and s[-1].value < current_node.value:
            popped_node = s.pop()
            current_node.left = popped_node
        if len(s) != 0:
            s[-1].right = current_node
        s.append(current_node)
    return s[0]

print_visual(maxTree([2,3,-1,8,4,5]))