#include "fixed_point.hpp"
#include <cstddef>
#include <iostream>

template <size_t dp> constexpr int32_t double_to_fixed(double d) {
  return int32_t(d * double(1 << dp) + (d >= 0 ? 0.5 : -0.5));
}

template <size_t dp> constexpr double fixed_to_double(int32_t f) {
  return double(f) / double(1 << dp);
}

#ifndef _MSC_VER
#define con_constexpr constexpr
#else
#define con_constexpr
#endif

template <size_t size, size_t dp> void number() {
  using fp_t = FixedPoint<size, dp>;
  constexpr fp_t a{3.5};
  constexpr fp_t b{1.83};
  constexpr auto p = a + b;
  con_constexpr auto m = a * b;
  constexpr auto di = a / b;
  constexpr auto d1 = double(p);

  con_constexpr auto d2 = double(m);
  constexpr auto d3 = double(di);
  std::cout << d1 << " Size " << sizeof(p) << " Align " << alignof(decltype(p))
            << std::endl;
  std::cout << d2 << " Size " << sizeof(m) << " Align " << alignof(decltype(m))
            << std::endl;
  std::cout << d3 << " Size " << sizeof(di) << " Align "
            << alignof(decltype(di)) << std::endl;

  fp_t t1{2};
  fp_t t2{3};
  t1 += 40000000.0;
  std::cout << t1 << " Addr " << &t1 << " Align " << alignof(decltype(t1))
            << std::endl;
  std::cout << t2 << " Size " << &t2 << " Align " << alignof(decltype(t2))
            << std::endl;
}

int main() {
  number<8, 4>();
  number<16, 8>();
  number<32, 16>();
#ifndef _MSC_VER
  number<64, 32>();
  number<128, 32>();
#endif
}
