#ifndef MC_DIETZCHEMISTRY_H
#define MC_DIETZCHEMISTRY_H

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "typetraits.h"


namespace MolCore {

  template<typename Model>
  class DietzChemistry
  {
    public:
      MODEL_TYPES(Model)

      DietzChemistry(Molecule *molecule);
      /**
       * Compute the formal bond order for the bond between @p atom1 and @p atom2.
       *
       * 0.5 * sum{ n(b) / p(b) }
       *
       */
      double computeBondOrder(UId atom1, UId atom2);
      double computeFormalCharge(UId atom);

      void fromConnectionTable(const Molecule *mol);
      void toConnectionTable(Molecule *mol);

    private:
      struct DietzAtom
      {
        unsigned char element;
        unsigned char hydrogens;
        std::vector<char> electrons;
      };

      struct DietzBondSys
      {
        typedef std::pair<UId, UId> AtomPair;
        typedef std::vector<AtomPair> AtomPairSet;
        typedef typename AtomPairSet::iterator AtomPairSetIter;
        typedef typename AtomPairSet::const_iterator AtomPairSetConstIter;

        unsigned short electrons;
        AtomPairSet atomPairs;
      };

      typedef std::vector<DietzAtom> DietzAtomSet;
      typedef std::vector<DietzBondSys> DietzBondSysSet;

      std::vector<DietzBondSys*> getBondSystems(UId atom1, UId atom2);
      std::vector<DietzBondSys*> getBondSystems(UId atom);
      unsigned int getNumAtomPairs(UId atom, const typename DietzBondSys::AtomPairSet &atomPairs) const;

      MoleculePtr d_molecule;
      std::vector<DietzAtom> d_atoms;
      std::vector<DietzBondSys> d_bondSystems;
  };

}

#endif
