// -*- Mode: C++ -*-

extern "C" {
#include <postgres.h>
}

namespace pgext {

template <class Result, class... Args>
class function {
 public:
  typedef Result result_type;

  template <typename Functor>
  function(Functor functor) noexcept : functor_(functor) {}

  function(nullptr_t) noexcept {}

  Result operator()(Args... args) const {}

 private:
  Datum (*functor_)(PG_FUNCTION_ARGS);
};

}  // namespace pgext
