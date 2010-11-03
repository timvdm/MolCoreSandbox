

namespace molcore {

  template<typename UnsharedElectronsModel>
  struct UnsharedElectronsConcept
  {
    BOOST_CONCEPT_USAGE(UnsharedElectronsConcept)
    {
      int numElectrons = num_electrons(unshared_electrons);
      int numElectronSystems = num_electron_systems(unshared_electrons);
    }

    UnsharedElectronsModel unshared_electrons;
  };

  template<typename SharedElectronsModel>
  struct SharedElectronsConcept
  {
    typedef shared_electrons_traits<SharedElectronsModel>::atom_pair_iterator iter;

    BOOST_CONCEPT_USAGE(SharedElectronsConcept)
    {
      int numElectrons = num_electrons(shared_electrons);
      int numAtomPairs = num_atom_pairs(shared_electrons);
      atom_pair_iter i = get_atom_pairs(shared_electrons);
    }

    SharedElectronsModel shared_electrons;
  };



}
