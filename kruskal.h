/*
    kruskal.h
    Shweta Tiwari LoVoi
    7/11/2019
    COP4531
    Project 4
*/
#include <edge.h>
#include <pq.h>
#include <list.h>
#include <tostring.h>
#include <partition2.h>
#include <aa.h>
#define DUMP_SIZE_THRESHOLD 64

  template < class G >
  class Kruskal
  {
    public:
      typedef G                                                     Graph;
      typedef typename G::Vertex                                    Vertex;
      typedef typename fsu::Edge<Vertex>                            Edge;
      typedef typename fsu::Vector<Edge>                            Container;
      typedef typename fsu::GreaterThan<Edge>                       Predicate;
      typedef fsu::PriorityQueue<Edge,Container,Predicate> PQ;

      void              Init   ( bool verbose = 0 );
      void              Exec   ( bool verbose = 0 );
      const fsu::List<Edge>& MST    () const { return mst_; }
      double            Weight () const { return mstw_; }
      Kruskal                  ( const G& g ) : g_(g),mst_(), mstw_(0.0),c_(0),pred_(),pq_(c_,pred_) {}

    private:
      const G&        g_;    // undirected weighted graph
      fsu::List<Edge> mst_;  // edges of MST  (calculated by Exec())
      double          mstw_; // weight of MST (calculated by Exec())
      Container       c_;    // "input" edge set
      Predicate       pred_; // edge prioritizer
      PQ              pq_;   // priority queue package operating on c_ with pred_
  };


  template < class G >
  void Kruskal<G>::Init(bool verbose)
  {
    //Clear mst_ and c_ and set mstw_ to zero
    mst_.Clear();
    c_.Clear();
    mstw_ = 0;
    //add all edges to c_, careful to not add any edge more than once
    typename G::Vertex v = 0;
    for(; v < g_.VrtxSize(); v++)
    {
      typename G::AdjIterator i = g_.Begin(v);
      for(; i != g_.End(v); i++)
        if(v < *i)
          c_.PushBack(g_.GetEdge(v, *i)); 
    }
    //init pq_
    pq_.Init();
    //If verbose, output pq_ info
     if (verbose)
    {
      std::cout << "Init: Initial edges in PQ: " << pq_.Size() << '\n';
      if (pq_.Size() < DUMP_SIZE_THRESHOLD)
      {
        std::cout << "PQ.Dump():\n";
        pq_.Dump(std::cout, 2);
        std::cout << '\n';
      }
    } 
  }

  template < class G >
  void Kruskal<G>::Exec(bool verbose)
  {
    //Use an fsu::Partition object to keep track of forest connectivity, as in
    //the Spanning Tree algorithm.
    size_t size = g_.VrtxSize();
    fsu::Partition<size_t> p (size);
    Edge e;
    size_t mstSize = mst_.Size();
    //for (size_t i = 0; i < n; i++)
    while(!pq_.Empty())
    {
      if (mstSize == (size - 1)) 
        break;
      e = pq_.Front();
      pq_.Pop();
      if (p.Find(e.x_, e.y_) == false)
      {
        //Add the edge to the forest
        //Update the partition object
        //update weight
        p.Union(e.x_, e.y_);
        mstw_ += e.w_;
        mst_.PushBack(e);
        mstSize++;
      }//Until the forest is connected 
    }
    //If verbose, output pq_ info
     if (verbose)
  {
    std::cout << "Exec: Remaining edges in PQ: " << pq_.Size() << '\n';
    if (pq_.Size() < DUMP_SIZE_THRESHOLD)
    {
      std::cout << "PQ.Dump():\n";
      pq_.Dump(std::cout, 2);
      std::cout << '\n';
      std::cout << "PQ.PopOut():";
      pq_.PopOut(std::cout, ' ');
      std::cout << '\n';
    }
  }
  }
