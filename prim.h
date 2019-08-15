/*
    prim.h
    Shweta Tiwari LoVoi
    7/11/2019
    COP4531
    Project 4
*/
#include <edge.h>
#include <pq.h>
#include <list.h>
#define DUMP_SIZE_THRESHOLD 64

  template < class G >
  class Prim
  {
    public:
      typedef G                                                     Graph;
      typedef typename G::Vertex                                    Vertex;
      typedef typename fsu::Edge<Vertex>                            Edge;
      typedef typename fsu::Vector<Edge>                            Container;
      typedef typename fsu::GreaterThan<Edge>                       Predicate;
      typedef fsu::PriorityQueue<Edge,Container,Predicate>          PQ;
      void              Init   ( bool verbose = 0 );
      void              Exec   ( bool verbose = 0 );
      const fsu::List<Edge>& MST    () const { return mst_; }
      double            Weight () const { return mstw_; }
      Prim                  ( const G& g ) : g_(g),mst_(), mstw_(0.0),c_(0),pred_(),pq_(c_,pred_), inTree_(0) {}

    private:
      const G&        g_;    // undirected weighted graph
      fsu::List<Edge> mst_;  // edges of MST  (calculated by Exec())
      double          mstw_; // weight of MST (calculated by Exec())
      Container       c_;    // "input" edge set
      Predicate       pred_; // edge prioritizer
      PQ              pq_;   // priority queue package operating on c_ with pred_
      fsu::Vector<bool> inTree_; // tree vertices
      
  };


  template < class G >
  void Prim<G>::Init(bool verbose)
  {
    //Clear mst_ and c_ and set mstw_ to zero
    mst_.Clear();
    c_.Clear();
    mstw_ = 0;
    //Initialize inTree_ to size with all values false
    inTree_.SetSize(g_.VrtxSize(), false);
    //Add all edges from the start vertex (0) to c_
    typename G::Vertex v = 0;
    typename G::AdjIterator i = g_.Begin(v);
    for(; i != g_.End(v); i++)
      c_.PushBack(g_.GetEdge(v, *i));
    //Init pq_
    pq_.Init();
    //Set inTree_[0] to true
    inTree_[0] = true;
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
  void Prim<G>::Exec(bool verbose)
  { 
    Edge edge;
    Vertex v;
    size_t size = g_.VrtxSize();
    size_t mstSize = mst_.Size();
    //REPEAT there are no more edges to consider
    while(!pq_.Empty())
    { 
      //Repeat until the tree has n-1 edges
      if (mstSize == (size - 1)) 
        break;
      edge = pq_.Front();
      pq_.Pop();
      //Pop edges from the priority queue until one 
      //has a vertex (say v) outside the tree        
      if((inTree_[edge.x_] && inTree_[edge.y_]) )
        continue;
      if(inTree_[edge.x_] == 1 && inTree_[edge.y_] != 1)
        v = edge.y_;
      else if(inTree_[edge.y_] == 1 && inTree_[edge.x_] != 1)
        v = edge.y_;
      if(!inTree_[v])
      {
        //add edge
        mst_.PushBack(edge);
        mstw_ = mstw_ + edge.w_;
        //update inTree
        inTree_[v] = true;
        mstSize++;
      }
      //push e into priority queue
      typename G::AdjIterator i = g_.Begin(v);
      for(; i != g_.End(v); i++)
      {
        if(!inTree_[v] || !inTree_[*i])
          pq_.Push(g_.GetEdge(v, *i));
      }
    }


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
