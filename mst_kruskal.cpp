/*
     mst.cpp

        calculating minimum spanning tree

*/

#include <wgraph.h>
#include <wgraph_util.h>
#include <kruskal.h>
//#include <prim.h>
#include <edge.h>
#include <cmath>
#include <timer.h>
#include <timer.cpp>

typedef   size_t                     VertexType;
typedef   fsu::ALUWGraph<VertexType> GraphType;
typedef   fsu::Edge<VertexType>      EdgeType;
typedef   fsu::List<EdgeType>        ListType;
typedef   Kruskal<GraphType>         AlgoType; const char* algoname = "Kruskal";
// typedef   Prim<GraphType>            AlgoType; const char* algoname = "Prim";

int main(int argc,char* argv[])
{
  if (argc < 2)
  {
    std::cout << " ** command line arguments expected:\n"
              << "    1: char* 'weighted_graph_filename' (required)\n"
              << "    2: bool  'show_mst_edges' (optional, default = true)\n"
              << "    3: bool  'show_PQ_states' (optional, default = false)\n";
    return EXIT_FAILURE;
  }
  GraphType             g;
  AlgoType              a(g);
  if (!WLoad (argv[1], g))
  {
    std::cout << " ** unable to open file \'" << argv[1] << "\'\n";
    return EXIT_FAILURE;
  }
  bool showMST = 1;
  if (argc > 2)
  {
    showMST = (bool)atoi(argv[2]);
  }
  bool verbose = 0;
  if (argc > 3)
  {
    verbose = (bool)atoi(argv[3]);
  }


  std::cout << ' ' << algoname << " operating on \'" << argv[1] << "\' with showMST = " << (size_t)showMST << '\n';
  fsu::Timer timer;
  fsu::Instant I1,I2;
  timer.EventReset();
  a.Init(verbose);
  a.Exec(verbose);
  I1 = timer.SplitTime();

  size_t edgeCount = 0;
  if (showMST)
  {
    int width = 2 + floor(log10(g.VrtxSize() - 1));
    std::cout << "  MST Edges:\n";
    for (typename ListType::ConstIterator i = a.MST().Begin(); i != a.MST().End(); ++i)
    {
      std::cout << std::setw(3 + width) << (*i).x_
                << std::setw(width)     << (*i).y_
                << ' '
                << (*i).w_
                << '\n';
      ++edgeCount;
    }
  }
  else
  {
    edgeCount = a.MST().Size();
  }
  std::cout << "  MST Weight = " << a.Weight() << '\n';
  std::cout << "  Edge count = " << edgeCount << '\n';
  I2 = timer.EventTime();
  std::cout << " End " << algoname << '\n';
  std::cout << " Runtimes:\n"
            << "  Algorithm: ";
  I1.Write_seconds(std::cout,2);
  std::cout << '\n';
  std::cout << "    += MST : ";
  I2.Write_seconds(std::cout,2);
  std::cout << '\n';
}
