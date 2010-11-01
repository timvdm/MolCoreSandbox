#include "molecule.h"
#include "atom.h"
#include "bond.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/smart_ptr.hpp>

namespace MolCore {
    
  typedef boost::graph_traits<MolGraph>::vertex_descriptor vertex_descriptor;
  typedef boost::property_map<MolGraph, boost::vertex_index_t>::type indexMap;

  Atom* Molecule::addAtom()
  {
    boost::shared_ptr<Atom> atom(new Atom(this));
    boost::add_vertex(atom, d_graph);
  }

  Atom* Molecule::getAtomByIndex(unsigned int index)
  {
    MolGraph::vertex_descriptor vd = boost::vertex(index, d_graph);
    Atom *atom = d_graph[vd].get();
    return atom;
  }

  Bond* Molecule::addBond()
  {
    boost::shared_ptr<Bond> bond(new Bond(this));
//    boost::add_edge(bond->bond, d_graph);
  }

  unsigned int getAtomIndex(const Molecule &mol, const Atom *atom)
  {
    AtomIterator ai_begin, ai_end, next;
    tie(ai_begin, ai_end) = boost::vertices(mol.getGraph());
    for (next = ai_begin; next != ai_end; ++next) {
      Atom *a = mol.getGraph()[*next].get();

      if (atom = a)
        return *next;    
    }
  }


}
