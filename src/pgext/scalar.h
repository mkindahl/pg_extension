// -*- Mode: C++ -*-

#ifndef PGEXT_SCALAR_H_
#define PGEXT_SCALAR_H_

extern "C" {
#include <postgres.h>
}

namespace pgext {
template <class Type>
class Scalar {
  friend Scalar<Type> operator+(const Scalar<Type>& lhs,
                                const Scalar<Type>& rhs) {
    return Scalar(lhs.value() + rhs.value(), lhs.is_null() || rhs.is_null());
  }

 public:
  Scalar(Type value) : isnull_(false), value_(value) {}
  Scalar(Type value, bool isnull) : isnull_(isnull), value_(value) {}

  Type value() const { return value_; }
  bool is_null() const { return isnull_; }

 private:
  bool isnull_;
  Type value_;
};
}  // namespace pgext

#endif  // PGEXT_SCALAR_H_
