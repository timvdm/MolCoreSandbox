#include "element.h"

namespace MolCore {
  
  unsigned short Element::getGroup(int element)
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
      case 4:
      case 12:
      case 20:
      case 38:
      case 56:
      case 88:
        return 2;
      case 22:
      case 40:
      case 72:
      case 104:
        return 4;
      case 23:
      case 41:
      case 73:
      case 105:
        return 5;
      case 24:
      case 42:
      case 74:
      case 106:
        return 6;
      case 25:
      case 43:
      case 75:
      case 107:
        return 7;
      case 26:
      case 44:
      case 76:
      case 108:
        return 8;
      case 27:
      case 45:
      case 77:
      case 109:
        return 9;
      case 28:
      case 46:
      case 78:
      case 110:
        return 10;
      case 29:
      case 47:
      case 79:
      case 111:
        return 11;
      case 30:
      case 48:
      case 80:
      case 112:
        return 12;
      case 5:
      case 13:
      case 31:
      case 49:
      case 81:
      case 113:
        return 13;
      case 6:
      case 14:
      case 32:
      case 50:
      case 82:
      case 114:
        return 14;
      case 7:
      case 15:
      case 33:
      case 51:
      case 83:
      case 115:
        return 15;
      case 8:
      case 16:
      case 34:
      case 52:
      case 84:
      case 116:
        return 16;
      case 9:
      case 17:
      case 35:
      case 53:
      case 85:
      case 117:
        return 17;
      case 2:
      case 10:
      case 18:
      case 36:
      case 54:
      case 86:
      case 118:
        return 18;
      default:
        return 3;
    }  
  }

  unsigned short Element::getPeriod(int element)
  {
    if (element < 3)
      return 1;
    if (element < 11)
      return 2;
    if (element < 19)
      return 3;
    if (element < 37)
      return 4;
    if (element < 55)
      return 5;
    if (element < 87)
      return 6;
    return 7;
  }

  unsigned short Element::getValenceElectrons(int element)
  {
    switch (getGroup(element)) {
      case 1:
        return 1;
      case 2:
        return 2;
      case 13:
        return 3;
      case 14:
        return 4;
      case 15:
        return 5;
      case 16:
        return 6;
      case 17:
        return 7;
      case 18:
        return 8;
      default:
        return 0;
    }
  }
    

  unsigned short Element::getExpectedValence(int element)
  {
    switch (getGroup(element)) {
      case 1:
        return 1;
      case 2:
        return 2;
      case 13:
        return 3;
      case 14:
        return 4;
      case 15:
        return 3;
      case 16:
        return 2;
      case 17:
        return 1;
      case 18:
        return 0;
      default:
        return 0;
    }  
  }

}
