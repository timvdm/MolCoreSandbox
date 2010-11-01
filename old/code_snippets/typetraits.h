#ifndef MC_MODEL_H
#define MC_MODEL_H

#include <tr1/memory>

namespace MolCore {

  template<typename T>
  struct RawPointerType
  {
    typedef T* Ptr;
    typedef const T* ConstPtr;
  };

  template<typename T>
  struct SharedPointerType
  {
    typedef std::tr1::shared_ptr<T> Ptr;
    typedef std::tr1::shared_ptr<const T> ConstPtr;      
  };


  template<template <typename> class MoleculeType,
           template <typename> class AtomType,
           template <typename> class BondType,
           template <typename> class ChemistryType,
           template <typename> class PointerType = RawPointerType>
  struct TypeTraits
  {
    typedef MoleculeType<TypeTraits> Molecule;
    typedef AtomType<TypeTraits> Atom;
    typedef BondType<TypeTraits> Bond;

    typedef typename PointerType<Molecule>::Ptr MoleculePtr;
    typedef typename PointerType<Atom>::Ptr AtomPtr;
    typedef typename PointerType<Bond>::Ptr BondPtr;

    typedef std::size_t Index;
    typedef unsigned long UId;

    typedef ChemistryType<TypeTraits> Chemistry;
  };

#define MODEL_TYPES(Model) \
  typedef typename Model::Molecule Molecule; \
  typedef typename Model::Atom Atom; \
  typedef typename Model::Bond Bond; \
  typedef typename Model::MoleculePtr MoleculePtr; \
  typedef typename Model::AtomPtr AtomPtr; \
  typedef typename Model::BondPtr BondPtr; \
  typedef typename Model::UId UId; \
  typedef typename Model::Index Index; \
  typedef typename Model::Chemistry Chemistry;



}

#endif
