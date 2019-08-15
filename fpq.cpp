/*
    fpq.cpp
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <xstring.h>
#include <vector.h>
#include <pq.h>

#include <xstring.cpp>

void DisplayMenu(std::ostream& os = std::cout);

/* // fsu::String
typedef fsu::String ElementType;
ElementType sentinal = ".";
const char* et = "fsu::String";
// */

// unsigned integers
typedef uint32_t  ElementType;
ElementType sentinal = 0;
const char* et = "uint32_t";
// */

// Max PQ
typedef fsu::LessThan < ElementType >    PredicateType;
const char* pt = "fsu::LessThan";
// */

/* // Min PQ
typedef fsu::GreaterThan < ElementType >    PredicateType;
const char* pt = "fsu::GreaterThan";
// */

typedef fsu::Vector < ElementType >      ContainerType;
const char* ct = "fsu::Vector";

#include <cmath>
template <typename T>
int Width(const T& t)
{
  return 1 + floor(log10(t));
}
template <>
int Width(const fsu::String& s)
{
  return s.Size();
}

int main(int argc, char* argv[])
{
  ElementType s;
  ContainerType v;
  PredicateType p;
  fsu::PriorityQueue<ElementType,ContainerType,PredicateType> pq(v,p);
  bool inited = 1;
  char command;
  size_t size;
  int width = 0, maxwidth = 0;
  fsu::String filename;
  std::ifstream ifs,com;

  bool BATCH = 0;
  std::istream * comptr = &std::cin;

  if (argc > 1)
  {
    com.open(argv[1]);
    if (com.fail())
    {
      std::cerr << " ** Unable to open command file \'" << argv[1] << "\'\n"
                << "    Try again\n";
      return EXIT_FAILURE;
    }
    comptr = &com;
    BATCH = 1;
  }

  if (!BATCH) DisplayMenu(std::cout);
  std::cout << " fsu::PriorityQueue < " << et << " , " << ct << " < " << et << " > , " << pt << " < " << et << " > >\n";
  do
  {
    std::cout << " Enter command ('Q' to quite): ";
    *comptr >> command;
    if (BATCH) std::cout << command;
    switch(command)
    {
      case '>':
        *comptr >> command;
        if (BATCH) std::cout << ' ' << command << '\n';
        switch(command)
        {
          case 'f': case 'F': // load from file
            std::cout << "  Enter file name: ";
            *comptr >> filename;
            if (BATCH) std::cout << filename << '\n';
            ifs.open(filename.Cstr());
            if (ifs.fail())
            {
              std::cout << " ** unable to open data file \'" << s << "\'\n";
              ifs.clear();
              break;
            }
            while (ifs >> s)
            {
              v.PushBack(s);
              width = Width(s);
              if (maxwidth < width) maxwidth = width;
            }
            ifs.clear();
            ifs.close();
            inited = 0;
            std::cout << "  Load complete\n";
            break;
          case 'k': case 'K': // load from keyboard
            std::cout << "  Enter elements (\"" << sentinal << "\" to stop):\n";
            *comptr >> s;
            while (s != sentinal)
            {
              if (BATCH) std::cout << ' ' << s;
              v.PushBack(s);
              width = Width(s);
              if (maxwidth < width) maxwidth = width;
              *comptr >> s;
            }
            if (BATCH) std::cout << ' ' << s << '\n';
            inited = 0;
            std::cout << "  Load complete\n";
            break;
          default:
            std::cout << " ** unknown load code\n";
        }
        break;

      case 'i': case 'I':
        if (BATCH) std::cout << '\n';
        pq.Init();
        inited = 1;
        break;
      case '+': case '1':
        *comptr >> s;
        if (BATCH) std::cout << ' ' << s << '\n';
        pq.Push(s);
        width = Width(s);
        if (maxwidth < width) maxwidth = width;
        std::cout << "  pq.Push(" << s << ")\n";
        break;
      case '-': case '2':
        if (BATCH) std::cout << '\n';
        pq.Pop();
        std::cout << "  pq.Pop();\n";
        break;
      case 'f': case 'F':
        if (BATCH) std::cout << '\n';
        s = pq.Front();
        std::cout << "  " << s << " = pq.Front()\n";
        break;
      case 's': case 'S':
        if (BATCH) std::cout << '\n';
        size = pq.Size();
        std::cout << "  " << size << " = pq.Size()\n";
        break;
      case 'd': case 'D':
        if (BATCH) std::cout << '\n';
        std::cout << "  pq.Dump():";
        if (inited)
          std::cout << "   // this should be a max-heap using predicate order\n";
        else
          std::cout << "   // this is the unstructured container contents\n";
        pq.Dump(std::cout, 1 + maxwidth);
        break;
      case 'o': case 'O':
        if (BATCH) std::cout << '\n';
        std::cout << "  pq.PopOut():";
        if (inited)
          std::cout << "  // this should be sorted in reverse predicate order\n";
        else
          std::cout << "  // this order is not specified\n";
        std::cout << ' ';
        pq.PopOut(std::cout, ' ');
        std::cout << '\n';    
        break;
      case 'x': case 'X':
        if (BATCH) std::cout << '\n';
        if (BATCH)
        {
          comptr = &std::cin;
          com.clear();
          com.close();
          BATCH = 0;
          DisplayMenu(std::cout);
          std::cout << " ** switched to keyboard input\n";
        }
        else
        {
          std::cout << " ** already in interactive mode\n";
        }
        break;
      case 'm': case 'M':
        if (BATCH) std::cout << '\n';
        DisplayMenu(std::cout);
        break;
      case 'q': case 'Q':
        if (BATCH) std::cout << '\n';
        command = 'q';
        break;
      default:
        if (BATCH) std::cout << '\n';
        std::cout << "  ** command not found\n";
        do command = comptr->get(); while (command != '\n');
        break;
    }
  }
  while (command != 'q');
}

void DisplayMenu(std::ostream& os)
{
  os
     << "  action                        command\n"
     << "  ------                        -------\n"
     << "  Load (file) ................  > F filename\n"
     << "  Load (keyboard) ............  > K\n"
     << "  Init () ....................  I             // g_build_heap (c_.Begin() , c_.End() , p_)\n"
     << "  Push (t) ...................  + t           // c_.PushBack(t) ; g_pop_heap(c_.Begin() , c_.End() , p_)\n"
     << "  Pop () .....................  -             // g_pop_heap(c_.Begin() ,c_.End() , p_) ; c_.PopBack()\n"
     << "  Front () ...................  F             // c_.Front()\n"
     << "  Size () ....................  S             // c_.Size()\n"
     << "  Dump () ....................  D             // display heap in tree format\n"
     << "  PopOut () ..................  O             // output Top & Pop until empty\n"
     << "  display menu  ..............  M\n"
     << "  switch to keyboard input ...  X\n"
     << '\n';
}
