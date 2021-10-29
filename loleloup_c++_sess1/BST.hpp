/* GNU C++ version 10.2 - "g++ -std=c++17"
 * Yves Roggeman - 2020/11 - <BST.hpp>
 * ADT générique d'arbre binaire de recherche polymorphe
 * Autorise les données synonymes (dans l'odre d'insertion)
 * Copies et transferts possibles
 */
#ifndef _BST_H_
#define _BST_H_

#include  <cstddef>             // nullptr_t, size_t, ptrdiff_t, byte...
#include  <ostream>             // output streams

template <typename T>
class BST {
protected:
  static const T _NOT_FOUND;                      // "not found" element
  // Implementation
  struct _Node;
  _Node *_root = nullptr;
  // Utilities
  inline _Node*& _findNode (const T&) noexcept;   // ready for insert or erase
  inline _Node*& _nextLeaf (const T&) noexcept;   // always null (for insert)
  static inline _Node* _erase (_Node*&) noexcept; // ready for delete
  virtual void _dsp (std::ostream&) const;        // Recursive
private:
  static _Node* _cp (const _Node* r) noexcept     // idem
    {return r ? new _Node(*r) : nullptr;}
public:
  // Constructors
  constexpr BST () noexcept = default;            // Empty tree
  // Getters
  static constexpr bool isNotFound (const T& v) noexcept
    {return &v == &_NOT_FOUND;}
  constexpr bool isEmpty () const noexcept {return !_root;}
  virtual const T& find (const T&) const noexcept;
  bool exists (const T& v) const noexcept {return !isNotFound(find(v));}
  // Traversal
  template <typename Fct, typename... Args>
  inline void traverse (Fct, Args...) const;
  // Setters
  virtual const T& insert (const T& v)            // always add
    {return (_nextLeaf(v) = new _Node(v))->info;}
  virtual bool erase (const T& v)                 // false if doesn't exist
    {_Node *res = _erase(_findNode(v)); delete res; return res;}
  // Copies & transfers
  BST (const BST& t) noexcept: _root(_cp(t._root)) {}
  constexpr BST (BST&& t) noexcept: _root(t._root) {t._root = nullptr;}
  virtual BST& operator= (const BST&);
  virtual BST& operator= (BST&&);
  // Destructor
  virtual ~BST () noexcept {delete _root;}        // recursive
  // Associated function
  template <typename U>
  friend inline std::ostream& operator<< (std::ostream&, const BST<U>&);
}; // BST<T>

template <typename T>
const T BST<T>::_NOT_FOUND{};

// Embedded class _Node ======================================================

template <typename T>
struct BST<T>::_Node {
  const T info;
  BST left{}, right{};
  explicit _Node (const T& v) noexcept: info(v) {}
};

// Getters ===================================================================

template <typename T>
template <typename Fct, typename... Args>
void BST<T>::traverse (Fct f, Args... args) const {
  if (_root) {
    _root->left.traverse(f, args...);
    f(_root->info, args...);
    _root->right.traverse(f, args...);
  }
}

template <typename T>
void BST<T>::_dsp (std::ostream& out) const {
#ifdef  _BST_DBG_
  if (_root) {
    out << '('; _root->left._dsp(out);
    out << _root->info;
    _root->right._dsp(out); out << ')';
  }
#else   // _BST_DBG_
  traverse([&out](const T& v){out << v << ' ';});
#endif  // _BST_DBG_
}

template <typename T>
const T& BST<T>::find (const T& v) const noexcept {
  const _Node *const res = const_cast<BST*>(this)->_findNode(v);
  return res ? res->info : _NOT_FOUND;
}

// Setters ===================================================================

template <typename T>
typename BST<T>::_Node*& BST<T>::_findNode (const T& v) noexcept {
  _Node **res = &_root;
  while (*res)
    if (v == (*res)->info) break;
    else res = v < (*res)->info ? &(*res)->left._root : &(*res)->right._root;
  return *res;   //  pointer to place where v is or should be
}

template <typename T>
typename BST<T>::_Node*& BST<T>::_nextLeaf (const T& v) noexcept {
  _Node **res = &_root;
  while (*res)
    res = v < (*res)->info ? &(*res)->left._root : &(*res)->right._root;
  return *res;
}

template <typename T>
typename BST<T>::_Node* BST<T>::_erase (_Node*& target) noexcept {
  _Node *const res = target; // saved
  if (target) {
    _Node *subst = target->right._root;
    if (subst) {
      _Node *father = nullptr;
      while (subst->left._root) {father = subst; subst = subst->left._root;}
      if (father) {
        father->left._root = subst->right._root;
        subst->right._root = target->right._root;
      }
      subst->left._root  = target->left._root;
    } else subst = target->left._root;
    target->left._root = nullptr; target->right._root = nullptr;
    target = subst;
  }
  return res;   // old isolated _Node (not yet deleted)
}

// Copies & transfers ========================================================

template <typename T>
BST<T>& BST<T>::operator= (const BST& t) {
  if (this != &t) {delete _root; _root = _cp(t._root);}
  return *this;
}

template <typename T>
BST<T>& BST<T>::operator= (BST&& t) {
  if (this != &t) {delete _root; _root = t._root; t._root = nullptr;}
  return *this;
}

// associated function =======================================================

template <typename T>
inline std::ostream& operator<< (std::ostream& out, const BST<T>& t)
  {out << "[ "; t._dsp(out); out << ']'; return out;}

#endif // _BST_H_
