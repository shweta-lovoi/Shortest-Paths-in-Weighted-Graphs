/*
    dijkstra.h
    Shweta Tiwari LoVoi
    7/11/2019
    COP4531
    Project 4
*/
#include <vector.h>
#include <list.h>
#include <cmath>
#include <edge.h>
#include <pq.h>
#include <pair.h>
#include <queue.h>

template < class G>
class Dijkstra
{
  public:
    typedef G                                                     Graph;
    typedef typename G::Vertex                                    Vertex;
    typedef typename fsu::Edge<Vertex>                            Edge;
    typedef typename fsu::Pair<double, size_t>                    Pair;
    typedef typename fsu::Vector<Pair>                            PairVect;
    typedef typename fsu::GreaterThan<Pair>                       Predicate;
    typedef fsu::PriorityQueue<Pair,PairVect, Predicate>          PQ;

    // constructor initializes all class variables in init list
  Dijkstra (const G& g): g_(g), d_(), p_(), pq_(pv_, pred_){}
    // implementing the algorithm
    void Init(Vertex source); // preps variables for startup of algorithm
    void Exec();              // executes algorithm
    // extracting information
    const fsu::Vector<double>& Distance() const {return d_;}
    const fsu::Vector<size_t>& Parent() const {return p_;}
    void                       Path(Vertex x, fsu::List<Vertex>& path) const;

  private: // data and functions
    const Graph&        g_; // directed weighted graph
    fsu::Vector<double> d_; //(calculated by Exec()
    fsu::Vector<size_t> p_; //(calculated by Exec())
    PairVect            pv_;
    Predicate           pred_;
    PQ                  pq_;
    Vertex              s_;
    fsu::Vector<bool>   finished;
    void Relax(Vertex v);
    bool neg;
  
};

template < class G >
void Dijkstra<G>::Init(Vertex v)
{
  //it will set up the distance to source 0 and
  //its neighbor to their weights
  d_.SetSize(g_.VrtxSize(), INFINITY);
  d_[v] = 0.0;
  p_.SetSize(g_.VrtxSize(), -1);
  finished.SetSize(g_.VrtxSize(), false);
  pv_.PushBack(Pair(d_[v], v));
  s_ = v;
  p_[s_] = g_.VrtxSize();
  neg = false;
}  

template < class G >
void Dijkstra<G>::Exec()
{
  //while pq_ is not empty
  while(!pq_.Empty())
  {
    //if negative value break
    if(neg)
      break;
    //pop the front
    Pair p = pq_.Front();
    Vertex x = p.second_;
    pq_.Pop();
    //if visited continue
    if(finished[x])
      continue;
    //relax on x
    Relax(x);
    //set the finished to true
    finished[x] = true;
  }
} 

template < class G >
void Dijkstra<G>::Path(Vertex x, fsu::List<Vertex>& path) const
{
  if(neg)
    std::cout << " distances invalid - negative weight detected";
  else
  {
    path.Clear();
    path.PushFront(x);
    while(path.Front() != s_)
    {
      if(x == g_.VrtxSize())
        return;
      x = p_[x]; 
      path.PushFront(x);
    }
  }
} 

template < class G >
void Dijkstra<G>::Relax(Vertex v)
{  
  typename G::AdjIterator y = g_.Begin(v);
  for (; y != g_.End(v); ++y)
  {
    //if negative weight found do not count
    if(g_.GetWeight(v,*y) < 0.0)
    {
      std::cout << "Negative weight found\n";
      neg = true;
      break;
    }
    else if (d_[v] + g_.GetWeight(v,*y) < d_[*y])
    {
      d_[*y] = d_[v] + g_.GetWeight(v,*y);
      p_[*y] = v;
      // push y onto the priority queue with the new priority d(y)
      pq_.Push(Pair(d_[*y], *y));
    }
  }
} 
