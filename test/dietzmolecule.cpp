#include <molcore/models/dietzmolecule.h>
#include <iostream>


using namespace molcore;


int main()
{

  BOOST_CONCEPT_ASSERT((AtomConcept<DietzAtom*>));


  return 0;
}
