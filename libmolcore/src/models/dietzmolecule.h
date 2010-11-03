
#include <molcore/concepts/atom.h>
#include <boost/lambda/lambda.hpp>
#include <vector>
#include <utility>
#include <algorithm>

namespace molcore {

  struct DietzAtom
  {
    char atomicNumber, neutronNumber, unsharedElectrons;
  };
  

  typedef std::pair<DietzAtom*, DietzAtom*> DietzAtomPair;

  struct DietzBondSys
  {
    char electrons;
    std::vector<DietzAtomPair> atomPairs;
  };

  struct DietzMolecule
  {
    std::vector<DietzAtom> atoms;
    std::vector<DietzBondSys> electrons;
  };

  /**
   * Associated types for AtomConcept
   */

  template<>
  struct atom_traits<DietzAtom*>
  {
    typedef DietzMolecule* molecule_type;
    typedef int number_type;
    typedef int unshared_electrons_type;
  };

  /**
   * Free functions for AtomConcept<DietzAtom*>.
   */

  int get_atomic_number(const DietzAtom *atom, const DietzMolecule *)
  {
    return atom->atomicNumber;
  }

  int get_neutron_number(const DietzAtom *atom, const DietzMolecule *)
  {
    return atom->neutronNumber;
  }

  int get_unshared_electrons(const DietzAtom *atom, const DietzMolecule *)
  {
    return atom->unsharedElectrons;
  }





  


  





}
