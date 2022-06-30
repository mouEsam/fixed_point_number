#include "fixed_point.hpp"
#include <cstddef>
#include <iostream>

template<size_t dp>
constexpr int32_t double_to_fixed(double d) {
  return int32_t(d * double(1 << dp) + (d >= 0 ? 0.5 : -0.5));
}

template<size_t dp>
constexpr double fixed_to_double(int32_t f) {
  return double(f) / double(1 << dp);
}

template<size_t size, size_t dp>
void number() {
  using fp_16 = FixedPoint<size, dp>;
  constexpr fp_16 a{3.5};
  constexpr fp_16 b{1.83};
  constexpr auto p = a + b;
  constexpr auto m = a * b;
  constexpr auto di = a / b;
  constexpr auto d1 = double(p);
  constexpr auto d2 = double(m);
  constexpr auto d3 = double(di);
  std::cout << d1 << " Size " << sizeof(p) << " Align " << alignof(decltype(p)) << std::endl;
  std::cout << d2 << " Size " << sizeof(m) << " Align " << alignof(decltype(m)) << std::endl;
  std::cout << d3 << " Size " << sizeof(di) << " Align " << alignof(decltype(di)) << std::endl;

  fp_16 t1{ 2 };
  fp_16 t2{ 3 };
  t1 += 4;
  std::cout << t1 << " Size " << sizeof(t1) << " Align " << alignof(decltype(t1)) << std::endl;
  std::cout << t2 << " Size " << sizeof(t2) << " Align " << alignof(decltype(t2)) << std::endl;
}

int main() {
  number<8, 4>();
  number<16, 8>();
  number<32, 16>();
#ifndef _MSC_VER
  number<64, 32>();
#endif // __MSVC
}
