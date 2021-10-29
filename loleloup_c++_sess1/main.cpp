#include <iostream>
#include "Cont.hpp"
#include "Complex.h"


template <typename T>
T Whatami(BST<T> *b, T i){
  b->insert(i);
  return b->find(i);
}

template <typename T>
T Whatamiconst(const BST<T> *b, T i){
  return b->find(i);
}

template <typename T>
void vectortest(Vect_fix<T> &v){
  std::cout << "vect non const index 0 = " << v[0] << std::endl;
}


template <typename T>
void vectortestconst(const Vect_fix<T> &v){
  std::cout << "vect const index 0 = "<< v[0] << std::endl;
}


template<typename T>
void Contstate(Cont<T> &c){
  std::cout << "State of the cont : size = " << c.dim() << ", content = " << c << ", used = " << c.used() << std::endl;
}


int main() {

  Cont<int> Test_cont(5);
  Cont<int> p(10);

  std::cout << "Cont vides : \n";
  Contstate(p);
  Contstate(Test_cont);

  p.insert({0, 83});
  p.insert({7, 3});
  p.insert({0, 666});
  //p.insert({2, 666});  //erreur doublons
  p.insert({3, 1});

  std::cout << "Apres insertions dans p : \n";
  Contstate(p);
  Contstate(Test_cont);

  Test_cont = p;  //copie avec modification taille Test_cont.dim() = 5 deviens 10

  std::cout << "Apres copie de p dans test cont et modifications : \n";
  Test_cont.insert({2, 64});  //modifie index 2 = 3
  Test_cont.insert({0, 999});
  Contstate(p);
  Contstate(Test_cont);

  std::cout << "Passe pour BST display 45 : " << Whatami(&Test_cont, {2, 45}) << "\n"; //polymorphisme BST
  std::cout << "Passe pour BST const display 999 : " << Whatamiconst(&Test_cont, {0, 999}) << std::endl;  //test BST const
  vectortest(p);
  vectortestconst(p);

  //Class complex
  Complex CX = Complex(5, 10);
  Cont<Complex> Test_cont_c(5);
  Test_cont_c.insert({0, Complex(20, 40)});  //same
  Test_cont_c.insert({1, CX});  //same
  std::cout << "Création d'une container de classe d'un type programmé : \n";
  std::cout << Test_cont_c << std::endl;

  return 0;
}
