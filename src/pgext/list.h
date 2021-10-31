// -*- Mode: C++ -*-

#ifndef PGEXT_LIST_H_
#define PGEXT_LIST_H_

namespace pg {
oextern "C" {
#include <postgres.h>

#include <nodes/pg_list.h>
}
}  // namespace pg

/**
 * PostgreSQL List support.
 *
 * List extensions that allows a PostgreSQL list to be used with
 * with iterators and the usual sequence containter operations.
 */

namespace pgext {

template <class Type>
class list_base {
 public:
  using value_type = Type;
  using pointer = value_type*;
  using reference = value_type&;
  using size_type = size_t;

#if __cplusplus >= 201103L
  list_base() = default;
#else
  list_base() {}
#endif

  size_type size() const noexcept { return pg::list_length(list_); }

 protected:
  pg::List* list_;
};

template <class Type>
class list : public list_base<Type> {
 public:
  void push_back(const Type& val) noexcept {
    this->list_ = lappend(this->list_, val);
  }
};

template <>
class list<int> : public list_base<int> {
 public:
  void push_back(int val) noexcept {
    this->list_ = lappend_int(this->list_, val);
  }

 private:
  pg::List* list_;
};
}  // namespace pgext

#endif  // PGEXT_LIST_H_
