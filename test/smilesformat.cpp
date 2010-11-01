#include <molcore/fileformat.h>
#include <molcore/detail/pluginfactory.h>
#include <molcore/detail/pluginloader.h>

#include <boost/shared_ptr.hpp>

#include <iostream>


namespace molcore {


  class PluginManager
  {
    public:
      struct PluginInfo
      {
        std::vector<std::string> name;
      };

      PluginManager();
      PluginManager(detail::PluginLoader *loader);
      ~PluginManager();

      std::vector<std::string> getPluginNames();

      /**
       * Get a shared pointer to a plugin.
       *
       * @param name The name for the plugin (e.g.
       */
      boost::shared_ptr<Plugin> getPlugin(const std::string &name, const std::string &T1 = std::string());

      //boost::shared_ptr<FileFormat> getFileFormat(const std::string &name, const std::string &moleculeModel)
    private:
      struct PluginManagerPrivate * const d;
      detail::PluginLoader *d_loader;
  };


  struct PluginManagerPrivate
  {
    std::map<std::string, void*> handles;
  };


  PluginManager::PluginManager() : d(new PluginManagerPrivate),
      d_loader(detail::PluginLoader::getInstance())
  {
  }

  PluginManager::PluginManager(detail::PluginLoader *loader) : d(new PluginManagerPrivate),
      d_loader(loader)
  {
  }
      
  std::vector<std::string> PluginManager::getPluginNames()
  {
    return d_loader->getPluginNames();
  }



  PluginManager::~PluginManager()
  {
    if (d_loader)
      delete d_loader;
    delete d;
  }

  boost::shared_ptr<Plugin> PluginManager::getPlugin(const std::string &name, const std::string &T1)
  {
    boost::shared_ptr<molcore::PluginFactory> factory = d_loader->factory(name, "create_factory_ABI_0");
    boost::shared_ptr<Plugin> plugin = factory->create();
    return plugin;
  }




}

using namespace molcore;
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH



int main()
{
  PluginManager manager;

  std::vector<std::string> names = manager.getPluginNames();
  foreach (const std::string &name, names)
    std::cout << "Plugin: " << name << std::endl;

  boost::shared_ptr<Plugin> plugin = manager.getPlugin("SmilesFormat");
  std::cout << plugin << std::endl;
  std::cout << plugin->getIdentifier() << std::endl;


  return 0;
}
