#ifndef MC_FILEFORMAT_H
#define MC_FILEFORMAT_H

#include <molcore/plugin.h>

namespace boost {
  struct any;
}

namespace molcore {

  struct FileFormat : public Plugin
  {
    virtual bool read(boost::any *data) = 0;
  };

}

#endif
