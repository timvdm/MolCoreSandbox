#ifndef MC_SIMPLEMOLECULE_H
#define MC_SIMPLEMOLECULE_H

#include <element.h>

#include <vector>
#include <algorithm>
#include <numeric>
#include <iostream>

namespace MolCore {

  template<typename TypeTraits>
  class SimpleMolecule
  {
    public:
      typedef typename TypeTraits::MoleculePtr Ptr;
      typedef typename TypeTraits::Molecule Molecule;
      typedef typename TypeTraits::Atom Atom;
      typedef typename TypeTraits::Bond Bond;
      typedef typename TypeTraits::MoleculePtr MoleculePtr;
      typedef typename TypeTraits::AtomPtr AtomPtr;
      typedef typename TypeTraits::BondPtr BondPtr;
      typedef typename TypeTraits::UId UId;
      typedef typename TypeTraits::Index Index;

      typedef typename TypeTraits::Chemistry Chemistry;

      SimpleMolecule();
      ~SimpleMolecule();

      //@name Atoms
      //@{
      /**
       * Add a new atom to the molecule.
       */
      AtomPtr addAtom() { return addAtom(d_atomIds.size()); }
      /**
       * Add a new atom with the specified unique @p id.
       */
      AtomPtr addAtom(UId id) { return addT<Atom>(id, d_atoms, d_atomIds); }
      /**
       * Get the number of atom.
       */
      std::size_t getNumAtoms() const { return d_atoms.size(); }
      /**
       * Get a const reference to the std::vector with all atom pointers.
       */
      const std::vector<AtomPtr>& getAtoms() const { return d_atoms; }
      /**
       * Get an atom by @p index. These indexes are in the range 0 to 
       * getNumAtoms() - 1. Removing atoms will update indexes.
       */
      AtomPtr getAtom(Index index) { return getT<Index, Atom>(index, d_atoms); }
      /**
       * Get an atom by unique @p id. 
       */
      AtomPtr getAtomById(UId id) { return getT<UId, Atom>(id, d_atomIds); }
      //@}

      //@name Bonds
      //@{
      /**
       * Add a new bond to the molecule.
       */
      BondPtr addBond() { return addBond(d_bondIds.size()); }
      /**
       * Add a new bond with the specified unique @p id.
       */
      BondPtr addBond(UId id) { return addT<Bond>(id, d_bonds, d_bondIds); }
      /**
       * Add a new bond between the atoms with @p beginId and @p endId. If the
       * specified unique ids are invalid, no bond will be added and 0 will be
       * returned.
       */
      BondPtr addBond(UId beginId, UId endId);
      /**
       * Get the number of bonds.
       */
      std::size_t getNumBonds() const { return d_bonds.size(); }
      /**
       * Get a const reference to the std::vector with all bond pointers.
       */
      const std::vector<BondPtr>& getBonds() const { return d_bonds; }
      /**
       * Get a bond by @p index.
       */
      BondPtr getBond(Index index) { return getT<Index, Bond>(index, d_bonds); }
      /**
       * Get the bond between the atoms with @p beginId and @p endId.
       */
      BondPtr getBond(UId beginId, UId endId);
      /**
       * Get a bond by unique @p id.
       */
      BondPtr getBondById(UId id) { return getT<UId, Bond>(id, d_bondIds); }
      //@}


      void sanitize();



      Chemistry& getChemistry() { return d_chemistry; }

    private:
      template<typename T>
      typename T::Ptr addT(UId id, std::vector<typename T::Ptr> &objects, std::vector<typename T::Ptr> &objectIds);
      
      template<typename IndexT, typename T>
      typename T::Ptr getT(IndexT index, std::vector<typename T::Ptr> &objects)
      {
        if (index < objects.size())
          return objects[index];
        return 0;
      }

      std::vector<AtomPtr> d_atoms;
      std::vector<BondPtr> d_bonds;
      std::vector<AtomPtr> d_atomIds;
      std::vector<BondPtr> d_bondIds;
      Chemistry d_chemistry;
  };

  template<typename TypeTraits>
  class SimpleAtom
  {
    public:
      /*enum {
        Type = TypeTraits::AtomType
      };*/
      typedef typename TypeTraits::AtomPtr Ptr;
      typedef typename TypeTraits::Molecule Molecule;
      typedef typename TypeTraits::Atom Atom;
      typedef typename TypeTraits::Bond Bond;
      typedef typename TypeTraits::MoleculePtr MoleculePtr;
      typedef typename TypeTraits::AtomPtr AtomPtr;
      typedef typename TypeTraits::BondPtr BondPtr;
      typedef typename TypeTraits::UId UId;
      typedef typename TypeTraits::Index Index;




      Index getIndex() const { return d_index; }
      UId getId() const { return d_id; }
      Element::Number getElement() const { return d_element; }
      void setElement(Element::Number element) { d_element = element; }
      std::size_t getNumBonds() const { return d_bonds.size(); }
      const std::vector<BondPtr>& getBonds() const { return d_bonds; }

      bool isAromatic() const { return d_aromatic; }
      void setAromatic(bool aromatic) { d_aromatic = aromatic; }

      int getFormalCharge() const { return d_formalCharge; }
      void setFormalCharge(int formalCharge) 
      { 
        d_formalCharge = formalCharge;
        d_electrons.clear();        
      }

      void sanitize() const
      {
        if (d_electrons.empty()) {
          int valence;
          if (Element::getGroup(d_element) > 14)
            valence = Element::getExpectedValence(d_element) + d_formalCharge;
          else
            valence = Element::getExpectedValence(d_element) - d_formalCharge;

          int numElectrons = Element::getValenceElectrons(d_element) - valence - d_formalCharge;
          setElectrons(numElectrons);
        } 
      }

      const std::vector<char>& getElectrons() const
      { 
        sanitize();
        return d_electrons; 
      }
      int getNumElectrons() const
      {
        sanitize();
        return std::accumulate(d_electrons.begin(), d_electrons.end(), 0);
      }
      void setElectrons(int pair1, int pair2, int pair3 = 0, int pair4 = 0) const
      { 
        d_electrons.clear();
        d_electrons.push_back(pair1);
        d_electrons.push_back(pair2);
        if (pair3)
          d_electrons.push_back(pair3);
        if (pair4)
          d_electrons.push_back(pair4);
      }
      void setElectrons(int numElectrons) const
      {
        d_electrons.clear();
        std::cout << "setElectrons(" << numElectrons << ")" << std::endl;
        bool radical = numElectrons % 2;
        int numPairs = radical ? (numElectrons-1) / 2 : numElectrons / 2;
        for (int i = 0; i < numPairs; ++i)
          d_electrons.push_back(2);
        if (radical)
          d_electrons.push_back(1);
      }
      /**
       * Get the valence for the atom. The valence is the maximum number of
       * single bonds needed considering the formal charge.
       */
      int getValence() const
      {
        return Element::getValenceElectrons(d_element) - getNumElectrons() - d_formalCharge;
      }


    protected:
      friend class TypeTraits::Molecule; // setIndex
      friend class TypeTraits::Bond; // addBond and removeBond

      SimpleAtom(UId id, MoleculePtr molecule);
      void setIndex(Index index) { d_index = index; }
      void addBond(BondPtr bond);
      void removeBond(BondPtr bond);

      MoleculePtr d_molecule;
      Index d_index;
      const UId d_id;
      Element::Number d_element;
      char d_formalCharge;
      mutable std::vector<char> d_electrons;
      bool d_aromatic;
      std::vector<BondPtr> d_bonds;
  };

  template<typename TypeTraits>
  class SimpleBond
  {
    public:
      /*enum {
        Type = TypeTraits::BondType
      };*/
      typedef typename TypeTraits::BondPtr Ptr;
      typedef typename TypeTraits::Molecule Molecule;
      typedef typename TypeTraits::Atom Atom;
      typedef typename TypeTraits::Bond Bond;
      typedef typename TypeTraits::MoleculePtr MoleculePtr;
      typedef typename TypeTraits::AtomPtr AtomPtr;
      typedef typename TypeTraits::BondPtr BondPtr;
      typedef typename TypeTraits::UId UId;
      typedef typename TypeTraits::Index Index;

      AtomPtr getBegin() const { return d_begin; }
      AtomPtr getEnd() const { return d_end; }
      AtomPtr getOther(AtomPtr atom) { return (atom == d_begin) ? d_end : d_begin; }
      int getBondOrder() const { return d_bondOrder; }

      void setBegin(AtomPtr begin);
      void setEnd(AtomPtr end);
      void setBondOrder(double bondOrder) { d_bondOrder = bondOrder; }
      void set(AtomPtr begin, AtomPtr end, double bondOrder = 1.0);

      bool isAromatic() const { return d_aromatic; }
      void setAromatic(bool aromatic) { d_aromatic = aromatic; }


    protected:
      friend class TypeTraits::Molecule; // setIndex

      SimpleBond(UId id, Molecule *molecule);
      void setIndex(Index index) { d_index = index; }

      MoleculePtr d_molecule;
      Index d_index;
      const UId d_id;
      AtomPtr d_begin, d_end;
      unsigned char d_bondOrder;
      bool d_aromatic;
  };





}

#endif
