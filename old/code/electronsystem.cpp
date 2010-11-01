
#include <vector>

namespace MolCore {

  class Atom;
  class Molecule;

  class ElectronSystem
  {
    public:
      enum Type {
        SigmaType,
        PiType,
        CoordType
      };

      explicit ElectronSystem();
      virtual ~ElectronSystem();

      virtual Type getType() const = 0;

      std::vector<Atom*> getAtoms() const;
      void setAtoms(const std::vector<Atom*> &atoms);

      int getNumElectrons() const;
      void setNumElectrons(int numElectrons);

    protected:
      Molecule           *d_molecule;
      std::vector<Atom*>  d_atoms;
      int                 d_numElectrons;
  };

  ElectronSystem::ElectronSystem() : d_molecule(0), d_numElectrons(0)
  {
  }
  
  ElectronSystem::~ElectronSystem()
  {
  }

  std::vector<Atom*> ElectronSystem::getAtoms() const
  {
    return d_atoms;
  }

  void ElectronSystem::setAtoms(const std::vector<Atom*> &atoms)
  {
    d_atoms = atoms;
  }

  int ElectronSystem::getNumElectrons() const
  {
    return d_numElectrons;
  }

  void ElectronSystem::setNumElectrons(int numElectrons)
  {
    d_numElectrons = numElectrons;
  }




  class SigmaElectrons : public ElectronSystem
  {
    public:
      enum Geometry {
        TwoCenter, //!< regular 2 electron, 2 center sigma system
        Bridged, //!< bridged sigma electron system (e.g. B-H-B in B5H11, transition states, ...)
        Closed //!< closed sigma electron system (e.g. BBB in B5H11)
      };
      
      SigmaElectrons(Geometry geometry = TwoCenter) : ElectronSystem(), d_geometry(geometry) {}
      virtual ~SigmaElectrons() {}

      Type getType() const { return SigmaType; }

      Geometry getGeometry() const { return d_geometry; }
      void setGeometry(Geometry geometry) { d_geometry = geometry; }

    protected:
      Geometry d_geometry;
  };

  class PiElectrons : public ElectronSystem
  {
    public:
      PiElectrons() : ElectronSystem() {}
      virtual ~PiElectrons() {}

      Type getType() const { return PiType; }
  };

  class CoordElectrons : public ElectronSystem
  {
    public:
      CoordElectrons() : ElectronSystem() {}
      virtual ~CoordElectrons() {}

      Type getType() const { return CoordType; }
  };


  class BridgeSigmaElectrons : public ElectronSystem
  {
    public:
  
  };


}
