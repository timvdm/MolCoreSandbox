#ifndef MC_PLUGINMANAGER_H
#define MC_PLUGINMANAGER_H

#include <molcore/plugin.h>
#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>

namespace molcore {

  namespace detail {
    struct PluginLoader;
    struct PluginManagerPrivate;
  }

  class PluginManager
  {
    public:
      struct PluginInfo
      {
        std::string name;
        int arity;
        std::vector<std::string> template_params;
      };

      PluginManager();
      PluginManager(detail::PluginLoader *loader);
      ~PluginManager();

      /**
       * Get a list of available plugin names.
       */
      std::vector<std::string> getPluginPaths();
      /**
       * Add @p path to the list of plugin paths.
       */
      void addPluginPath(const std::string &path);
      /**
       * Get a list of all plugin files found in the plugin paths.
       */
      std::vector<std::string> getPluginFiles();
      /**
       * Get a list of all plugin names.
       */
      std::vector<std::string> getPluginNames();
      /**
       * Get a shared pointer to a plugin. The plugin can be nullary template
       * (non-template), unary template with parameter T1 or binary template with
       * parameters T1 and T2.
       *
       * @param name The name for the plugin (e.g. SmilesFormat)
       * @param T1 The first template parameter for the plugin.
       * @param T2 The second template parameter for the plugin.
       */
      boost::shared_ptr<Plugin> getPlugin(const std::string &name, const std::string &T1 = std::string(), const std::string &T2 = std::string());
    private:
      struct detail::PluginManagerPrivate * const d;
      detail::PluginLoader *d_loader;
  };

}

#endif
