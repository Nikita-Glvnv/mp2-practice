// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <bitset>
#include <stdexcept>

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) throw std::out_of_range("Fieldlength OUR");
    BitLen = len;
    MemLen = (len + 31) / 32;
    pMem = new TELEM[MemLen]();
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n>>5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1u << (n & 31);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("setbit OUR");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("ClrBit OUR");
    pMem[GetMemIndex(n)] &= ~(GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) throw std::out_of_range("GetBit OUR");
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}

// битовые операции

const TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;
    if (MemLen != bf.MemLen) {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    BitLen = bf.BitLen;
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)return 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)return 1;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 1;
        }
    }
    return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField ans(max(BitLen, bf.BitLen));
    if (BitLen < bf.BitLen) {
        for (int i = 0; i < bf.MemLen; i++) {
            ans.pMem[i] = bf.pMem[i];
        }
        for (int i = 0; i < MemLen; i++) {
            ans.pMem[i] = ans.pMem[i] | pMem[i];
        }
    }
    else {
        for (int i = 0; i < MemLen; i++) {
            ans.pMem[i] = pMem[i];
        }
        for (int i = 0; i < bf.MemLen; i++) {
            ans.pMem[i] = pMem[i] | bf.pMem[i];
        }
    }
    return ans;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField ans(max(BitLen, bf.BitLen));
    if (BitLen < bf.BitLen) {
        for (int i = 0; i < bf.MemLen; i++) {
            ans.pMem[i] = 0;
        }
        for (int i = 0; i < MemLen; i++) {
            ans.pMem[i] = bf.pMem[i] & pMem[i];
        }
    }
    else {
        for (int i = 0; i < MemLen; i++) {
            ans.pMem[i] = 0;
        }
        for (int i = 0; i < bf.MemLen; i++) {
            ans.pMem[i] = pMem[i] & bf.pMem[i];
        }
    }
    return ans;
}

TBitField TBitField::operator~(void)
{
    TBitField ans(BitLen);
    for (int i = 0; i < MemLen; i++) {
        ans.pMem[i] = ~pMem[i];
    }
    int rem = BitLen & 31;
    if (rem != 0) {
        ans.pMem[MemLen - 1] &= (1u << rem) - 1;
    }
    return ans;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.MemLen; i++) {
        istr >> bf.pMem[i];
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.MemLen; i++) {
        ostr << std::bitset < 32>(bf.pMem[i]) << " ;; ";
    }
    return ostr;
}
