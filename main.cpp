#include "fixed_point.hpp"
#include <iostream>

template<size_t dp>
constexpr int32_t double_to_fixed(double d) {
  return int32_t(d * double(1 << dp) + (d >= 0 ? 0.5 : -0.5));
}

template<size_t dp>
constexpr double fixed_to_double(int32_t f) {
  return double(f) / double(1 << dp);
}

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
