#include <molcore/fileformat.h>
#include <molcore/detail/pluginfactory.h>
#include <molcore/detail/pluginloader.h>
#include <molcore/pluginmanager.h>

#include <boost/shared_ptr.hpp>

#include <iostream>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

using namespace molcore;


int main()
{
  PluginManager manager;

  std::vector<std::string> names = manager.getPluginNames();
  foreach (const std::string &name, names)
    std::cout << "Plugin: " << name << std::endl;

  boost::shared_ptr<Plugin> plugin = manager.getPlugin("SmilesFormat", "Molecule");
  std::cout << plugin << std::endl;
  std::cout << plugin->getIdentifier() << std::endl;


  return 0;
}
