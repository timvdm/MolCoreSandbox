

#include <boost/graph/adjacency_list.hpp>
#include <boost/smart_ptr.hpp>

namespace MolCore {

  class Atom;
  class Bond;
  class CoordinateSet;

  typedef boost::adjacency_list<boost::vecS,
                                boost::vecS,
                                boost::undirectedS,
                                boost::shared_ptr<Atom>, 
                                boost::shared_ptr<Bond> 
                               > MolGraph;

  typedef boost::graph_traits<MolGraph>::vertex_iterator AtomIterator;
  typedef boost::graph_traits<MolGraph>::edge_iterator BondIterator;



  class Molecule
  {
    public:
      Atom* addAtom();

      Atom* getAtomByIndex(unsigned int index);
      
      Bond* addBond();

      CoordinateSet* getCoordinateSet() const;
      void setCoordinateSet(CoordinateSet *coordinateSet);

      const MolGraph& getGraph() const { return d_graph; }

    protected:
      MolGraph d_graph;
  };


  unsigned int getAtomIndex(const Molecule &mol, const Atom *atom);

  

}
