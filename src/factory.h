namespace MolCore {


  template<typename AbstractProduct, typename Identifier>
  struct DefaultFactoryError
  {
    AbstractProduct* onUnknownType(const Identifier &id)
    {
      return 0;
    }
  };


  template<typename AbstractProduct, typename Identifier, typename Creator = AbstractProduct* (*)(),
      typename ErrorPolicy = DefaultFactoryError>
  class Factory : public ErrorPolicy<AbstractProduct, Identifier>
  {
    public:
      void registerProduct(const Identifier &id, Creator creator)
      {
        m_map[id] = creator;
      }
      AbstractProduct* create(const Identifier &id)
      {
        typename MapType::const_iterator i = m_map.find(id);
        if (i != m_map.end())
          return (i->second)();
        return onUnknownType(id);
      }
    private:
      MapType m_map;
  };


}
