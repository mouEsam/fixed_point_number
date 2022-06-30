#pragma once 

#include <cstddef>
#include <cstdint>

typedef int int128_t __attribute__((mode(TI)));

template<size_t size, size_t dp>
class FixedPoint {

  #pragma pack(push, 1)
  union _Types {
    int128_t d1: size;
    int128_t d2: size*2;
  };
  #pragma pack(pop)

  _Types value{0};
  constexpr _Types _extended() const {
    _Types t;
    t.d2 = value.d1;
    return t;
  }
public:
  constexpr FixedPoint() = default;
  constexpr FixedPoint(const double d) {
    value.d1 = int128_t(d * double(1LL << dp) + (d >= 0 ? 0.5 : -0.5));
  }
  constexpr operator double() const {
    return double(value.d1) / double(1LL << dp);
  }
private:
  static constexpr FixedPoint from(_Types v) { 
    FixedPoint k; 
    k.value = v; 
    return k; 
  }
  static constexpr FixedPoint from(int128_t v) { 
    FixedPoint k; 
    k.value.d1 = v; 
    return k; 
  }
public:
  constexpr FixedPoint& operator = (const FixedPoint& f) = default;
  constexpr FixedPoint operator - () const {
    return from(-this->value.d1);
  }
  constexpr FixedPoint operator + (const FixedPoint& f) const {
    return from(this->value.d1 + f.value.d1);
  }
  constexpr FixedPoint& operator += (const FixedPoint& f) {
    this->value.d1 += f.value.d1;
    return *this;
  }
  constexpr FixedPoint operator - (const FixedPoint& f) const {
    return from(this->value.d1 - f.value.d1);
  }
  constexpr FixedPoint& operator -= (const FixedPoint& f) {
    this->value.d1 -= f.value.d1;
    return *this;
  }
  constexpr FixedPoint operator * (const FixedPoint& f) const {
    _Types t = _extended();
    _Types t2 = f._extended();
    return from((t.d2 * t2.d2) >> dp);
  }
  constexpr FixedPoint& operator *= (const FixedPoint& f) {
    _Types t = _extended();
    _Types t2 = f._extended();
    this->value.d1 = (t.d2 * t2.d2) >> dp;
    return *this;
  }
  constexpr FixedPoint operator / (const FixedPoint& f) const {
    _Types t = _extended();
    _Types t2 = f._extended();
    return from((t.d2 << dp) / t2.d2);
  }
  constexpr FixedPoint& operator /= (const FixedPoint& f) {
    _Types t = _extended();
    _Types t2 = f._extended();
    this->value.d1 = (t.d2 << dp) / t2.d2;
    return *this;
  }
};
