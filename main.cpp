#include <chrono>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <cstdint>

template<size_t dp>
constexpr int32_t double_to_fixed(double d) {
  return int32_t(d * double(1 << dp) + (d >= 0 ? 0.5 : -0.5));
}

template<size_t dp>
constexpr double fixed_to_double(int32_t f) {
  return double(f) / double(1 << dp);
}

template<size_t size, size_t dp>
class FixedPoint {
  union _Types {
    struct {
      int64_t d1: size;
    };
    struct {
      int64_t d2: size*2;
    };
  };
  _Types value{0};
public:
  constexpr FixedPoint() = default;
  constexpr FixedPoint(const double d) {
    value.d1 = int64_t(d * double(1 << dp) + (d >= 0 ? 0.5 : -0.5));
  }
  constexpr operator double() const {
    return double(value.d1) / double(1 << dp);
  }
private:
  static constexpr FixedPoint from(_Types v) { FixedPoint k; k.value = v; return k; }
  static constexpr FixedPoint from(int64_t v) { FixedPoint k; k.value.d1 = v; return k; }
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
};

int main() {
  using fp32_16 = FixedPoint<32, 16>;
  constexpr fp32_16 a{3.5};
  constexpr fp32_16 b{4.83};
  constexpr fp32_16 p = a + b;
  constexpr fp32_16 m = a * b;
  constexpr fp32_16 di = a / b;
  constexpr auto d1 = double(p);
  constexpr auto d2 = double(m);
  constexpr auto d3 = double(di);
  std::cout << d3 << std::endl;
}
