/* GNU C++ version 10.2 - "g++ -std=c++17"
 * Leloup Loïc - 2020/09 - <Complex.h>
 * classe de nombre complexe;
 * supporte les évaluations strictes == et <;
 */
#ifndef INFOF202_COMPLEX_H
#define INFOF202_COMPLEX_H

#include <ostream>

class Complex{
  int _im;
  int _real;

  void _dsp(std::ostream&) const;
  friend inline std::ostream& operator<< (std::ostream&, const Complex&);
public:
    inline Complex() noexcept = default;
    inline Complex(int Im, int Real) noexcept : _im(Im), _real(Real){};
    Complex(const Complex&) noexcept = default;
    Complex(Complex&&) noexcept = default;
    ~Complex() noexcept = default;


    inline bool constexpr operator== (const Complex&) const;
    inline bool constexpr operator< (const Complex&) const;


};

void Complex::_dsp(std::ostream& out) const{
  out << _real << ", " << _im;
}

bool constexpr Complex::operator== (const Complex& c) const{
  return _real == c._real && _im == c._im;
}


bool constexpr Complex::operator< (const Complex& c) const{
  return _real + _im < c._real + c._im; //not mathematically correct indeed
}


inline std::ostream& operator<< (std::ostream& out, const Complex& c){
  out << "( ";
  c._dsp(out);
  out << " )";
  return out;
}

#endif //INFOF202_COMPLEX_H
