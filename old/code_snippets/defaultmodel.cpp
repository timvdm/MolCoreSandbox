#include <defaultmodel.h>

#include <simplemolecule.cxx>
#include <dietzchemistry.cxx>

namespace MolCore {

  /**
   * Explicit template instantiation to make templates work for libraries. Since
   * SimpleAtom and SimpleBond are used by SimpleMolecule, no additional 
   * instantiations are needed for these classes.
   */
  template class SimpleMolecule<DefaultModel>;
  template class DietzChemistry<DefaultModel>;

//  template void DietzChemistry::fromConnectionTable(const DefaultModel::Molecule*);
//  template void DietzChemistry::toConnectionTable(const DefaultModel::Molecule*);

}
