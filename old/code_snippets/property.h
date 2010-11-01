


namespace MolCore {

  class PropertyBase
  {
    public:

      const std::string& getName() const { return d_name; }
      void setName(const std::string &name) { d_name = name; }
    private:
      std::string d_name;  
  };


  template<typename T> struct is_ref { enum { result = false }; };
  template<typename T> struct is_ref<T&> { enum { result = true }; };
  template<typename T> struct is_pointer { enum { result = false }; };
  template<typename T> struct is_pointer<T*> { enum { result = true }; };
  template<typename T> struct is_value { enum { result = true }; };
  template<typename T> struct is_value<T&> { enum { result = false }; };
  template<typename T> struct is_value<T*> { enum { result = false }; };


  template<typename T>
  class Property : public PropertyBase
  {
    public:
      Property() : PropertyBase()
      {
      }
      Property(const std::string &name) : PropertyBase(name)
      {
      }
      Property(const std::string &name, T value) : PropertyBase(name), 
          d_value(value)
      {
      }
      ~Property()
      {
        if (is_pointer<T>::result) {
          delete d_value;
          d_value = 0;
        }
      }

      T getValue() { return d_value; }
      void setValue(T value) { d_value = value; }

    private:
      T d_value;
  };

  class PropertyHolder
  {
    public:
      ~PropertyHolder();
      const std::vector<PropertyBase*>& getProperties() const { return d_props; }
      bool hasProperty(const std::string &name) const;
      PropertyBase* getProperty(const std::string &name) const;
      template<typename T>
      Property<T>* getProperty(const std::string &name) const { return dynamic_cast<Property<T>*>(getProperty(name)); }
      void setProperty(PropertyBase *property) const { d_props.push_back(property); }
      template<typename T>
      void setProperty(Property<T> *property) const { d_props.push_back(property); }
    private:
      mutable std::vector<PropertyBase*> d_props;
  };


}
