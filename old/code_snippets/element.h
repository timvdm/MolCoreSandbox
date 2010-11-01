#ifndef MC_ELEMENT_H
#define MC_ELEMENT_H

namespace MolCore {

  struct Element
  {
    enum Number {
      NoElement = 0,
      Hydrogen,
      Helium,
      Lithium,
      Beryllium,
      Boron,
      Carbon,
      Nitrogen,
      Oxygen,
      Fluorine,
      Neon,
      Sodium,
      Magnesium,
      Aluminium,
      Silicon,
      Phosphorus,
      Sulfur,
      Chlorine,
      Argon,
      Potassium,
      Calcium,
      Scandium,
      Titanium,
      Vanadium,
      Chromium,
      Manganese,
      Iron,
      Cobalt,
      Nickel,
      Copper,
      Zinc,
      Gallium,
      Germanium,
      Arsenic,
      Selenium,
      Bromine,
      Krypton,

      H = 1, He, 
      Li, Be, B, C, N, O, F, Ne,
      Na, Mg, Al, Si, P, S, Cl, Ar,
      K, Ca, Sc, Ti, V, Cr, Mn, Fe, Co, Ni, Cu, Zn, Ga, Ge, As, Se, Br, Kr,
      Rb, Sr, Y, Zr, Nb, Mo, Tc, Ru, Rh, Pd, Ag, Cd, In, Sn, Sb, Te, I, Xe
 
    };

    static unsigned short getGroup(int element);
    static unsigned short getPeriod(int element);
    static unsigned short getValenceElectrons(int element);
    static unsigned short getExpectedValence(int element);
  };

}

#endif
