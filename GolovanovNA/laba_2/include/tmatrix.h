// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>

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
  TDynamicVector(size_t size = 1) : sz(size)
  {
    if (sz == 0)
      throw out_of_range("Vector size should be greater than zero");
    pMem = new T[sz]();// {}; // У типа T д.б. констуктор по умолчанию
  }
  TDynamicVector(T* arr, size_t s) : sz(s)
  {
    assert(arr != nullptr && "TDynamicVector ctor requires non-nullptr arg");
    pMem = new T[sz];
    std::copy(arr, arr + sz, pMem);
  }
  TDynamicVector(const TDynamicVector& v);
  TDynamicVector(TDynamicVector&& v) noexcept;
  ~TDynamicVector() {
      delete[]pMem;
  }
  TDynamicVector& operator=(const TDynamicVector& v) {
      if (this == &v)
          return *this;

      if (sz != v.sz)
      {
          T* newMem = new T[v.sz];
          delete[] pMem;
          pMem = newMem;
          sz = v.sz;
      }

      for (size_t i = 0; i < sz; i++)
      {
          pMem[i] = v.pMem[i];
      }

      return *this;
  }
  TDynamicVector& operator=(TDynamicVector&& v) noexcept
  {
      if (this == &v)
          return *this;
      sz = v.sz;
      delete[]pMem;
      pMem = v.pMem;
      v.sz = 0;
      v.pMem = nullptr;
      return *this;
  }

  // индексация
  T& operator[](size_t ind)
  {
      return pMem[ind];
  }
  const T& operator[](size_t ind) const
  {
      return pMem[ind];
  }
  // индексация с контролем
  T& at(size_t ind)
  {
      if (ind < 0 && ind > sz) {
          throw std::invalid_argument("invalid index");
      }
      return pMem[ind];
  }
  const T& at(size_t ind) const
  {
      if (ind < 0 && int > sz) {
          throw std::invalid_argument("invalid index");
      }
      return pMem[ind];
  }

  // сравнение
  bool operator==(const TDynamicVector& v) const noexcept
  {
      if (sz != v.sz) return false;
      for (int i = 0; i < sz; i++) {
          if (pMem[i] != v.pMem[i]) return false;
      }
      return true;
  }
  bool operator!=(const TDynamicVector& v) const noexcept
  {
      if (*this == v)return false;
      return true;
  }

  // скалярные операции
  TDynamicVector operator+(T val)
  {
     TDynamicVector<T> v(*this);
     for (int i = 0; i < v.sz; i++) {
          v.pMem[i] += val;
     }
     return v;
  }
  TDynamicVector operator-(T val)
  {
      TDynamicVector<T> v(*this);
      for (int i = 0; i < v.sz; i++) {
          v.pMem[i] -= val;
      }
      return v;
  }
  TDynamicVector operator*(T val)
  {
      TDynamicVector<T> v(*this);
      for (int i = 0; i < v.sz; i++) {
          v.pMem[i] *= val;
      }
      return v;
  }

  // векторные операции
  TDynamicVector operator+(const TDynamicVector& v)
  {
      if (sz != v.sz) throw std::invalid_argument("different length vectors in +");
      TDynamicVector<T> res(sz);
      for (int i = 0; i < v.sz; i++) {
          res.pMem[i] = pMem[i] + v.pMem[i];
      }
      return res;
  }
  TDynamicVector operator-(const TDynamicVector& v)
  {
      if (sz != v.sz) throw std::invalid_argument("different length vectors in -");
      TDynamicVector<T> res(sz);
      for (int i = 0; i < v.sz; i++) {
          res.pMem[i] = pMem[i] - v.pMem[i];
      }
      return res;
  }
  T operator*(const TDynamicVector& v) noexcept(noexcept(T()))
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
    for (size_t i = 0; i < v.sz; i++)
      istr >> v.pMem[i]; // требуется оператор>> для типа T
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
public:
  TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s)
  {
    for (size_t i = 0; i < sz; i++)
      pMem[i] = TDynamicVector<T>(sz);
  }

  using TDynamicVector<TDynamicVector<T>>::operator[];
  using TDynamicVector<TDynamicVector<T>>::size;

  // сравнение
  bool operator==(const TDynamicMatrix& m) const noexcept
  {
      if (sz != m.sz) return false;
      for (size_t i = 0; i < sz; i++){
          if (pMem[i] != m.pMem[i])
              return false;
      }
      return true;
  }

  // матрично-скалярные операции
  TDynamicMatrix operator*(const T& val)
  {
      TDynamicMatrix res(sz);
      for (size_t i = 0; i < sz; i++){
          res.pMem[i] = pMem[i] * val;
      }
      return res;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T>& v)
  {
      if (sz != v.size()) throw invalid_argument("Different sizes");
      TDynamicVector<T> res(sz);
      for (size_t i = 0; i < sz; i++){
          res[i] = pMem[i] * v;
      }
      return res;
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix& m)
  {
      if (sz != m.sz) throw std::invalid_argument("different length matrix in m+m");
      TDynamicMatrix ans(sz);
          for (size_t i = 0; i < sz; i++) {
              ans.pMem[i] = pMem[i] + m.pMem[i];
          }
      return ans;
  }
  TDynamicMatrix operator-(const TDynamicMatrix& m)
  {
      if (sz != m.sz) throw std::invalid_argument("different length matrix in m-m");
      TDynamicMatrix ans(sz);
          for (size_t i = 0; i < sz; i++) {
              ans.pMem[i] = pMem[i] - m.pMem[i];
          }
      return ans;
  }
  TDynamicMatrix operator*(const TDynamicMatrix& m)
  {
      if (sz != m.sz) throw std::invalid_argument("different length matrix in m*m");
      TDynamicMatrix ans(sz);
      for (int i = 0; i < sz; i++) {
          for (int j = 0; j < sz; j++) {
              for (int k = 0; k < sz; k++) {
                  ans.pMem[i][j] += pMem[i][k] * m.pMem[k][j];
              }
          }
      }
      return ans;
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicMatrix& v)
  {
      for (int i = 0; i < v.sz; i++) {
          for (int j = 0; j < v.sz; j++) {
              istr >> v.pMem[i][j];
          }
      }
      return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& v)
  {
      ostr << "[ ";
      for (int i = 0; i < v.sz; i++) {
          for (int j = 0; j < v.sz; j++) {
              ostr << v.pMem[i][j] << " ";
          }
          ostr << "\n";
      }
      ostr << " ]";
      return ostr;
  }
};

#endif