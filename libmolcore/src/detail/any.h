#ifndef MC_ANY_H
#define MC_ANY_H

#include <boost/any.h>

namespace boost {

  template<typename ValueType>
  ValueType model_cast(any *operand)
  {
    typedef BOOST_DEDUCED_TYPENAME remove_reference<ValueType>::type nonref;

#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
    // If 'nonref' is still reference type, it means the user has not
    // specialized 'remove_reference'.

    // Please use BOOST_BROKEN_COMPILER_TYPE_TRAITS_SPECIALIZATION macro
    // to generate specialization of remove_reference for your class
    // See type traits library documentation for details
    BOOST_STATIC_ASSERT(!is_reference<nonref>::value);
#endif

    nonref * result = any_cast<nonref>(&operand);
    if(!result)
      boost::throw_exception(bad_any_cast());
    return *result;
  }

}

