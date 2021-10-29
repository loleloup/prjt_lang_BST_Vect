/* GNU C++ version 10.2 - "g++ -std=c++17"
 * Leloup Loïc - 2020/09 - <Vect_fix.hpp>
 * ADT de vecteur statique, mais paramétrique (template)
 * Taille fixée dans le type, modifiable par transfert/copie
 */
#ifndef _VECT_FIX_H_
#define _VECT_FIX_H_


#include  <cstddef>             // nullptr_t, size_t, ptrdiff_t, byte...
#include  <exception>
#include  <stdexcept>           // exceptions standard
#include  <ostream>             // Flux d'output
#include <iostream>

template <typename T>
class Vect_fix {
  std::size_t DIM;
  T *_val;
  inline static T* _cp (const Vect_fix&);
public:
  // constructeurs
  explicit Vect_fix (std::size_t d) noexcept : DIM(d), _val(new T[d]) {
  };   // Tableau vide

  // observateurs
  std::size_t dim () const noexcept {return DIM;}
  virtual const T& operator[] (std::ptrdiff_t) const;
  // modificateurs
  virtual T& operator[] (std::ptrdiff_t);

  //copy, transfert
  Vect_fix (const Vect_fix &v) noexcept: DIM(v.DIM), _val(_cp(v)) {}
  constexpr Vect_fix (Vect_fix&& v) noexcept: DIM(v.DIM), _val(v._val)
  {v.DIM = 0; v._val = nullptr;}
  virtual Vect_fix& operator= (const Vect_fix&) noexcept;
  virtual Vect_fix& operator= (Vect_fix&&) noexcept;



  // fonctions externes
  template <typename U>
  friend inline std::ostream& operator<< (std::ostream&, const Vect_fix<U>&);
  void _dsp (std::ostream& out) const{
    out << *this;
  }
  //destructeur
  virtual ~Vect_fix() noexcept {delete[] _val;}
}; // Vect_fix<T>

// observateurs =============================================================

template <typename T>
const T& Vect_fix<T>::operator[] (std::ptrdiff_t idx) const {
  if (std::size_t(idx) >= DIM)
    throw std::domain_error("Vect_fix::op[]: index out of range");
  return _val[idx];
}

// modificateurs ============================================================

template <typename T>
T& Vect_fix<T>::operator[] (std::ptrdiff_t idx) {
  if (std::size_t(idx) >= DIM)
    throw std::domain_error("Vect_fix::op[]: index out of range");
  return _val[idx];
}

//copy, transfert, etc
template <typename T>
T* Vect_fix<T>::_cp (const Vect_fix<T>& v) {
  T *res = new T[v.DIM];
  for (std::size_t i = 0; i < v.DIM; ++i) res[i] = v._val[i];
  return res;
}

template <typename T>
Vect_fix<T>& Vect_fix<T>::operator= (const Vect_fix& v) noexcept {
  if (this != &v) {delete[] _val; DIM = v.DIM; _val = _cp(v);}
  return *this;
}

template <typename T>
Vect_fix<T>& Vect_fix<T>::operator= (Vect_fix&& v) noexcept {
  if (this != &v) {
    delete[] _val; DIM = v.DIM; _val = v._val;
    v.DIM = 0; v._val = nullptr;
  }
  return *this;
}

// fonctions externes =======================================================

template <typename T>
inline std::ostream& operator<< (std::ostream& out, const Vect_fix<T>& v){
  std::ptrdiff_t i;
  out << "[ "; for (i = 0; i<std::ptrdiff_t (v.dim()); ++i){v._val[i]._dsp(out); out << ' ';}  out << ']';
  return out;
}



#endif // _VECT_FIX_H_
