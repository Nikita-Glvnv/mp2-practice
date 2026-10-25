#include "tmatrix.h"

template <typename T>
TDynamicVector<T>::TDynamicVector(const TDynamicVector& v)
{
    sz = v.sz;
    pMem = new T[sz]();
    for (int i = 0; i < sz; i++) {
        pMem[i] = v.pMem[i];
    }
}

template <typename T>
TDynamicVector<T>::TDynamicVector(TDynamicVector&& v) noexcept
{
    sz = v.sz;
    pMem = v.pMem;
    v.pMem = NULL;
    ~v;
}
