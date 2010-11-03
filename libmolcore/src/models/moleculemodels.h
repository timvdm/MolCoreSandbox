#ifndef MC_MOLECULEMODELS_H
#define MC_MOLECULEMODELS_H

#include <molcore/models/molecule.h>
#include <molcore/models/abstractmolecule.h>

#include <boost/any.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>

namespace molcore {


  template<typename Model>
  Model model_cast(Model model)
  {
    typedef typename boost::remove_pointer<Model>::type nonptr;
    return boost::any_cast<nonptr>(model);
  }
  
  

  /**
   * This is the only place for specifying the models
   */
  #define MOLECULE_MODELS (Molecule)(AbstractMolecule)

  /**
   * Create mpl vector from preprocessor sequence. This mpl vector is used to
   * determine the template parameters for plugin factories.
   */
  #define MACRO(r, data, i, elem) BOOST_PP_COMMA_IF(i) BOOST_PP_CAT(elem, data)*
  typedef boost::mpl::vector<BOOST_PP_SEQ_FOR_EACH_I(MACRO, , MOLECULE_MODELS)> molecule_models;
  #undef MACRO

}

#endif
