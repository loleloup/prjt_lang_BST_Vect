/* GNU C++ version 10.2 - "g++ -std=c++17"
 * Leloup, Loïc - 475607 - 2020/12 - <Cont.hpp>
 * Exemple de conteneur (projet n°1)
 */
#ifndef _CONT_H_
#define _CONT_H_

#include  "Vect_fix.hpp"
#include  "BST.hpp"
#include  <cstddef>             // nullptr_t, size_t, ptrdiff_t, byte...
#include  <utility>             // swap, move, forward...
#include  <exception>
#include  <stdexcept>           // standard exceptions
#include  <ostream>             // output streams
#include <iostream>

// Common output operator ====================================================

namespace _Cont_base {
  namespace {
    template <typename U>
    struct _Base {
      constexpr void _dsp (std::ostream& out) const
        {static_cast<const U*>(this)->_dsp(out);}
    };
  }
}

template <typename U>
std::ostream& operator<< (std::ostream& out, const _Cont_base::_Base<U>& b)
  {b._dsp(out); return out;}

// Abstract base class =======================================================

template <typename T>
class Cont_base { // abstract
public:
  class Info;
  class Ptr2Info;
protected:
  static const Info _EMPTY;
  // Access methods
  static constexpr std::ptrdiff_t _index (const Info& i) {return i._index;}
  static constexpr std::ptrdiff_t& _index (Info& i) {return i._index;}
  static constexpr const Info* _ptr (const Ptr2Info& p) {return p._ptr;}
  static constexpr const Info*& _ptr (Ptr2Info& p) {return p._ptr;}
  // Implementation
  std::size_t _used = 0;
  virtual void _dsp (std::ostream&) const = 0;
  // Non virtual assignations => protected
  Cont_base& operator= (const Cont_base&) = default;
  Cont_base& operator= (Cont_base&& c)
    {if (this != &c) {_used = c._used; c._used = 0;} return *this;}
public:
  // Getter
  constexpr std::size_t used () const noexcept {return _used;};
  // Constructors & destructor
  constexpr Cont_base () noexcept = default;
  constexpr Cont_base (const Cont_base&) noexcept = default;
  constexpr Cont_base (Cont_base&& c) noexcept: _used(c._used) {c._used = 0;}
  virtual ~Cont_base () noexcept = default;
}; // Cont_base<T>

template <typename T>
const typename Cont_base<T>::Info Cont_base<T>::_EMPTY{};


// Embedded class Info =======================================================

template <typename T>
class Cont_base<T>::Info:
    public _Cont_base::_Base<typename Cont_base<T>::Info> {
  friend class Cont_base<T>;   // for _index static methods
  std::ptrdiff_t _index = -1;
  const T _data{};
  friend struct _Cont_base::_Base<Info>;
  constexpr void _dsp (std::ostream& out) const {out << _data;}
public:
  // Traits
  using value_type      = T;
  using reference       = T&;
  using const_reference = const T&;
  // Constructors & casts
  constexpr Info () noexcept = default;
  constexpr Info (std::ptrdiff_t i, const T& v) noexcept: _index(i), _data(v) {}
  constexpr Info (const T& v) noexcept: _data(v) {}             // implicit cast
  constexpr operator const T& () const noexcept {return _data;} // implicit cast
  template <typename>
  friend constexpr bool operator< (const Info&, const Info&) noexcept;
  template <typename>
  friend constexpr bool operator== (const Info&, const Info&) noexcept;
  constexpr bool operator< (const Info& i) const noexcept
    {return _data < i._data;}
  constexpr bool operator== (const Info& i) const noexcept
    {return _data == i._data;}
}; // Info

// Embedded class Ptr2Info ===================================================

template <typename T>
class Cont_base<T>::Ptr2Info:
    public _Cont_base::_Base<typename Cont_base<T>::Ptr2Info> {
  friend class Cont_base<T>;   // for _ptr static methods
  const Info *_ptr = nullptr;
  friend struct _Cont_base::_Base<Ptr2Info>;
public:
  constexpr void _dsp (std::ostream& out) const
    {out << (_ptr ? *_ptr : _EMPTY);}
public:
  // Traits
  using value_type      = T;
  using reference       = T&;
  using const_reference = const T&;
  // Constructors & casts
  constexpr Ptr2Info () noexcept = default;
  constexpr operator const Info& () const noexcept     // implicit cast
    {return _ptr ? *_ptr : _EMPTY;}
  constexpr operator const T& () const noexcept        // implicit cast
    {return _ptr ? *_ptr : _EMPTY;}
  // Getter
  constexpr bool isEmpty () const noexcept {return !_ptr;}
  constexpr bool operator< (const Ptr2Info& i) const noexcept
    {return _ptr->_data < i._ptr->_data;}
  constexpr bool operator== (const Ptr2Info& i) const noexcept
    {return _ptr->_data == i._ptr->_data;}
}; // Ptr2Info

namespace _Cont_base {
  template <typename> using _Base = void;   // "destroy" access to real _Base
}

// Main class ================================================================

template <typename T>
class Cont final: private Cont_base<T>, public BST<typename Cont_base<T>::Info>, public Vect_fix<typename Cont_base<T>::Ptr2Info> {
  using _Base = Cont_base<T>;
  using _Vect = Vect_fix<typename Cont_base<T>::Ptr2Info>;
  using _BST =  BST<typename Cont_base<T>::Info>;
  using _Base::_index;
  using _Base::_ptr;
  using _Base::_used;


public:
  // Traits
  using value_type      = T;
  using reference       = T&;
  using const_reference = const T&;
  using Info = typename _Base::Info;
  using Ptr2Info = typename _Base::Ptr2Info;
  using _Vect::dim;
  using _Base::used;

  // Constructeur
  constexpr explicit Cont(std::size_t DIM) noexcept : _Base(), _BST(), _Vect(DIM) {};

  // Copy, transfert, etc...
public:
  Cont(const Cont& c) noexcept : _Base(c), _BST(c), _Vect(c.dim()) {
    _BST::traverse([this](const Info& v){_ptr(operator[] (_index(v))) = &v;});};
  Cont(Cont&& c) noexcept : _Base(c), _BST(c), _Vect(c) {};
  Cont& operator= (Cont&&) noexcept;
  Cont& operator= (const Cont& cont){
    if(this != &cont){
      _Base::operator= (cont);
      _BST::operator= (cont);
      _Vect::operator= (cont);
      _BST::traverse([this](const Info& v){_ptr(operator[] (_index(v))) = &v;});
    }
    return *this;
  };

  //Vect overrides
  const Ptr2Info& operator[] (std::ptrdiff_t) const override;

  // BST overrides
  const Info& find (const Info&) const noexcept override;
  const Info& insert (const Info&) override;
  bool erase (const Info&) override;

  //display
  void _dsp (std::ostream&) const override;
  template<typename U>
  friend inline std::ostream& operator<< (std::ostream&, const Cont<U>&);

  //destructor
  virtual ~Cont() noexcept = default;
}; // Cont<T>


/////////////////////// Copy, transfer ////////////////////////////
template<typename T>
Cont<T>& Cont<T>::operator= (Cont<T>&& cont) noexcept {
  if(this != &cont){
    _Base::operator= (cont);
    _BST::operator= (cont);
    _Vect::operator= (cont);
  }

  return *this;
}


////////////////////// Random Access //////////////////////////////////////
template<typename T>
const typename Cont<T>::Ptr2Info& Cont<T>::operator[] (std::ptrdiff_t i) const{
  return _Vect::operator[](i);
}


///////////////////////////////// BST overrides ////////////////////////////////
template<typename T>
const typename Cont<T>::Info& Cont<T>::insert (const Info& v){
  std::ptrdiff_t i = _index(v);
  if (i == -1){ //si pas d'index précisé
    throw std::out_of_range("Pas d'index précisé lors d'une insert dans Cont");
  }

  if (_BST::exists(v)){
    throw std::domain_error("Le conteneur n'accepte pas de doublons");
  }

  if (not operator[](i).isEmpty()){ //efface l'ancienne valeur dans le BST si index déjà occupé
    //std::cout << "efface index " << i << " de valeur : " << *_ptr(operator[](i)) << " dans insert avec " << v << std::endl;
    //std::cout << "il est 4h et tout va bien\n";
    erase(operator[](i));
    //std::cout << "erase succesful? " << (res ? "true" : "false") << std::endl;
  }

  _ptr(operator[](i)) = &_BST::insert(v);
  //std::cout << "vect added verif : " << _ptr(operator[](i)) << " introduit : " << &v << std::endl;
  _used += 1;
  return v;
}


template<typename T>
bool Cont<T>::erase (const Info& v){
  std::ptrdiff_t i = _index(v);
  bool res = false;

  if (i == -1){  //no index precised
    i = _index(find(v));  //find index
    res = _BST::erase(v);  //erase in BST
    if (res){
      _ptr(operator[](i)) = nullptr;  //delete ptr
      _used -= 1;
    }
  }

  else if (_index(find(v)) == i){ //given index == index in BST
    _ptr(operator[](i)) = nullptr;
    res = _BST::erase(v);
    _used -= 1;
  }
  return res;
}

template<typename T>
const typename Cont<T>::Info& Cont<T>::find (const Info& v) const noexcept{  //remplacer par un using
  return _BST::find(v);
}



/////////////////////////////// display ///////////////////////////////
template<typename T>
void Cont<T>::_dsp (std::ostream& out) const {  //remplacer par using
  _BST::_dsp(out);
  out << " || ";
  _Vect::_dsp(out);
}

template<typename T>
inline std::ostream & operator<<(std::ostream &out, const Cont<T> &cont) {
  out << "[ ";
  cont._dsp(out);
  out << "]";
  return out;
}



// Deduction guides ==========================================================

template <typename T>
Cont (const Vect_fix<T*>&) -> Cont<typename T::value_type>;

template <typename T>
Cont (const BST<T>&) -> Cont<typename T::value_type>;

#endif // _CONT_H_

