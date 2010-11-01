
#include <vector>

namespace MolCore {

  class Atom;
  class Molecule;

  /**
   * Base class for electron systems.
   *
   * The ElectronSystem class and it's subclasses are an alternative way to
   * store how electrons are distributed in a molecule. It overcomes many 
   * problems associated with connection tables and can be used to represent
   * a wide variety molecules.
   *
   * The SigmaElectrons class 
   *
   */
  class ElectronSystem
  {
    public:
      enum Type {
        SigmaType,
        PiType,
        CoordType,
        DeficientSigmaType,
      };

      explicit ElectronSystem();
      virtual ~ElectronSystem();

      /**
       * Get the type for this electron system
       */
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
