#include <molcore/fileformat.h>
#include <molcore/detail/pluginfactory.h>
#include <molcore/moleculemodels.h>

#include <boost/any.hpp>
#include <boost/mpl/at.hpp>
#include <boost/shared_ptr.hpp>

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


extern "C" std::string plugin_name()
{
  return "SmilesFormat";
}



#define MOLCORE_PLUGIN(abstract,concrete) \
extern "C" boost::shared_ptr<molcore::PluginFactory> create_factory() \
{ \
  return new molcore::NullaryPluginFactory<abstract,concrete>; \
}

#define MOLCORE_UNARY_T_PLUGIN(abstract,concrete,models,model) \
extern "C" boost::shared_ptr<molcore::PluginFactory> create_factory_ABI_ ## model() \
{ \
  typedef boost::mpl::at<models, boost::mpl::int_<model> >::type T1; \
  return boost::shared_ptr<molcore::PluginFactory>(new molcore::UnaryPluginFactory<abstract, concrete, T1>); \
} 

MOLCORE_UNARY_T_PLUGIN(molcore::FileFormat*, molcore::SmilesFormat, molcore::molecule_models, 0);
MOLCORE_UNARY_T_PLUGIN(molcore::FileFormat*, molcore::SmilesFormat, molcore::molecule_models, 1);

