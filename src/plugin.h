#ifndef MC_PLUGIN_H
#define MC_PLUGIN_H

#include <string>

namespace molcore {

  struct Plugin
  {
    enum Type {
      FileFormat,
      LastType,
      UserType = 1024
    };

    virtual ~Plugin() {}
    virtual std::string getIdentifier() const = 0;
    virtual Type getType() const = 0;
  };

}

#endif
