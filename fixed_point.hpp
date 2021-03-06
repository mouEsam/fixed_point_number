#pragma once 

#include <cstddef>
#include <cstdint>

#if defined(__clang__) || defined(__GNUC__)
typedef __int128_t int128_t;
#else
typedef long long int128_t;
#endif

template<size_t size, size_t dp>
class FixedPoint {
  static_assert(size <= sizeof(int128_t) * 8);
  static_assert(size > dp);
  static inline const bool _IsValidExtended = size <= sizeof(int128_t) * 4;
  #pragma pack(push, 1)
  struct _Storage {
    int128_t d: size;
  };
  struct _ExtendedStorage {
    int128_t d: (size <= sizeof(int128_t) * 8 / 2) ? size*2 : size;
  };
  #pragma pack(pop)

  _Storage value{0};
  constexpr _ExtendedStorage _extended() const {
    _ExtendedStorage t;
    t.d = value.d;
    return t;
  }
public:
  constexpr FixedPoint() = default;
  constexpr FixedPoint(const double& d) {
      set(d);
  }
  constexpr operator double() const {
    return double(value.d) / double(1LL << dp);
  }
private:
  static constexpr FixedPoint from(_Storage v) { 
    FixedPoint k; 
    k.value = v; 
    return k; 
  }
  static constexpr FixedPoint from(int128_t v) { 
    FixedPoint k; 
    k.value.d = v; 
    return k; 
  }
public:
  constexpr FixedPoint& operator = (const FixedPoint& f) = default;
  
  constexpr FixedPoint& operator = (const double& d) {
     this->set(d);
     return *this;
  }
  constexpr void set(const double& d) {
      this->value.d = int128_t(d * double(1LL << dp) + (d >= 0 ? 0.5 : -0.5));
  }
  constexpr FixedPoint operator - () const {
    return from(-this->value.d);
  }
  constexpr FixedPoint operator + (const FixedPoint& f) const {
    return from(this->value.d + f.value.d);
  }
  constexpr FixedPoint& operator += (const FixedPoint& f) {
    this->value.d += f.value.d;
    return *this;
  }
  constexpr FixedPoint operator - (const FixedPoint& f) const {
    return from(this->value.d - f.value.d);
  }
  constexpr FixedPoint& operator -= (const FixedPoint& f) {
    this->value.d -= f.value.d;
    return *this;
  }
  FixedPoint operator * (const FixedPoint& f) const requires(!_IsValidExtended) {
      auto t = _extended();
      auto t2 = f._extended();
      return from((t.d * t2.d) >> dp);
  }
  constexpr FixedPoint operator * (const FixedPoint& f) const {
    auto t = _extended();
    auto t2 = f._extended();
    return from((t.d * t2.d) >> dp);
  }
  FixedPoint& operator *= (const FixedPoint& f) requires(!_IsValidExtended) {
      auto t = _extended();
      auto t2 = f._extended();
      this->value.d = (t.d * t2.d) >> dp;
      return *this;
  }
  constexpr FixedPoint& operator *= (const FixedPoint& f) {
    auto t = _extended();
    auto t2 = f._extended();
    this->value.d = (t.d * t2.d) >> dp;
    return *this;
  }
  constexpr FixedPoint operator / (const FixedPoint& f) const requires(!_IsValidExtended) {
      auto t = double(*this);
      auto t2 = double(f);
      return { t / t2 };
  }
  constexpr FixedPoint operator / (const FixedPoint& f) const {
      auto t = _extended();
      auto t2 = f._extended();
      return from((t.d << dp) / t2.d);
  }
  constexpr FixedPoint& operator /= (const FixedPoint& f) requires(!_IsValidExtended) {
      auto t = double(*this);
      auto t2 = double(f);
      *this = t / t2;
      return *this;
  }
  constexpr FixedPoint& operator /= (const FixedPoint& f) {
      auto t = _extended();
      auto t2 = f._extended();
      this->value.d = (t.d << dp) / t2.d;
    return *this;
  }
};
