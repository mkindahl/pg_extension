// -*- Mode: C++ -*-

#ifndef PGEXT_LIST_H_
#define PGEXT_LIST_H_

extern "C" {
#include <postgres.h>

#include <utils/array.h>
}

#include <iterator>

namespace pgext {
template <class Type>
class ArrayBase {
 public:
  // TODO(matski): This needs to be a nullable type. Make sure only
  // nullable types are allowed.
  using value_type = Type;
  using pointer = value_type *;
  using reference = value_type &;
  using size_type = size_t;

  class iterator {
    friend class ArrayBase;

    friend bool operator==(const iterator &lhs, const iterator &rhs) {
      return lhs.iter_ == rhs.iter_;
    }

    friend bool operator!=(const iterator &lhs, const iterator &rhs) {
      return lhs.iter_ != rhs.iter_;
    }

   public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = Type;
    using pointer = value_type *;
    using reference = value_type &;

    // Could be good to require a smart pointer here to ensure that
    // the underlying storage does not go away, but that is currently
    // not well-supported by PostgreSQL.
    iterator(ArrayType *array) : iter_(array_create_iterator(array, 0, NULL)) {
      array_iterate(iter_, &value_, &isnull_);
    }

    reference operator*() const { return value_type(value_, isnull_); }

    iterator &operator++() {
      array_iterate(iter_, &value_, &isnull_);
      return *this;
    }

    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

   private:
    iterator() : iter_(nullptr) {}

    ArrayIterator iter_;
    Datum value_;
    bool isnull_;
  };

#if __cplusplus >= 201103L
  ArrayBase() = default;
#else
  ArrayBase() {}
#endif

  iterator begin() { return iterator(array_); }
  iterator end() { return iterator(); }

 private:
  ArrayType *array_;
};

template <class Type>
class Array : public ArrayBase<Type> {
 public:
  Array(ArrayType *arr);

 private:
};

}  // namespace pgext

#endif  // PGEXT_LIST_H_
