#include <molcore/fileformat.h>
#include <molcore/detail/pluginfactory.h>
#include <molcore/models/moleculemodels.h>

#include <boost/any.hpp>
#include <boost/mpl/at.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>

namespace molcore {

  template<typename MoleculeModel>
  struct SmilesFormat : public FileFormat
  {
    std::string getIdentifier() const
    {
      return "SmilesFormat";
    }

    Plugin::Type getType() const
    {
      return Plugin::FileFormat;
    }

    bool read(boost::any *molecule)
    {
      MoleculeModel *mol = boost::any_cast<MoleculeModel>(molecule);
      if (!mol) {
        // error...
        return false;
      }

      return true;
    }
  };

}








#define MOLCORE_PLUGIN(abstract, concrete) \
extern "C" boost::shared_ptr<molcore::PluginFactory> create_factory() \
{ \
  return boost::shared_ptr<molcore::PluginFactory>(new molcore::NullaryPluginFactory<abstract,concrete>); \
}

#define MOLCORE_UNARY_T_PLUGIN(abstract, concrete, models, index, model) \
extern "C" boost::shared_ptr<molcore::PluginFactory> create_factory_ ## model() \
{ \
  typedef boost::mpl::at<models, boost::mpl::int_<index> >::type T1; \
  return boost::shared_ptr<molcore::PluginFactory>(new molcore::UnaryPluginFactory<abstract, concrete, T1>); \
} 

#define MOLCORE_FILEFORMAT_MACRO(r, data, i, elem) \
  MOLCORE_UNARY_T_PLUGIN(molcore::FileFormat*, molcore::SmilesFormat, molcore::molecule_models, i, elem)

#define MOLCORE_PUSHBACK_MODEL_MACRO(r, data, i, elem) \
  params.push_back(BOOST_PP_STRINGIZE(elem));

#define MOLCORE_FILEFORMAT(name, klass) \
  extern "C" std::string plugin_name() \
  { \
    return name; \
  } \
  extern "C" std::vector<std::string> plugin_template_params() \
  { \
    std::vector<std::string> params; \
    BOOST_PP_SEQ_FOR_EACH_I(MOLCORE_PUSHBACK_MODEL_MACRO, , MOLECULE_MODELS) \
    return params; \
  } \
  BOOST_PP_SEQ_FOR_EACH_I(MOLCORE_FILEFORMAT_MACRO, , MOLECULE_MODELS)

MOLCORE_FILEFORMAT("SmilesFormat", molcore::SmilesFormat)


