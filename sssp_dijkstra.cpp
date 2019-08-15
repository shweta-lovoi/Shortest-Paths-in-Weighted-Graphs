/*
   sssp.cpp

   calculating single source shortest paths

*/

#include <wgraph.h>
#include <wgraph_util.h>
#include <dijkstra.h>
// #include <bellford.h>
// #include <dijkstra_demo.h>
// #include <bellford_demo.h>
#include <cmath>
#include <timer.h>
#include <timer.cpp>

typedef   size_t                        VertexType;
typedef   fsu::ALDWGraph<VertexType>    GraphType;
// typedef   fsu::ALUWGraph<VertexType>    GraphType;
typedef   fsu::List<VertexType>         ListType;
typedef   Dijkstra<GraphType>           AlgoType; const char* algoname = "Dijkstra";
// typedef   BellmanFord<GraphType>        AlgoType; const char* algoname = "BellmanFord";

int main(int argc,char* argv[])
{
  if (argc < 3)
  {
    std::cout << " ** command line arguments expected:\n"
              << "    1: char* 'weighted_digraph_filename' (required)\n"
              << "    2: uint  'start_vertex'              (required)\n"
              << "    3: uint  'limit_output_lines'        (optional)\n";
    return EXIT_FAILURE;
  }
  GraphType   g;
  AlgoType    a(g);
  if (!WLoad (argv[1], g))
  {
    std::cout << " ** unable to open file \'" << argv[1] << "\'\n";
    return EXIT_FAILURE;
  }
  typename GraphType::Vertex s = atoi(argv[2]);
  if (s >= g.VrtxSize())
  {
    std::cout << " ** start vertex \'" << s << "\' out of range [0," << g.VrtxSize() << ")\n";
    return EXIT_FAILURE;
  }
  size_t output_limit = g.VrtxSize();
  if (argc > 3)
  {
    output_limit = atol(argv[3]);
  }

  std::cout << ' ' << algoname << " operating on \'" << argv[1] << "\' from vertex " << s << '\n';
  fsu::Timer timer;
  fsu::Instant I1,I2;
  timer.EventReset();
  a.Init(s);
  a.Exec();
  I1 = timer.SplitTime();

  // set data widths
  int wv = 1;
  if (g.VrtxSize() > 9)
  {
    wv = ceil(log10(g.VrtxSize()));
  }
  int wd = 8;

  // set column spacing - at least 2 more than col header width
  int c1 = 2+wv; if (c1 < 6)  c1 = 6;
  int c2 = 2+wv; if (c2 < 6)  c2 = 6;
  int c3 = 2+wd; if (c3 < 10) c3 = 10;

  std::cout << std::dec << std::fixed << std::showpoint << std::setprecision(2);
  std::cout << std::setw(c1)  << "vrtx"
            << std::setw(c2)  << "pred" 
            << std::setw(c3)  << "distance"
            << std::setw(6)   << "path" << '\n';
  std::cout << std::setw(c1)  << "----"
            << std::setw(c2)  << "----"
            << std::setw(c3)  << "--------"
            << std::setw(6)   << "--->" << '\n';

  fsu::List<VertexType> path;
  typename fsu::List<VertexType>::Iterator i;
  for (VertexType x = 0; x < output_limit; ++x)
  {
    // std::cout << std::setw(c1)  << x << std::setw(c2) << a.Parent()[x] << std::setw(c3) << a.Distance()[x];
    std::cout << std::setw(c1)  << x;
    if (a.Parent()[x] < g.VrtxSize())
      std::cout << std::setw(c2) << a.Parent()[x];
    else
      std::cout << std::setw(c2) << "null";
    std::cout << std::setw(c3) << a.Distance()[x];
    if (a.Distance()[x] < INFINITY)
    {
      a.Path(x,path);
      i = path.Begin();
      if (i != path.End())
      {
        std::cout << "  " << *i;
        ++i;
      }
      for (; i != path.End(); ++i)
      {
        std::cout << std::setw(1+wv) << *i;
      }
    }
    else
    {
      std::cout << "  -";
    }
    std::cout << '\n';
  }
  I2 = timer.EventTime();
  std::cout << " End " << algoname << '\n';
  std::cout << " Runtimes:\n"
            << "  Algorithm: ";
  I1.Write_seconds(std::cout,2);
  std::cout << '\n';
  std::cout << "   += Paths: ";
  I2.Write_seconds(std::cout,2);
  std::cout << '\n';
}

