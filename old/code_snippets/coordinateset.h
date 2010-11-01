#ifndef COORDINATESET_H
#define COORDINATESET_H

#include <Eigen/Core>
#include <vector>

namespace MolCore {

  struct CoordinateSet3D
  {
    std::vector<Eigen::Vector3d> coords;          
  };

}

#endif
