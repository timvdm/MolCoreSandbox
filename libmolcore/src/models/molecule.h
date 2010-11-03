#ifndef MC_MOLECULE_H
#define MC_MOLECULE_H

#include <molcore/traits.h>
#include <vector>

namespace molcore {

  class Atom;

  class Molecule
  {
    public:
      unsigned int numAtoms() const
      {
        return d_atoms.size();
      }

    private:
      std::vector<Atom*> d_atoms;
  };

  template<>
  struct molecule_traits<Molecule>
  {
  };

}

#endif
