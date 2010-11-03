#include <molcore/pluginmanager.h>
#include <molcore/detail/pluginloader.h>
#include <molcore/foreach.h>

#include <boost/assign/list_of.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <map>
#include <iostream>

namespace molcore {

  namespace detail {

    struct PluginInfo
    {
      PluginInfo() {}
      PluginInfo(const std::string &_file, const std::string &_name, const std::vector<std::string> &_params)
          : file(_file), name(_name), params(_params) {}
      std::string file;
      std::string name;
      std::vector<std::string> params;
    };

    typedef std::map<std::string, PluginInfo> PluginMap;

    struct PluginManagerPrivate
    {
      detail::PluginMap plugins;

      void refreshPluginMap(PluginManager *self, PluginLoader *loader)
      {
        using boost::algorithm::to_lower_copy;

        plugins.clear();
        foreach (const std::string &file, self->getPluginFiles()) {
          // Get the plugin name (e.g. SmilesFormat)
          std::string Name = loader->getPluginName(file);
          if (!Name.empty()) {
            // Generate the C function name (e.g. smilesformat_Molecule, smilesformat_AbstractMolecule)
            const std::string name = to_lower_copy(Name);
            std::vector<std::string> params = loader->getPluginTemplateParams(file);
            foreach (const std::string &param, params) {
              std::string function_name = name + "_" + param;
              std::cout << function_name << " -> " << file << std::endl;
              plugins[function_name] = PluginInfo(file, name, params);
            }
          }
        }
      }

    };

    std::string plugin_key(const std::string &name, const std::string &T1, const std::string &T2)
    {
      using boost::algorithm::to_lower_copy;

      std::string function(to_lower_copy(name));
      if (!T1.empty())
        function += "_" + T1;
      if (!T2.empty())
        function += "_" + T2;
      return function;
    }


    std::string plugin_function_name(const std::string &T1, const std::string &T2)
    {
      std::string function("create_factory");
      if (!T1.empty())
        function += "_" + T1;
      if (!T2.empty())
        function += "_" + T2;
      return function;
    }

  }

  PluginManager::PluginManager() : d(new detail::PluginManagerPrivate),
      d_loader(detail::PluginLoader::getInstance())
  {
  }

  PluginManager::PluginManager(detail::PluginLoader *loader) : d(new detail::PluginManagerPrivate),
      d_loader(loader)
  {
  }
 
  PluginManager::~PluginManager()
  {
    if (d_loader)
      delete d_loader;
    delete d;
  }


  std::vector<std::string> PluginManager::getPluginPaths()
  {
    return boost::assign::list_of("lib")("/usr/lib/molcore")("/usr/local/lib/molcore");
  }
     
  std::vector<std::string> PluginManager::getPluginFiles()
  {
    typedef boost::filesystem::directory_iterator dir_iter;

    // Find all .dyn files in the plugin paths (i.e. getPluginPaths)
    std::vector<std::string> files;
    foreach(const std::string &path, getPluginPaths()) {
      if (!boost::filesystem::exists(path))
        continue;
      dir_iter end; // default constructor yields past-the-end
      for (dir_iter i(path); i != end; ++i) {
        if (boost::filesystem::is_directory(i->status()))
          continue;
        if (i->path().extension() != ".dyn")
          continue;
        files.push_back(i->string());
        std::cout << i->path() << std::endl;
      }
    }

    return files;
  }

  std::vector<std::string> PluginManager::getPluginNames()
  {
    std::vector<std::string> names, files = getPluginFiles();

    foreach(const std::string &file, files) {
      std::string name = d_loader->getPluginName(file);
      if (!name.empty())
        names.push_back(name);
    }

    return names;
  }

  boost::shared_ptr<Plugin> PluginManager::getPlugin(const std::string &name, const std::string &T1, const std::string &T2)
  {
    // Check if the correct plugin is in the map.
    detail::PluginMap::iterator it = d->plugins.find(detail::plugin_key(name, T1, T2));
    if (it == d->plugins.end()) {
      // Refresh the map if needed.
      d->refreshPluginMap(this, d_loader);
      // Check again.
      it = d->plugins.find(detail::plugin_key(name, T1, T2));
      if (it == d->plugins.end())
        return boost::shared_ptr<Plugin>();
    }

    // The plugin is found
    std::string file = it->second.file;

    boost::shared_ptr<molcore::PluginFactory> factory = d_loader->factory(file, detail::plugin_function_name(T1, T2));
    boost::shared_ptr<Plugin> plugin;
    if (factory)
      plugin = factory->create();
    return plugin;
  }

}
