


namespace molcore {

  class Atom
  {
    public:
      int getFormatCharge() const
      {
        return d_fcharge;
      }

    private:

      std::vector<Bond*> d_bonds;

  };

  template<>
  struct atom_traits<Atom>
  {
    typedef Molecule molecule_type;

    typedef int fcharge_type;
  };

  class Hydrogen
  {
  };

  //    B C  N  O  F
  //      Si P  S  Cl
  //      Ge As Se Br
  //      Sn Sb Te I
  // Hg   Pb Bi

}
