/*
   edge.h

   Edge class for use in weighted graph algorithms

*/

#ifndef _EDGE_H
#define _EDGE_H

namespace fsu
{

  template < typename N >
  class Edge
  {
  public:
    typedef N Vertex;
    Edge (); // default constructor so arrays are possible
    Edge (Vertex x, Vertex y, double w); // preferred initializing constructor
    Vertex x_, y_;
    double w_;
  };

  template < typename N >
  Edge<N>::Edge () : x_(), y_(), w_() {}

  template < typename N >
  Edge<N>::Edge (Vertex x, Vertex y, double w) : x_(x), y_(y), w_(w) {}

  template < typename N >
  bool operator< (const Edge<N>& e1, const Edge<N>& e2)
  {
    return e1.w_ < e2.w_;
  }

  template < typename N >
  std::ostream& operator<< (std::ostream& os, const Edge<N>& e)
  {

    // uint32_t flags = os.flags();
    std::ios::fmtflags flags = os.setf(std::ios::fixed | std::ios::showpoint);
    os.precision(2);
    os << '[' << e.x_ << ',' << e.y_ << ',' << e.w_ << ']';
    os.setf(flags);
    return os;
  } // */

}

#endif
