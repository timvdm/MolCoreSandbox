#ifndef MC_MOLECULEMODELS_H
#define MC_MOLECULEMODELS_H

#include <molcore/molecule.h>
#include <molcore/abstractmolecule.h>

#include <boost/any.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/remove_pointer.hpp>

namespace molcore {


  template<typename Model>
  Model model_cast(Model model)
  {
    typedef typename boost::remove_pointer<Model>::type nonptr;
    return boost::any_cast<nonptr>(model);
  }
  
  
  typedef boost::mpl::vector<Molecule*, AbstractMolecule*> molecule_models;

  template<typename Model>
  struct model_traits;

  template<>
  struct model_traits<Molecule*>
  {
    static const char* identifier() { return "Molecule00"; }
  };

  template<>
  struct model_traits<AbstractMolecule*>
  {
    static const char* identifier() { return "AbstractMolecule00"; }
  };




}

#endif
