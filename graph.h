/*
    graph.h
    Chris Lacher
    03/27/17

    Copyright 2017, R.C. Lacher
*/

#ifndef _GRAPH_H
#define _GRAPH_H

#include <iostream>
#include <vector.h>
// #include <ovector.h>
#include <list.h>
// #include <olist.h>
// #include <rbllt_adt.h>
// #include <matrix.h>
// #include <debug.h>

namespace fsu
{

  template < typename N >
  class ALUGraph
  {
  public:
    typedef N                                      Vertex;
    typedef typename fsu::List<Vertex>             SetType;
    typedef typename SetType::ConstIterator        AdjIterator;

    void   SetVrtxSize  (N n);
    size_t VrtxSize     () const;
    void   AddEdge      (Vertex from, Vertex to);
    bool   HasEdge      (Vertex from, Vertex to) const;
    size_t EdgeSize     () const;           // Theta (|V| + |E|)
    size_t OutDegree    (Vertex v) const;
    size_t InDegree     (Vertex v) const;
    void   SortStar     (Vertex v);

    AdjIterator Begin   (Vertex v) const;
    AdjIterator End     (Vertex v) const;

    void Clear          ();
    void Dump           (std::ostream& os); // Theta (|V| + |E|)
    void Shuffle        (); // shuffles all adjacency lists
    
    ALUGraph            ();
    explicit ALUGraph   (N n);

  protected:
    fsu::Vector < SetType > al_;
  };

  template < typename N >
  class ALDGraph : public ALUGraph <N>
  {
  public:
    typedef N                                          Vertex;
    typedef typename ALUGraph<N>::SetType              SetType;
    typedef typename ALUGraph<N>::AdjIterator          AdjIterator;

    // void   SetVrtxSize  (N n);
    // size_t VrtxSize     () const;
    void   AddEdge      (Vertex from, Vertex to);
    // bool   HasEdge      (Vertex from, Vertex to) const;
    size_t EdgeSize     () const;          // Theta (|V| + |E|)
    // size_t OutDegree    (Vertex v) const;
    size_t InDegree     (Vertex v) const;  // Theta (|V| + |E|)
    // AdjIterator Begin   (Vertex v) const;
    // AdjIterator End     (Vertex v) const;

    // void Dump           (std::ostream& os);  // Theta (|V| + |E|)
    // void Shuffle        (); // pseudo-randomizes order in adjacency lists

    ALDGraph ( );
    explicit ALDGraph ( N n );

    // new method - creates d as the reverse directed graph of *this 
    void Reverse(ALDGraph& d) const;
  };

  /////////////////////////
  // ALU implementations //
  /////////////////////////

  template < typename N >
  ALUGraph<N>::ALUGraph () : al_(0){}

  template < typename N >
  ALUGraph<N>::ALUGraph ( N n ) : al_((size_t)n){}

  template < typename N >
  void ALUGraph<N>::SetVrtxSize (N n)
  {
    al_.SetSize((size_t)n);
  }

  template < typename N >
  void ALUGraph<N>::AddEdge (Vertex from, Vertex to)
  {
    if (from == to) return;
    al_[(size_t)from].Insert(to);
    al_[(size_t)to].Insert(from);
  }

  template < typename N >
  bool ALUGraph<N>::HasEdge (Vertex from, Vertex to) const
  {
    AdjIterator i = al_[from].Includes(to);
    if (i == End(from))
      return 0;
    return 1;
  }

  template < typename N >
  size_t ALUGraph<N>::VrtxSize () const
  {
    return al_.Size();
  }

  template < typename N >
  size_t ALUGraph<N>::EdgeSize () const // Theta (|V| + |E|)
  // Theta (|V| + |E|)
  {
    size_t esize = 0;
    for (Vertex v = 0; v < al_.Size(); ++v)
      esize += al_[v].Size();
    return esize >> 1; // divide by 2
  }

  template < typename N >
  size_t ALUGraph<N>::OutDegree (Vertex v) const
  {
    return al_[v].Size();
  }

  template < typename N >
  size_t ALUGraph<N>::InDegree (Vertex v) const
  {
    return OutDegree(v);
  }

  template < typename N >
  typename ALUGraph<N>::AdjIterator ALUGraph<N>::Begin (Vertex v) const
  {
    return al_[v].Begin();
  }

  template < typename N >
  typename ALUGraph<N>::AdjIterator ALUGraph<N>::End (Vertex v) const
  {
    return al_[v].End();
  }

  template < typename N >
  void ALUGraph<N>::Dump (std::ostream& os)
  // Theta (|V| + |E|)
  {
    AdjIterator i;
    for (Vertex v = 0; v < VrtxSize(); ++v)
    {
      os << '[' << v << "]->";
      i = this->Begin(v);
      if (i != this->End(v))
      {
	os << *i;
	++i;
      }
      for ( ; i != this->End(v); ++i)
      {
	os << ',' << *i;
      }
      os << '\n';
    }
  }

  template < typename N >
  void ALUGraph<N>::Shuffle()
  {
    for (Vertex v = 0; v < VrtxSize(); ++v) al_[v].Shuffle();
  }

  template < typename N >
  void ALUGraph<N>::Clear()
  {
    for (Vertex v = 0; v < VrtxSize(); ++v) al_[v].Clear();
    al_.SetSize(0);
  }

  template < typename N >
  void ALUGraph<N>:: SortStar (Vertex v)
  {
    al_[v].Sort();
  }

  /////////////////////////
  // ALD implementations //
  /////////////////////////

  template < typename N >
  ALDGraph<N>::ALDGraph () : ALUGraph<N> (){}

  template < typename N >
  ALDGraph<N>::ALDGraph ( N n ) : ALUGraph<N>(n){}

  template < typename N >
  void ALDGraph<N>::AddEdge (Vertex from, Vertex to)
  {
    if (from == to) return;
    ALUGraph<N>::al_[(size_t)from].Insert(to);
  }

  template < typename N >
  size_t ALDGraph<N>::EdgeSize () const
  // Theta (|V| + |E|)
  {
    size_t esize = 0;
    for (Vertex v = 0; v < ALUGraph<N>::al_.Size(); ++v)
      esize += ALUGraph<N>::al_[v].Size();
    return esize;
  }

  template < typename N >
  size_t ALDGraph<N>::InDegree (Vertex v) const
  // Theta (|V| + |E|)
  {
    size_t indegree = 0;
    AdjIterator i;
    for (Vertex x = 0; x < ALUGraph<N>::VrtxSize(); ++x)
    {
      for (i = this->Begin(x); i != this->End(x); ++i)
      {
	if (v == *i) ++indegree;
      }
    }
    return indegree;
  }

  template < typename N >
  void ALDGraph<N>::Reverse(ALDGraph& d) const
  {
    d.Clear();
    d.SetVrtxSize(this->VrtxSize());
    for (Vertex v = 0; v < this->VrtxSize(); ++v)
    {
      for (AdjIterator i = this->Begin(v); i != this->End(v); ++i)
      {
        d.AddEdge(*i,v);
      }
    }
    return;
  }

} // namespace fsu
#endif
