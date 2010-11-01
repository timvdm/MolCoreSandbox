#ifndef MC_ABSTRACTMOLECULE_H
#define MC_ABSTRACTMOLECULE_H

#include <molcore/traits.h>

namespace molcore {

  struct AbstractMolecule
  {
    virtual unsigned int numAtoms() const = 0;
  };

  template<>
  struct molecule_traits<AbstractMolecule>
  {
  };

}

#endif
