#include <simplemolecule.h>

namespace MolCore {

  template<typename TypeTraits>
  SimpleMolecule<TypeTraits>::SimpleMolecule() : d_chemistry(this)
  {
  }

  template<typename TypeTraits>
  SimpleMolecule<TypeTraits>::~SimpleMolecule()
  {
    for (typename std::vector<AtomPtr>::iterator atom = d_atoms.begin(); atom != d_atoms.end(); ++atom) {
      delete *atom;
      *atom = 0;
    }
    for (typename std::vector<BondPtr>::iterator bond = d_bonds.begin(); bond != d_bonds.end(); ++bond) {
      delete *bond;
      *bond = 0;
    }
  }

  template<typename TypeTraits>
  template<typename T>
  typename T::Ptr SimpleMolecule<TypeTraits>::addT(UId id, std::vector<typename T::Ptr> &objects, std::vector<typename T::Ptr> &objectIds)
  {
    typename T::Ptr ptr(new T(id, this));
    // update objectIds
    if (id < objectIds.size()) {
      // check if the atom already or still exists
      if (objectIds[id])
        return objectIds[id];
    } else {
      // resize objectIds
      objectIds.resize(id+1, 0);
    }
    objectIds[id] = ptr;
    // add ptr to objects and set the index
    ptr->setIndex(objects.size());
    objects.push_back(ptr);
    return ptr;
  }

  template<typename TypeTraits>
  typename TypeTraits::BondPtr SimpleMolecule<TypeTraits>::addBond(UId beginId, UId endId)
  {
    Atom *begin = getAtomById(beginId);
    Atom *end = getAtomById(endId);
    if (!begin || !end)
      return 0;
    BondPtr bond = addBond();
    bond->setBegin(begin);
    bond->setEnd(end);
    return bond;  
  }
  
  template<typename TypeTraits>
  typename TypeTraits::BondPtr SimpleMolecule<TypeTraits>::getBond(UId beginId, UId endId)
  {
    Atom *begin = getAtomById(beginId);
    Atom *end = getAtomById(endId);
    if (!begin || !end)
      return 0; 
    const std::vector<BondPtr> &bonds = begin->getBonds();
    for (Index i = 0; i < bonds.size(); ++i) {
      if (bonds[i]->getOther(begin) == end)
        return bonds[i];
    }
    return 0;  
  }

  template<typename TypeTraits>
  void SimpleMolecule<TypeTraits>::sanitize()
  {
    for (typename std::vector<AtomPtr>::iterator atom = d_atoms.begin(); atom != d_atoms.end(); ++atom) {
    }
  }






  template<typename TypeTraits>
  SimpleAtom<TypeTraits>::SimpleAtom(UId id, MoleculePtr molecule) : 
      d_molecule(molecule), d_id(id), d_element(Element::NoElement),
      d_formalCharge(0), d_aromatic(false)
  {
  }

  template<typename TypeTraits>
  void SimpleAtom<TypeTraits>::addBond(BondPtr bond)
  {
    typename std::vector<BondPtr>::iterator b = std::find(d_bonds.begin(), d_bonds.end(), bond);
    if (b == d_bonds.end())
      d_bonds.push_back(bond);
  }
  
  template<typename TypeTraits>
  void SimpleAtom<TypeTraits>::removeBond(BondPtr bond)
  {
    typename std::vector<BondPtr>::iterator b = std::find(d_bonds.begin(), d_bonds.end(), bond);
    if (b != d_bonds.end())
      d_bonds.erase(b);
  }





  template<typename TypeTraits>
  SimpleBond<TypeTraits>::SimpleBond(UId id, Molecule *molecule) : 
      d_molecule(molecule), d_id(id), d_begin(0), d_end(0), d_bondOrder(0),
      d_aromatic(false)
  {
  }

  template<typename TypeTraits>
  void SimpleBond<TypeTraits>::setBegin(AtomPtr begin)
  {
    if (d_begin == begin)
      return;
    if (d_begin)
      d_begin->removeBond(this);
    d_begin = begin;
    if (d_begin)
      d_begin->addBond(this);
  }

  template<typename TypeTraits>
  void SimpleBond<TypeTraits>::setEnd(AtomPtr end)
  {
    if (d_end == end)
      return;
    if (d_end)
      d_end->removeBond(this);
    d_end = end;
    if (d_end)
      d_end->addBond(this);
  }

  template<typename TypeTraits>
  void SimpleBond<TypeTraits>::set(AtomPtr begin, AtomPtr end, double bondOrder)
  {
    setBegin(begin);
    setEnd(end);
    setBondOrder(bondOrder);
  }


}
