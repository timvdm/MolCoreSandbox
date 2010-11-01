#ifndef MC_UNIXPLUGINLOADER_H
#define MC_UNIXPLUGINLOADER_H

#include <molcore/detail/pluginloader.h>
#include <molcore/plugin.h>

#include <boost/shared_ptr.hpp>
#include <dlfcn.h>
#include <iostream>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include <boost/assign/list_of.hpp>


namespace molcore {
  namespace detail {
    
    struct UnixPluginLoader : public PluginLoader
    {
      typedef std::vector<std::string> plugin_names_fn();
      typedef boost::shared_ptr<PluginFactory> create_t();
      typedef void destroy_t(PluginFactory*);

      template<typename ReturnType, typename FunctionType>
      ReturnType execute_function(const std::string &filename, const std::string &functionName, bool *error = 0)
      {
        if (error)
          *error = true;

        // open the library
        void *handle = dlopen(filename.c_str(), RTLD_LAZY);
        if (!handle) {
          std::cerr << "Cannot load library: " << dlerror() << std::endl;
          return ReturnType();
        }

        // reset errors
        dlerror();

        // load the symbol
        FunctionType *function = reinterpret_cast<FunctionType*>(dlsym(handle, functionName.c_str()));
        const char *dlsym_error = dlerror();
        if (dlsym_error) {
          std::cerr << "Cannot load symbol " << functionName << ": " << dlsym_error << std::endl;
          return ReturnType();
        }

        // execute the function
        ReturnType result = function();

        // close the library
        dlclose(handle);

        if (error)
          *error = false;

        return result;
      }

      boost::shared_ptr<PluginFactory> factory(const std::string &filename, const std::string &symbol)
      {
        // open the library
        void *handle = dlopen(filename.c_str(), RTLD_LAZY);
        if (!handle) {
          std::cerr << "Cannot load library: " << dlerror() << std::endl;
          return boost::shared_ptr<PluginFactory>();
        }

        // reset errors
        dlerror();

        // load the symbols
        create_t *create_factory = reinterpret_cast<create_t*>(dlsym(handle, symbol.c_str()));
        const char *dlsym_error = dlerror();
        if (dlsym_error) {
          std::cerr << "Cannot load symbol " << symbol << ": " << dlsym_error << std::endl;
          return boost::shared_ptr<PluginFactory>();
        }

        boost::shared_ptr<PluginFactory> factory = create_factory();

        return factory;
      }

      void getPluginInfo(PluginInfo &pluginInfo, const std::vector<std::string> &files)
      {

        foreach (const std::string &file, files) {
          void *handle = dlopen(file.c_str(), RTLD_LAZY);
          if (handle) {
            std::cerr << "Cannot load library: " << dlerror() << std::endl;
            continue;
          }

          // reset errors
          dlerror();
          
          plugin_names_fn *plugin_names = reinterpret_cast<plugin_names_fn*>(dlsym(handle, "plugin_names"));
          const char *dlsym_error = dlerror();
          if (dlsym_error) {
            std::cerr << "Cannot load symbol plugin_names: " << dlsym_error << std::endl;
            continue;
          }

          std::vector<std::string> pluginNames = plugin_names();
          foreach (const std::string &pluginName, pluginNames)
            pluginInfo[pluginName] = file;

          dlclose(handle);
        }
      }

      std::string getPluginName(const std::string &filename)
      {
        return execute_function<std::string, std::string()>(filename, "plugin_name");
      }


    };
      
    PluginLoader* PluginLoader::getInstance()
    {
      return new UnixPluginLoader;
    }
    
    std::vector<std::string> PluginLoader::getPluginNames()
    {
      std::vector<std::string> names, files = getPluginFiles();

      foreach(const std::string &file, files) {
        std::string name = getPluginName(file);
        if (!name.empty())
          names.push_back(name);
      }

      return names;
    }

    std::vector<std::string> PluginLoader::getPluginFiles()
    {
      return boost::assign::list_of("lib/libsmilesformat.so");
    }


  }
}

#endif
