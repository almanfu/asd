# Done
- dfs transitive closure
- iterative dfs
- (cannot do layout on only the edges -- I need forces to act on all pairs)(perhaps this heuristic works better on dense graphs)
- randomized position in the circle
- cooled maximum magnitude
- faster transitive closure O(n+m1)
- for big ns, use local search
- better tabu
- SA
- cluster merging
- FORCE d&c as a starting point

# Doing


# To do
- priority queues (maxdege ~ 200 lgn ~ 12 ... posso anche non farle)
- how to tackle problem 9,10? Cluster splitting with minimum cut


##
k ~ 303
R ~ 200 with custom parameters

=> we are bounded by n^2 needed to 1.calculate the displacements (must consider all pair-interactions) 2.build the auxiliary graph (must consider all possible edges)


Parameters suggested by the paper:
  //
  const int R=180;
  const double rho = 200;;
  const double fatt=1.245;
  const double frep=1.687;
  const double M0 = 633;
  const double r = 0.99;
  //
  const double delta_init=0;
  const double delta_max=200;
  const double sigma_init=0.01;
  const double alpha=1.1+0.0000001*n;
  
k ~ 50
R ~ 80 with original parameters


It works very well when there are few edge additions to make
It doesn't work that well when there are a lot of additions to make

Force is not fast enough. But it gets the job done for 4 test cases.
For the other test cases => local search
