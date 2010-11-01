#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>


typedef std::size_t Index;



unsigned short getValenceElectrons(unsigned short element)
{
  switch (element) {
    case 1:
    case 3:
    case 11:
    case 19:
    case 37:
    case 55:
    case 87:
      return 1;
    case 2:
    case 4:
    case 12:
    case 20:
    case 38:
    case 56:
    case 88:
      return 2;
    case 5:
    case 13:
    case 31:
    case 49:
    case 81:
    case 113:
      return 3;
    case 6:
    case 14:
    case 32:
    case 50:
    case 82:
    case 114:
      return 4;
    case 7:
    case 15:
    case 33:
    case 51:
    case 83:
    case 115:
      return 5;
    case 8:
    case 16:
    case 34:
    case 52:
    case 84:
    case 116:
      return 6;
    case 9:
    case 17:
    case 35:
    case 53:
    case 85:
    case 117:
      return 7;
    case 10:
    case 18:
    case 36:
    case 54:
    case 86:
    case 118:
      return 8;
    default:
      return 0;
  }
}


class DietzMolecule
{

  public:

    /**
     * Compute the formal bond order for the bond between @p atom1 and @p atom2.
     *
     * 0.5 * sum{ n(b) / p(b) }
     *
     */
    double computeFormalBondOrder(Index atom1, Index atom2)
    {
      std::vector<DietzBondSys*> bondSystems(getBondSystems(atom1, atom2));
      double sum = 0.0;
      for (Index i = 0; i < bondSystems.size(); ++i)
        sum += bondSystems[i]->electrons / bondSystems[i]->atomPairs.size(); // note: atomPairs is always non-empty
      return 0.5 * sum;
    }

    double computeFormalCharge(Index atom)
    {
      std::vector<DietzBondSys*> bondSystems(getBondSystems(atom));
      double sum = 0.0;
      for (Index i = 0; i < bondSystems.size(); ++i)
        sum += (bondSystems[i]->electrons * getNumAtomPairs(atom, bondSystems[i]->atomPairs)) / (2.0 * bondSystems[i]->atomPairs.size());

      double fc = getValenceElectrons(d_atoms[atom].element) - d_atoms[atom].unshared - sum;
      return fc;
    }



  private:
    struct DietzAtom
    {
      unsigned short unshared;
      unsigned short element;
      Index index;
    };

    struct DietzBondSys
    {
      typedef std::pair<Index, Index> AtomPair;
      typedef std::vector<AtomPair> AtomPairSet;
      typedef AtomPairSet::iterator AtomPairSetIter;
      typedef AtomPairSet::const_iterator AtomPairSetConstIter;

      unsigned short electrons;
      AtomPairSet atomPairs;
    };

    typedef std::vector<DietzAtom> DietzAtomSet;
    typedef std::vector<DietzBondSys> DietzBondSysSet;

    std::vector<DietzBondSys*> getBondSystems(Index atom1, Index atom2)
    {
      std::vector<DietzBondSys*> result;

      for (DietzBondSysSet::iterator bondsys = d_bondSystems.begin(); bondsys != d_bondSystems.end(); ++bondsys) {
        for (DietzBondSys::AtomPairSetIter p = bondsys->atomPairs.begin(); p != bondsys->atomPairs.end(); ++p) {
          if ((p->first == atom1 && p->second == atom2) || (p->first == atom2 && p->second == atom1)) {
            result.push_back(&*bondsys);
            break;
          }
        }
      }

      return result;
    }

    std::vector<DietzBondSys*> getBondSystems(Index atom)
    {
      std::vector<DietzBondSys*> result;

      for (DietzBondSysSet::iterator bondsys = d_bondSystems.begin(); bondsys != d_bondSystems.end(); ++bondsys) {
        for (DietzBondSys::AtomPairSetIter p = bondsys->atomPairs.begin(); p != bondsys->atomPairs.end(); ++p) {
          if (p->first == atom || p->second == atom) {
            result.push_back(&*bondsys);
            break;
          }
        }
      }

      return result;
    }

    unsigned int getNumAtomPairs(Index atom, const DietzBondSys::AtomPairSet &atomPairs) const
    {
      unsigned int result = 0;
      for (DietzBondSys::AtomPairSetConstIter p = atomPairs.begin(); p != atomPairs.end(); ++p)
        if (p->first == atom || p->second == atom)
          result++;
      return result;
    }




    std::vector<DietzAtom> d_atoms;
    std::vector<DietzBondSys> d_bondSystems;
};


class DietzAtomConfig
{

  public:
    struct PaddleWheel
    {
      struct HalfPlane
      {
        std::vector<Index> lower; // ordered atoms in lower half plane
        std::vector<Index> inter; // atom on peripendicular plane
        std::vector<Index> upper; // ordered atoms in upper half plane

        bool isEmpty() const
        {
          return lower.empty() && inter.empty() && upper.empty();
        }

        bool operator==(const HalfPlane &other) const
        {
          if (lower != other.lower)
            return false;
          if (inter != other.inter)
            return false;
          if (upper != other.upper)
            return false;
          return true;
        }

        bool operator!=(const HalfPlane &other) const
        {
          return !(*this == other);
        }

      };

      typedef std::vector<HalfPlane> HalfPlaneList;
      typedef HalfPlaneList::iterator HalfPlaneListIter;
      typedef HalfPlaneList::const_iterator HalfPlaneListConstIter;


      Index getHalfPlaneIndexForAtom(Index atom)
      {
        Index index = 0;
        for (HalfPlaneListIter halfPlane = halfPlanes.begin(); halfPlane != halfPlanes.end(); ++halfPlane, ++index) {
          if (std::find(halfPlane->lower.begin(), halfPlane->lower.end(), atom) != halfPlane->lower.end())
            return index;
          if (std::find(halfPlane->inter.begin(), halfPlane->inter.end(), atom) != halfPlane->inter.end())
            return index;
          if (std::find(halfPlane->upper.begin(), halfPlane->upper.end(), atom) != halfPlane->upper.end())
            return index;
        }
        return index;
      }

      bool operator==(const PaddleWheel &other) const
      {
        // ensure the neighbor atoms are the same
        if (neighbor != other.neighbor)
          return false;
        // ensure the number of half planes are the same
        if (halfPlanes.size() != other.halfPlanes.size())
          return false;

        // rotate the other half planes cyclic list until it starts with the same element as this->halfPlanes
        HalfPlaneList otherHalfPlanes(other.halfPlanes);
        for (Index i = 0; i < otherHalfPlanes.size(); ++i) {
          if (halfPlanes.front() == otherHalfPlanes.front())
            break;
          std::rotate(otherHalfPlanes.begin(), otherHalfPlanes.begin()+1, otherHalfPlanes.end());
        }

        // compare the half planes
        for (Index i = 0; i < otherHalfPlanes.size(); ++i)
          if (halfPlanes[i] != otherHalfPlanes[i])
            return false;

        return true;
      }

      bool operator!=(const PaddleWheel &other) const
      {
        return !(*this == other);
      }


      Index neighbor; // neighbor atom (i.e. y)
      HalfPlaneList halfPlanes; // clockwise cyclic list of half planes
    };

    typedef std::vector<PaddleWheel> PaddleWheelSet;
    typedef PaddleWheelSet::iterator PaddleWheelSetIter;
    typedef PaddleWheelSet::const_iterator PaddleWheelSetConstIter;

    Index getPaddleWheelIndexForNeighbor(Index neighbor) const
    {
      Index index = 0;
      for (PaddleWheelSetConstIter paddleWheel = d_paddleWheels.begin(); paddleWheel != d_paddleWheels.end(); ++paddleWheel, ++index)
        if (paddleWheel->neighbor == neighbor)
          return index;
      return index;
    }

    bool operator==(const DietzAtomConfig &other) const
    {
      // make sure the central atoms are the same
      if (d_atom != other.d_atom)
        return false;
      // check to make sure the number of paddle wheels are the same
      if (d_paddleWheels.size() != other.d_paddleWheels.size())
        return false;
      // check the set of paddle wheels
      for (PaddleWheelSetConstIter paddleWheel = d_paddleWheels.begin(); paddleWheel != d_paddleWheels.end(); ++paddleWheel) {
        Index otherPaddleWheelIndex = other.getPaddleWheelIndexForNeighbor(paddleWheel->neighbor);
        if (otherPaddleWheelIndex >= d_paddleWheels.size())
          return false;
        if (*paddleWheel != other.d_paddleWheels[otherPaddleWheelIndex])
          return false;
      }

      return true;
    }

    Index getAtomIndex() const { return d_atom; }
    const PaddleWheelSet& getPaddleWheelSet() const { return d_paddleWheels; }

  private:
    /**
     * The central atom for the atom configuration. This is x in the Dietz paper.
     */
    Index d_atom;
    /**
     * The set of configurational paddle wheels for this atom configuration.
     */
    PaddleWheelSet d_paddleWheels;
};


class DietzConform
{

};



namespace std {

  ostream& operator<<(ostream &out, const DietzAtomConfig::PaddleWheel::HalfPlane &halfPlane)
  {
    if (halfPlane.isEmpty()) {
      out << "E";
      return out;
    }

    out << "((";
    for (std::vector<Index>::const_iterator atom = halfPlane.lower.begin(); atom != halfPlane.lower.end(); ++atom) {
      out << *atom;
      if (atom + 1 != halfPlane.lower.end())
        out << ",";
    }
    out << "),";

    if (halfPlane.inter.empty()) {
      out << "e";
    } else {
      out << halfPlane.inter[0];
    }

    out << ",(";
    for (std::vector<Index>::const_iterator atom = halfPlane.upper.begin(); atom != halfPlane.upper.end(); ++atom) {
      out << *atom;
      if (atom + 1 != halfPlane.upper.end())
        out << ",";
    }
    out << "))";

    return out;
  }

  ostream& operator<<(ostream &out, const DietzAtomConfig::PaddleWheel &paddleWheel)
  {
    typedef DietzAtomConfig::PaddleWheel::HalfPlaneListConstIter HalfPlaneIter;
    out << "(" << paddleWheel.neighbor << ",(";
    for (HalfPlaneIter halfPlane = paddleWheel.halfPlanes.begin(); halfPlane != paddleWheel.halfPlanes.end(); ++halfPlane) {
      out << *halfPlane;
      if (halfPlane + 1 != paddleWheel.halfPlanes.end())
        out << ",";
    }
    out << "))";  
    return out;
  }

  ostream& operator<<(ostream &out, const DietzAtomConfig &config)
  {
    out << "AtConfig(" << config.getAtomIndex() << ") =" << endl;
    
    stringstream ss;
    ss << " (" << config.getAtomIndex() << ",{";
    out << ss.str();
    
    string spacer = ss.str();
    for (Index i = 0; i < spacer.size(); ++i)
      spacer[i] = ' ';
    
    for (DietzAtomConfig::PaddleWheelSetConstIter paddleWheel = config.getPaddleWheelSet().begin(); paddleWheel != config.getPaddleWheelSet().end(); ++paddleWheel) {
      out << spacer << *paddleWheel;
      if (paddleWheel + 1 != config.getPaddleWheelSet().end())
        out << "," << endl;
    }

    out << "})" << endl;
    return out;
  }

}


int main()
{


}
