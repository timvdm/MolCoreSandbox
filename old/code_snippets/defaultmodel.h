#ifndef MC_DEFAULTMODEL_H
#define MC_DEFAULTMODEL_H

#include <simplemolecule.h>
#include <dietzchemistry.h>
#include <typetraits.h>

namespace MolCore {

//  typedef TypeTraits<SimpleMolecule, SimpleAtom, SimpleBond, DietzChemistry> DefaultModelType;
  
  struct DefaultModel
  {
    typedef SimpleMolecule<DefaultModel> Molecule;
    typedef SimpleAtom<DefaultModel> Atom;
    typedef SimpleBond<DefaultModel> Bond;
    typedef RawPointerType<Molecule>::Ptr MoleculePtr; 
    typedef RawPointerType<Atom>::Ptr AtomPtr; 
    typedef RawPointerType<Bond>::Ptr BondPtr; 
    typedef unsigned long UId; 
    typedef std::size_t Index; 
    typedef DietzChemistry<DefaultModel> Chemistry;
  };

}

#endif
