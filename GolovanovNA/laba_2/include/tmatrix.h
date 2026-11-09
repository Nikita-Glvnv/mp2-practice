// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <utility>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Динамический вектор - 
// шаблонный вектор на динамической памяти
template<typename T>
class TDynamicVector
{
protected:
  size_t sz;
  T* pMem;
public:
  size_t size() const noexcept
  {
      return sz;
  }
  TDynamicVector(size_t size = 1) : sz(size), pMem(nullptr)
  {
    if (sz == 0 || sz > static_cast<size_t>(MAX_VECTOR_SIZE))
      throw out_of_range("Invalid vector size");
    pMem = new T[sz]();
  }
  TDynamicVector(T* arr, size_t s) : sz(s), pMem(nullptr)
  {
    if (sz == 0 || sz > static_cast<size_t>(MAX_VECTOR_SIZE))
      throw out_of_range("Invalid vector size");
    if (arr == nullptr)
      throw invalid_argument("Null source array");

    T* newMem = new T[sz]();
    try {
      std::copy(arr, arr + sz, newMem);
    }
    catch (...) {
      delete[] newMem;
      throw;
    }
    pMem = newMem;
  }
  TDynamicVector(const TDynamicVector& v) : sz(0), pMem(nullptr)
  {
    if (v.sz == 0) return;
    T* newMem = new T[v.sz]();
    try {
      for (size_t i = 0; i < v.sz; i++)
        newMem[i] = v.pMem[i];
    }
    catch (...) {
      delete[] newMem;
      throw;
    }
    pMem = newMem;
    sz = v.sz;
  }
  TDynamicVector(TDynamicVector&& v) noexcept : sz(v.sz), pMem(v.pMem)
  {
    v.sz = 0;
    v.pMem = nullptr;
  }
  ~TDynamicVector()
  {
    delete[] pMem;
  }
  TDynamicVector& operator=(const TDynamicVector& v)
  {
    if (this == &v)
      return *this;

    T* newMem = nullptr;
    if (v.sz != 0) {
      newMem = new T[v.sz]();
      try {
        for (size_t i = 0; i < v.sz; i++)
          newMem[i] = v.pMem[i];
      }
      catch (...) {
        delete[] newMem;
        throw;
      }
    }
    delete[] pMem;
    pMem = newMem;
    sz = v.sz;
    return *this;
  }
  TDynamicVector& operator=(TDynamicVector&& v) noexcept
  {
    if (this == &v)
      return *this;
    delete[] pMem;
    sz = v.sz;
    pMem = v.pMem;
    v.sz = 0;
    v.pMem = nullptr;
    return *this;
  }
  // индексация
  T& operator[](size_t ind)
  {
    if (ind >= sz)
      throw out_of_range("Invalid vector index");
    return pMem[ind];
  }
  const T& operator[](size_t ind) const
  {
    if (ind >= sz)
      throw out_of_range("Invalid vector index");
    return pMem[ind];
  }
  // индексация с контролем
  T& at(size_t ind)
  {
    if (ind >= sz)
      throw out_of_range("Invalid vector index");
    return pMem[ind];
  }
  const T& at(size_t ind) const
  {
    if (ind >= sz)
      throw out_of_range("Invalid vector index");
    return pMem[ind];
  }
  // сравнение
  bool operator==(const TDynamicVector& v) const
  {
      if (sz != v.sz) return false;
      for (size_t i = 0; i < sz; i++) {
          if (pMem[i] != v.pMem[i]) return false;
      }
      return true;
  }
  bool operator!=(const TDynamicVector& v) const
  {
      if (*this == v)return false;
      return true;
  }

  // скалярные операции
  TDynamicVector operator+(T val) const
  {
     TDynamicVector<T> v(*this);
     for (size_t i = 0; i < v.sz; i++) {
          v.pMem[i] += val;
     }
     return v;
  }
  TDynamicVector operator-(T val) const
  {
      TDynamicVector<T> v(*this);
      for (size_t i = 0; i < v.sz; i++) {
          v.pMem[i] -= val;
      }
      return v;
  }
  TDynamicVector operator*(T val) const
  {
      TDynamicVector<T> v(*this);
      for (size_t i = 0; i < v.sz; i++) {
          v.pMem[i] *= val;
      }
      return v;
  }

  // векторные операции
  TDynamicVector operator+(const TDynamicVector& v) const
  {
      if (sz != v.sz) throw std::invalid_argument("different length vectors in +");
      TDynamicVector<T> res(sz);
      for (size_t i = 0; i < v.sz; i++) {
          res.pMem[i] = pMem[i] + v.pMem[i];
      }
      return res;
  }
  TDynamicVector operator-(const TDynamicVector& v) const
  {
      if (sz != v.sz) throw std::invalid_argument("different length vectors in -");
      TDynamicVector<T> res(sz);
      for (size_t i = 0; i < v.sz; i++) {
          res.pMem[i] = pMem[i] - v.pMem[i];
      }
      return res;
  }
  T operator*(const TDynamicVector& v) const
  {
      if (sz != v.sz) throw invalid_argument("Different vector sizes");
      T res = T();
      for (size_t i = 0; i < sz; i++){
          res += pMem[i] * v.pMem[i];
      }
      return res;
  }
  

  friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
  {
    std::swap(lhs.sz, rhs.sz);
    std::swap(lhs.pMem, rhs.pMem);
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++) {
      if (!(istr >> v.pMem[i]))
        break;
    }
    return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      ostr << v.pMem[i] << ' '; // требуется оператор<< для типа T
    return ostr;
  }
};


// Динамическая матрица - 
// шаблонная матрица на динамической памяти
template<typename T>
class TDynamicMatrix : private TDynamicVector<TDynamicVector<T>>
{
  using TDynamicVector<TDynamicVector<T>>::pMem;
  using TDynamicVector<TDynamicVector<T>>::sz;

  static size_t CheckMatrixSize(size_t s)
  {
    if (s == 0 || s > static_cast<size_t>(MAX_MATRIX_SIZE))
      throw out_of_range("Invalid matrix size");
    return s;
  }
public:
  TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(CheckMatrixSize(s))
  {
    for (size_t i = 0; i < sz; i++)
      pMem[i] = TDynamicVector<T>(sz);
  }

  TDynamicMatrix(const TDynamicMatrix& m)
    : TDynamicVector<TDynamicVector<T>>(m.sz == 0 ? 1 : m.sz)
  {
    if (m.sz == 0) {
      delete[] pMem;
      pMem = nullptr;
      sz = 0;
      return;
    }
    for (size_t i = 0; i < sz; i++)
      pMem[i] = m.pMem[i];
  }
  TDynamicMatrix(TDynamicMatrix&& m) noexcept
    : TDynamicVector<TDynamicVector<T>>(std::move(m))
  {
  }
  TDynamicMatrix& operator=(const TDynamicMatrix& m)
  {
    if (this == &m)
      return *this;

    TDynamicVector<T>* newMem = nullptr;
    if (m.sz != 0) {
      newMem = new TDynamicVector<T>[m.sz]();
      try {
        for (size_t i = 0; i < m.sz; i++)
          newMem[i] = m.pMem[i];
      }
      catch (...) {
        delete[] newMem;
        throw;
      }
    }
    delete[] pMem;
    pMem = newMem;
    sz = m.sz;
    return *this;
  }
  TDynamicMatrix& operator=(TDynamicMatrix&& m) noexcept
  {
    if (this == &m)
      return *this;
    delete[] pMem;
    sz = m.sz;
    pMem = m.pMem;
    m.sz = 0;
    m.pMem = nullptr;
    return *this;
  }

  using TDynamicVector<TDynamicVector<T>>::operator[];
  using TDynamicVector<TDynamicVector<T>>::size;
  using TDynamicVector<TDynamicVector<T>>::at;

  // сравнение
  bool operator==(const TDynamicMatrix& m) const
  {
      if (sz != m.sz) return false;
      for (size_t i = 0; i < sz; i++){
          if (pMem[i] != m.pMem[i])
              return false;
      }
      return true;
  }

  bool operator!=(const TDynamicMatrix& m) const
  {
      return !(*this == m);
  }

  // матрично-скалярные операции
  TDynamicMatrix operator*(const T& val) const
  {
      TDynamicMatrix res(sz);
      for (size_t i = 0; i < sz; i++){
          res.pMem[i] = pMem[i] * val;
      }
      return res;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T>& v) const
  {
      if (sz != v.size()) throw invalid_argument("Different sizes");
      TDynamicVector<T> res(sz);
      for (size_t i = 0; i < sz; i++){
          res[i] = pMem[i] * v;
      }
      return res;
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix& m) const
  {
      if (sz != m.sz) throw std::invalid_argument("different length matrix in m+m");
      TDynamicMatrix ans(sz);
          for (size_t i = 0; i < sz; i++) {
              ans.pMem[i] = pMem[i] + m.pMem[i];
          }
      return ans;
  }
  TDynamicMatrix operator-(const TDynamicMatrix& m) const
  {
      if (sz != m.sz) throw std::invalid_argument("different length matrix in m-m");
      TDynamicMatrix ans(sz);
          for (size_t i = 0; i < sz; i++) {
              ans.pMem[i] = pMem[i] - m.pMem[i];
          }
      return ans;
  }
  TDynamicMatrix operator*(const TDynamicMatrix& m) const
  {
      if (sz != m.sz) throw std::invalid_argument("different length matrix in m*m");
      TDynamicMatrix ans(sz);
      for (size_t i = 0; i < sz; i++) {
          for (size_t j = 0; j < sz; j++) {
              for (size_t k = 0; k < sz; k++) {
                  ans.pMem[i][j] += pMem[i][k] * m.pMem[k][j];
              }
          }
      }
      return ans;
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicMatrix& v)
  {
      for (size_t i = 0; i < v.sz; i++) {
          for (size_t j = 0; j < v.sz; j++) {
              if (!(istr >> v.pMem[i][j]))
                  return istr;
          }
      }
      return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& v)
  {
      ostr << "[ ";
      for (size_t i = 0; i < v.sz; i++) {
          for (size_t j = 0; j < v.sz; j++) {
              ostr << v.pMem[i][j] << " ";
          }
          ostr << "\n";
      }
      ostr << " ]";
      return ostr;
  }
};

#endif // __TDynamicMatrix_H__
