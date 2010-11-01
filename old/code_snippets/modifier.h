#ifndef MC_MODIFIER_H
#define MC_MODIFIER_H





namespace MolCore {



  template<typename Model>
  class Modifier
  {
    public:
      MODEL_TYPES(Model)
      
      Modifier(Molecule *molecule) : d_molecule(molecule)
      {
      }

      Modifier<Model>& addAtom(Element::Number element, int position = 0, int bondOrder = 1)
      {
        Atom *previous = 0;
        if (d_positions.size()) {
          if (position < d_positions.back().size())
            previous = d_molecule->getAtom(d_positions.back()[position]);
        }

        Atom *atom = d_molecule->addAtom();
        atom->setElement(element);
        if (previous) {
          Bond *bond = d_molecule->addBond(previous->getId(), atom->getId());
          bond->setBondOrder(bondOrder);
        }

        std::vector<Index> positions;
        positions.push_back(atom->getIndex());
        d_positions.push_back(positions);

        return *this;
      }

      Modifier<Model>& addChain(int length, int position = 0)
      {
        Atom *previous = 0;
        if (d_positions.size()) {
          if (position < d_positions.back().size())
            previous = d_molecule->getAtom(d_positions.back()[position]);
        }

        std::vector<Index> positions;
        for (int i = 0; i < length; ++i) {
          Atom *atom = d_molecule->addAtom();
          atom->setElement(Element::Carbon);
          if (previous) {
            Bond *bond = d_molecule->addBond(previous->getId(), atom->getId());
            bond->setBondOrder(1);
          }
          positions.push_back(atom->getIndex());
          previous = atom;
        }
        d_positions.push_back(positions);

        return *this;
      }

      Modifier<Model>& addRing(int size, bool aromatic = false, int position = 0)
      {
        Atom *previous = 0;
        if (d_positions.size()) {
          if (position < d_positions.back().size())
            previous = d_molecule->getAtom(d_positions.back()[position]);
        }

        Atom *first = 0;
        std::vector<Index> positions;
        for (int i = 0; i < size; ++i) {
          Atom *atom = d_molecule->addAtom();
          atom->setElement(Element::Carbon);
          atom->setAromatic(aromatic);
          if (previous) {
            Bond *bond = d_molecule->addBond(previous->getId(), atom->getId());
            bond->setBondOrder(1);
            bond->setAromatic(aromatic);
          }
          positions.push_back(atom->getIndex());
          previous = atom;
          if (!first)
            first = atom;
        }
        Bond *bond = d_molecule->addBond(first->getId(), previous->getId());
        bond->setBondOrder(1);
        bond->setAromatic(aromatic);


        d_positions.push_back(positions);

        return *this;
      }


      

      Modifier<Model>& stop()
      {
        if (d_positions.size())
          d_positions.pop_back();              
      }

    private:
      MoleculePtr d_molecule;
      std::vector< std::vector<Index> > d_positions;
  };





}


#endif
