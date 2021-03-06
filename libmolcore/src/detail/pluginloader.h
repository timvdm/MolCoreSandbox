#ifndef MC_PLUGIN_LOADER
#define MC_PLUGIB_LOADER

#include <molcore/detail/pluginfactory.h>

#include <boost/shared_ptr.hpp>
#include <map>
#include <vector>

namespace molcore {
  namespace detail {

    struct PluginLoader
    {
      /**
       * Map plugin name to filename
       *
       */
      typedef std::map<std::string, std::string> PluginInfo;

      virtual ~PluginLoader() {}
       
      virtual std::string getPluginName(const std::string &filename) = 0;
      virtual std::vector<std::string> getPluginTemplateParams(const std::string &filename) = 0;

      virtual boost::shared_ptr<PluginFactory> factory(const std::string &filename, const std::string &symbol) = 0;


      static PluginLoader* getInstance();
    };

  }
}

#endif
