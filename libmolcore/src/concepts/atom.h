#ifndef MC_ATOMCONCEPT_H
#define MC_ATOMCONCEPT_H

#include <boost/concept/requires.hpp>
#include <boost/concept_check.hpp>

namespace molcore {

  /**
   * The @p atom_traits class provides the associated types for the @p
   * AtomConcept.
   */
  template<typename AtomModel>
  struct atom_traits;

  /**
   * Concept checking class for the @p AtomConcept. 
   */
  template<typename AtomModel>
  struct AtomConcept
  {
    typedef typename atom_traits<AtomModel>::molecule_type MoleculeModel;
    typedef typename atom_traits<AtomModel>::number_type number_type;
    typedef typename atom_traits<AtomModel>::unshared_electrons_type unshared_electrons_type;

    BOOST_CONCEPT_USAGE(AtomConcept)
    {
      number_type atomicNumber = get_atomic_number(atom, molecule);
      number_type neutronNumber = get_neutron_number(atom, molecule);
      unshared_electrons_type unshared = get_unshared_electrons(atom, molecule);
    }

    AtomModel atom;
    MoleculeModel molecule;
  };

  template<typename GraphAtomModel>
  struct graph_atom_traits;

  template<typename GraphAtomModel>
  struct GraphAtomConcept
  {
    typedef typename graph_atom_traits<GraphAtomModel>::molecule_type MoleculeModel;
    typedef typename graph_atom_traits<GraphAtomModel>::bond_iterator bond_iter;
    typedef typename graph_atom_traits<GraphAtomModel>::atom_iterator atom_iter;
    typedef typename graph_atom_traits<GraphAtomModel>::num_bonds_type num_bonds_type;

    BOOST_CONCEPT_USAGE(GraphAtomConcept)
    {
      // Valence Bond / Connection Table
      num_bonds_type numBonds = num_bonds(atom, molecule);
      bond_iter bi = get_bonds(atom, molecule);
      atom_iter ai = get_neighbors(atom, molecule);
    }

    GraphAtomModel atom;
    MoleculeModel molecule;
  };




}

#endif 
