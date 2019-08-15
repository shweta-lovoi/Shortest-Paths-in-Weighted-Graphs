/*
    bellford.h
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
#include <queue.h>
#include <pair.h>

template < class G>
class BellmanFord
{
  public:
    typedef G                                                     Graph;
    typedef typename G::Vertex                                    Vertex;
    typedef typename fsu::Edge<Vertex>                            Edge;
    typedef typename fsu::Pair<Vertex, Vertex>                    Pair;
    typedef fsu::Queue<Vertex>                                    Queue;
    typedef typename fsu::GreaterThan<Vertex>                     Predicate;

    // constructor initializes all class variables in init list
    BellmanFord (const G& g): g_(g), d_(), p_(){}
    // implementing the algorithm
    void Init(Vertex source); // preps variables for startup of algorithm
    void Exec();              // executes algorithm
    // extracting information
    const fsu::Vector<double>& Distance() const {return d_;}
    const fsu::Vector<size_t>& Parent() const {return p_;}
    void                       Path(Vertex x, fsu::List<Vertex>& path) const;

  private: // data and functions
    const Graph&        g_; // directed weighted graph
    Queue               q_;
    fsu::Vector<bool>   onQ_;
    Predicate           pred_;
    Vertex              s_; 
    fsu::Vector<double> d_; //(calculated by Exec())
    fsu::Vector<size_t> p_; //(calculated by Exec())
    void Relax(Vertex v);
  
};

template < class G >
void BellmanFord<G>::Init(Vertex v)
{
  // set size and initial values for distance vector d_ // INFINITY
  d_.SetSize(g_.VrtxSize(), INFINITY);
  // set size and initial values for parent vector p_   // null vertex (one more than last vertex index)
  p_.SetSize(g_.VrtxSize(), -1);
  // set size and initial values for vector onQ_        // bool false
  onQ_.SetSize(g_.VrtxSize(), false);
  s_ = v;
  d_[s_] = 0.0;
  q_.Push(s_);
  onQ_[s_] = true;
  p_[s_] = g_.VrtxSize();
}  

template < class G >
void BellmanFord<G>::Exec()
{
  //go until p has elements in it
  Vertex v;
  while (!q_.Empty())
  {
    //pop front
    v = q_.Front();
    q_.Pop();
    //visited = false
    onQ_[v] = false;
    //relax
    Relax(v);
  }  
} 

template < class G >
void BellmanFord<G>::Path(Vertex x, fsu::List<Vertex>& path) const
{
  //recursively calculate the path
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

template < class G >
void BellmanFord<G>::Relax(Vertex v)
{
  typename G::AdjIterator i = g_.Begin(v);
  for (; i != g_.End(v); ++i)
  {
    if (d_[v] + g_.GetWeight(v,*i) < d_[*i])
    {
      d_[*i] = d_[v] + g_.GetWeight(v, *i);
      p_[*i] = v;
      if (!onQ_[*i]) // prevents redundant inserts
      {
        q_.Push(*i);     // consider y for relaxation in next pass
        onQ_[*i] = true; // record y is on the queue
      }
    }
  }
} 
