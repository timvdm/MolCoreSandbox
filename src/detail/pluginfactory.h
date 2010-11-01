#ifndef MC_PLUGINFACTORY_H
#define MC_PLUGINFACTORY_H

#include <molcore/plugin.h>
#include <boost/shared_ptr.hpp>

namespace molcore {

  struct PluginFactory
  {
    virtual boost::shared_ptr<Plugin> create() const = 0;
  };

  template<typename AbstractPlugin, typename ConcretePlugin>
  struct NullaryPluginFactory : public PluginFactory
  {
    boost::shared_ptr<Plugin> create() const
    {
      return new ConcretePlugin;
    }
  };

  template<typename AbstractPlugin, template<typename> class ConcretePlugin, typename Model>
  struct UnaryPluginFactory : public PluginFactory
  {
    boost::shared_ptr<Plugin> create() const
    {
      return boost::shared_ptr<Plugin>(new ConcretePlugin<Model>);
    }
  };


}


#endif
