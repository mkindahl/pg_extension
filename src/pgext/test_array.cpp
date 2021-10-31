#include <cpostgres>

#include <numeric>

#include "pgext/array.h"
#include "pgext/result.h"
#include "pgext/scalar.h"

#define PG_RETURN_NULLABLE(VAL) \
  do {                          \
    if ((VAL).is_null())        \
      PG_RETURN_NULL();         \
    else                        \
      return (VAL).as_datum();  \
  } while (0)

PG_FUNCTION_INFO_V1(test_array);
Datum test_array(PG_FUNCTION_ARGS) {
  pgext::Result result(fcinfo);
  pgext::Array<pgext::Scalar<int>> arr = PG_GETARG_ARRAYTYPE_P(0);
  result = std::accumulate(arr.begin(), arr.end(), pgext::Scalar<int>(0));
  return result;
}
