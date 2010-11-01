#include "dietzchemistry.h"
#include "element.h"

#include <numeric>

namespace MolCore {

  namespace detail {

    template<typename Atom>
    void findAromaticAtoms(Atom *atom, std::vector<Atom*> &atoms, std::vector<bool> &visited)
    {
      if (std::find(atoms.begin(), atoms.end(), atom) != atoms.end())
        return;
      atoms.push_back(atom);
      visited[atom->getIndex()] = true;

      typename std::vector<typename Atom::BondPtr>::const_iterator b = atom->getBonds().begin();
      for (; b != atom->getBonds().end(); ++b) {
        if (!(*b)->isAromatic())
          continue;
        Atom *nbr = (*b)->getOther(atom);
        if (!nbr->isAromatic())
          continue;
        findAromaticAtoms(nbr, atoms, visited);
      }
    
    }

    template<typename Atom>
    int getNumAromaticBonds(Atom *atom)
    {
      int result = 0;
      typename std::vector<typename Atom::BondPtr>::const_iterator b = atom->getBonds().begin();
      for (; b != atom->getBonds().end(); ++b)
        if ((*b)->isAromatic())
          result++;
      return result;    
    }

    int getNumElectronsInAromaticRing(int numAtoms)
    {
      int numElectrons = 0;
      for (int n = 0; n < 100; ++n) {
        numElectrons = 4 * n + 2;
        if (numElectrons >= numAtoms)
          break;
      }
      return numElectrons;    
    }

  }

  using namespace std;


  template<typename Model>
  DietzChemistry<Model>::DietzChemistry(Molecule *molecule) : d_molecule(molecule)
  {
  }



  template<typename Model>
  void DietzChemistry<Model>::fromConnectionTable(const typename Model::Molecule *mol)
  {
    MODEL_TYPES(Model)

    // Add the atoms
    const std::vector<AtomPtr> &atoms = mol->getAtoms();
    d_atoms.resize(atoms.size());
    for (std::size_t i = 0; i < atoms.size(); ++i) {
      Atom *a = atoms[i];
      DietzAtom &atom = d_atoms[i];
      atom.element = a->getElement();
      atom.hydrogens = a->getValence() - a->getNumBonds();
      atom.electrons = a->getElectrons();
    }

    // Add the bonds
    std::vector<DietzBondSys*> piSystems;
    const std::vector<BondPtr> &bonds = mol->getBonds();
    d_bondSystems.reserve(bonds.size());
    for (std::size_t i = 0; i < bonds.size(); ++i) {
      int bondOrder = bonds[i]->getBondOrder();
      if (bondOrder >= 1) {
        d_bondSystems.resize(d_bondSystems.size()+1);
        d_bondSystems.back().electrons = 2;
        d_bondSystems.back().atomPairs.push_back(typename DietzBondSys::AtomPair( 
              bonds[i]->getBegin()->getId(), bonds[i]->getEnd()->getId()));
      }
    }


    // Handle single bond aromatic systems
    std::vector<bool> visited(atoms.size(), false);
    for (std::size_t i = 0; i < atoms.size(); ++i) {
      if (visited[i])
        continue;
      Atom *a = atoms[i];
      if (a->isAromatic()) {
        // make a list of the atoms in the aromatic fragment
        std::vector<Atom*> aromaticAtoms;
        detail::findAromaticAtoms(a, aromaticAtoms, visited);
        // create a bond system
        d_bondSystems.resize(d_bondSystems.size()+1);
        d_bondSystems.back().electrons = detail::getNumElectronsInAromaticRing(aromaticAtoms.size());
        for (std::size_t j = 0; j < aromaticAtoms.size(); ++j) {
          for (std::size_t k = 0; k < aromaticAtoms.size(); ++k) {
            if (j >= k)
              continue;

            UId jId = aromaticAtoms[j]->getId();
            UId kId = aromaticAtoms[k]->getId();
            Bond *bond = d_molecule->getBond(jId, kId);
            if (!bond)
              continue;
            if (detail::getNumAromaticBonds(aromaticAtoms[j]) >= 3 && detail::getNumAromaticBonds(aromaticAtoms[k]) >= 3)
              continue;
            d_bondSystems.back().atomPairs.push_back(typename DietzBondSys::AtomPair(jId, kId)); 
          }
        }
      }
    }
 


  }

  template<typename Model>
  void DietzChemistry<Model>::toConnectionTable(Molecule *mol)
  {
  
  }

  template<typename Model>
  double DietzChemistry<Model>::computeBondOrder(UId atom1, UId atom2)
  {
    std::vector<DietzBondSys*> bondSystems(getBondSystems(atom1, atom2));
    double sum = 0.0;
    for (UId i = 0; i < bondSystems.size(); ++i)
      sum += bondSystems[i]->electrons / bondSystems[i]->atomPairs.size(); // note: atomPairs is always non-empty
    return 0.5 * sum;
  }

  template<typename Model>
  double DietzChemistry<Model>::computeFormalCharge(UId atomId)
  {
    DietzAtom &atom = d_atoms[d_molecule->getAtomById(atomId)->getIndex()];

    std::vector<DietzBondSys*> bondSystems(getBondSystems(atomId));
    double sum = 0.0;
    for (UId i = 0; i < bondSystems.size(); ++i)
      sum += (bondSystems[i]->electrons * getNumAtomPairs(atomId, bondSystems[i]->atomPairs)) / (2.0 * bondSystems[i]->atomPairs.size());

    int numElectrons = std::accumulate(atom.electrons.begin(), atom.electrons.end(), 0);
    double fc = Element::getValenceElectrons(atom.element) - numElectrons - sum - atom.hydrogens;
    return fc;
  }

  template<typename Model>
  std::vector<typename DietzChemistry<Model>::DietzBondSys*> DietzChemistry<Model>::getBondSystems(UId beginId, UId endId)
  {
    std::vector<DietzBondSys*> result;

    for (typename DietzBondSysSet::iterator bondsys = d_bondSystems.begin(); bondsys != d_bondSystems.end(); ++bondsys) {
      for (typename DietzBondSys::AtomPairSetIter p = bondsys->atomPairs.begin(); p != bondsys->atomPairs.end(); ++p) {
        if ((p->first == beginId && p->second == endId) || (p->first == endId && p->second == beginId)) {
          result.push_back(&*bondsys);
          break;
        }
      }
    }

    return result;
  }

  template<typename Model>
  std::vector<typename DietzChemistry<Model>::DietzBondSys*> DietzChemistry<Model>::getBondSystems(UId atomId)
  {
    std::vector<DietzBondSys*> result;

    for (typename DietzBondSysSet::iterator bondsys = d_bondSystems.begin(); bondsys != d_bondSystems.end(); ++bondsys) {
      for (typename DietzBondSys::AtomPairSetIter p = bondsys->atomPairs.begin(); p != bondsys->atomPairs.end(); ++p) {
        if (p->first == atomId || p->second == atomId) {
          result.push_back(&*bondsys);
          break;
        }
      }
    }

    return result;
  }

  template<typename Model>
  unsigned int DietzChemistry<Model>::getNumAtomPairs(UId atomId, const typename DietzBondSys::AtomPairSet &atomPairs) const
  {
    unsigned int result = 0;
    for (typename DietzBondSys::AtomPairSetConstIter p = atomPairs.begin(); p != atomPairs.end(); ++p)
      if (p->first == atomId || p->second == atomId)
        result++;
    return result;
  }

}
