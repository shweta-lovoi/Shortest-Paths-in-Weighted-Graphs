/*
    fwgraph.cpp
    10/07/18

    Tests weighted graphs from wgraph file
    Includes topological sort driver and OutDegreeFrequencyDistribution

    File format:

      Lines at the top of the file begining with '#' are file documentation.
      Once those lines are passed, the remainder of the file is data.

      The data in the file consists of unsigned integers. 
      The first number (after documentation) is the vertex count.
      Then there are numbers in triples representing "from to weight" edges.
      The number of edges is limited only by the graph structures possible.
      The numbers (which represent vertices) must be in the range [0..VertexCount).

      For example:
        # 
        # yada dada
        # 
        3 
        1 2 2.0 
        1 0 3.0
        2 0 4.5
        0 1 1.2 0 2 1.4
      represents a graph with 3 vertices (enumerated 0, 1, 2) and weighted edges
      [1 2 2.0] [1 0 3.0] [2 0 4.5] [0 1 1.2] [0 2 1.4].

      The format of the file is irrelevant to the graph properties,
      but may be more readable if the edges from a given vertex are organized together.

    Copyright 2018, R.C. Lacher
*/

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ansicodes.h>
#include <wgraph.h>
#include <wgraph_util.h> // ShowAL, FileSpec, OutDegreeFrequencyDistribution
#include <topsort.h>
#include <queue.h>

// choose the adjacency list representations
typedef size_t Vertex;
typedef fsu::ALUWGraph<Vertex>  Ungraph;
typedef fsu::ALDWGraph<Vertex>  Digraph;
// */

/* // choose the adjacency matrix representations
typedef size_t Vertex;
typedef fsu::IAMUGraph        Ungraph;
typedef fsu::IAMDGraph        Digraph;
// */

int main( int argc , char* argv[] )
{
  // std::cout << ANSI_BOLD_BLUE;
  std::cout << "\n  Welcome to fgraph - a graph functionality test\n";
  // std::cout << ANSI_RESET_ALL;
  char* inFile(0);
  bool verbose = 0;

  if (argc < 2)
  {
    std::cout << ANSI_BOLD_RED
              << "  Arguments expected:\n"
              << "                      1: input filename (required)\n"
              << "                      2: verbose mode   (optional)\n";
    FileSpec(std::cout);
    std::cout << ANSI_RESET_ALL;
    exit (EXIT_FAILURE);
  }
  else if (argc == 2)
  {
    std::cout << "  Input file: " << argv[1] << '\n'
	      << "  Verbose output disabled\n";
    inFile = argv[1];
  }
  else
  {
    std::cout << "  Input file: " << argv[1] << '\n'
	      << "  Verbose output enabled\n";
    inFile = argv[1];
    verbose = 1;
  }

  Ungraph unGraph;
  Digraph diGraph;

  std::ifstream inStream;
  std::ofstream logStream;
  Vertex x, y; // general purpose vertex variables
  double w;    // weight
  char ch;     // general purpose file read variable

  std::cout << "Loading from file " << inFile << "..." << std::flush;
  inStream.open(inFile);
  if (inStream.fail())
  {
    std::cout << "Cannot open file " << inFile << ". Please try again.\n";
    FileSpec(std::cout);
    exit (EXIT_FAILURE);
  }
 
  // skip documentation -- '#' in column 1
  ch = inStream.peek();
  while (ch == '#')
  {
    ch = inStream.get();
    while ((ch != '\n') && (!inStream.eof()))
      ch = inStream.get();
    ch = inStream.peek();
  }

  inStream >> x;
  unGraph.SetVrtxSize(x);
  diGraph.SetVrtxSize(x);
  while (inStream >> x >> y >> w)
  {
    // if (verbose) std::cout << " adding undirected edge [" << x << ',' << y << ',' << w << "]\n";
    unGraph.AddEdge(x,y,w);
    // if (verbose) std::cout << " adding   directed edge (" << x << ',' << y << ',' << w << ")\n";
    diGraph.AddEdge(x,y,w);
  }
  inStream.close();
  std::cout << "\nLoad complete.\n" << std::flush;

  // ungraph checks
  std::cout << "unGraph.VrtxSize(): " << unGraph.VrtxSize() << '\n';
  std::cout << "unGraph.EdgeSize(): " << unGraph.EdgeSize() << '\n';
  size_t degSum = 0;
  for (size_t i = 0; i < unGraph.VrtxSize(); ++i)
    degSum += unGraph.InDegree(i);
  std::cout << "unGraph Degree sum: " << degSum << '\n';

  if (verbose) 
  {
    std::cout << "unGraph.Dump():\n";
    unGraph.Dump(std::cout);
    for (size_t i = 0; i < unGraph.VrtxSize(); ++i)
    {
      std::cout << "unGraph.InDegree(" << i << ")  == " << unGraph.InDegree(i) << '\n'
                << "unGraph.OutDegree(" << i << ") == " << unGraph.OutDegree(i) << '\n';
    }
  }
  std::cout << "OutDegreeFrequencyDistribution(unGraph):\n";
  OutDegreeFrequencyDistribution(unGraph);

  // digraph checks
  std::cout << "diGraph.VrtxSize(): " << diGraph.VrtxSize() << '\n';
  std::cout << "diGraph.EdgeSize(): " << diGraph.EdgeSize() << '\n';
  size_t indegSum = 0, outdegSum = 0;
  for (size_t i = 0; i < diGraph.VrtxSize(); ++i)
  {
    indegSum += diGraph.InDegree(i);
    outdegSum += diGraph.OutDegree(i);
  }
  std::cout << "diGraph InDegree sum: " << indegSum << '\n';
  std::cout << "       OutDegree sum: " << outdegSum << '\n';

  if (verbose) 
  {  
    std::cout << "diGraph.Dump():\n";
    diGraph.Dump(std::cout);
    for (size_t i = 0; i < diGraph.VrtxSize(); ++i)
    {
      std::cout << "diGraph.InDegree(" << i << ")  == " << diGraph.InDegree(i) << '\n'
                << "diGraph.OutDegree(" << i << ") == " << diGraph.OutDegree(i) << '\n';
    }
  }
  std::cout << "OutDegreeFrequencyDistribution(diGraph):\n";
  OutDegreeFrequencyDistribution(diGraph);

  // reverse digraph checks
  Digraph reverse;
  diGraph.Reverse(reverse);

  std::cout << "reverse.VrtxSize(): " << reverse.VrtxSize() << '\n';
  std::cout << "reverse.EdgeSize(): " << reverse.EdgeSize() << '\n';
  indegSum = 0, outdegSum = 0;
  for (size_t i = 0; i < reverse.VrtxSize(); ++i)
  {
    indegSum += reverse.InDegree(i);
    outdegSum += reverse.OutDegree(i);
  }
  std::cout << "reverse InDegree sum: " << indegSum << '\n';
  std::cout << "       OutDegree sum: " << outdegSum << '\n';

  if (verbose) 
  {  
    std::cout << "reverse.Dump():\n";
    reverse.Dump(std::cout);
    for (size_t i = 0; i < reverse.VrtxSize(); ++i)
    {
      std::cout << "reverse.InDegree(" << i << ")  == " << reverse.InDegree(i) << '\n'
                << "reverse.OutDegree(" << i << ") == " << reverse.OutDegree(i) << '\n';
    }
  }
  std::cout << "OutDegreeFrequencyDistribution(reverse):\n";
  OutDegreeFrequencyDistribution(reverse);

  // top sorts
  std::cout << "\nTopological sort tests:\n";
  fsu::Queue<int> result;
  bool success =  fsu::TopSort(diGraph, result);
  if (success)
  {
    std::cout << " Top Sort (diGraph): " ;
  }
  else
  {
    std::cout << " No Top Sort: diGraph has cycle\n";
    std::cout << " Partial Top Sort: " ;
  }
  result.Display(std::cout, ' ');
  std::cout << '\n';
  result.Clear();
  success =  fsu::TopSort(reverse, result);
  if (success)
  {
    std::cout << " Top Sort (reverse): " ;
  }
  else
  {
    std::cout << " No Top Sort: reverse has cycle\n";
    std::cout << " Partial Top Sort: " ;
  }
  result.Display(std::cout, ' ');
  std::cout << '\n';
  // top sorts */

  std::cout << " unGraph.Check():\n";
  unGraph.Check(std::cout);
  std::cout << " diGraph.Check():\n";
  diGraph.Check(std::cout);
  return EXIT_SUCCESS;
}
