// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

typedef unsigned int TELEM;
constexpr int BIT_IN_ELEM = sizeof(TELEM) * 8;

TBitField::TBitField(int len) : BitLen(len) {
    if (len < 0) {
        throw exception("");
    }
    MemLen = ceil((double)BitLen / BIT_IN_ELEM);
    pMem = new TELEM[MemLen]();
}

TBitField::TBitField(const TBitField& other) : BitLen(other.BitLen), MemLen((other.MemLen)) {
    pMem = new TELEM[MemLen];
    std::copy(other.pMem, other.pMem + MemLen, pMem);
}

TBitField::~TBitField() {
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / BIT_IN_ELEM;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{ 
   return (1u << (n % BIT_IN_ELEM));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen) {
        throw exception("");
    }
    TELEM mask = GetMemMask(n);
    int id = GetMemIndex(n);
    pMem[id] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen) {
        throw exception("");
    }
    int id = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[id] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen) {
        throw exception("");
    }
    int id = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return (pMem[id] & mask) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &other) // присваивание
{
    if (this != &other) {
        if (MemLen != other.MemLen) {
            delete pMem;
            MemLen = other.MemLen;
            pMem = new TELEM[MemLen];
        }
        BitLen = other.BitLen;
        std::copy(other.pMem, other.pMem + MemLen, pMem);
    }
    return *this;
}

bool TBitField::operator==(const TBitField &other) const // сравнение
{
    if (BitLen != other.BitLen)
        return false;

    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != other.pMem[i])
            return false;
    }
    return true;
}

bool TBitField::operator!=(const TBitField &other) const // сравнение
{
  return !(*this == other);
}

TBitField TBitField::operator|(const TBitField &other) // операция "или"
{
    int maxLen = std::max(BitLen, other.BitLen);
    TBitField result(maxLen);

    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = pMem[i];

    for (int i = 0; i < other.MemLen; i++)
        result.pMem[i] |= other.pMem[i];

    return result;
}

TBitField TBitField::operator&(const TBitField &other) // операция "и"
{
    int maxLen = std::max(BitLen, other.BitLen);
    TBitField result(maxLen);

    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = pMem[i];

    for (int i = 0; i < other.MemLen; i++)
        result.pMem[i] &= other.pMem[i];

    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < BitLen; i++)
        GetBit(i) == 0 ? res.SetBit(i) : res.ClrBit(i);

    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    char ch;
    for (size_t i = 0; i < bf.BitLen; i++)
    {
        istr >> ch;
        switch (ch)
        {
        case '0':
            bf.ClrBit(i);
            break;
        case '1':
            bf.SetBit(i);
            break;
        default:
            continue;
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        ostr << (bf.GetBit(i) ? 1 : 0);
    return ostr;
}
