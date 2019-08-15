
HOME = /home/courses/cop4531p/LIB
CPP   = $(HOME)/cpp
TCPP  = $(HOME)/tcpp
BST   = $(HOME)/bst
GRAPH = $(HOME)/graph
INCL  = -I. -I$(CPP) -I$(TCPP) -I$(BST) -I$(GRAPH)

CC = g++ -std=c++11 -Wall -Wextra $(INCL)


all:   kruskal.x prim.x dijkstra.x bellford.x

fg.x : wgraph.h fwgraph.cpp
	$(CC) -o fg.x fwgraph.cpp

kruskal.x : wgraph.h kruskal.h mst_kruskal.cpp
	$(CC) -o kruskal.x mst_kruskal.cpp

prim.x : wgraph.h prim.h mst_prim.cpp
	$(CC) -o prim.x mst_prim.cpp

dijkstra.x : wgraph.h dijkstra.h sssp_dijkstra.cpp
	$(CC) -o dijkstra.x sssp_dijkstra.cpp

bellford.x : wgraph.h bellford.h sssp_bellford.cpp
	$(CC) -o bellford.x sssp_bellford.cpp

