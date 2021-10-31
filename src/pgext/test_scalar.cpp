#include <cpostgres>

#include "pgext/scalar.h"

PG_FUNCTION_INFO_V1(test_array);
Datum test_scalar_int32(PG_FUNCTION_ARGS) {
  pgext::scalar<int> value = PG_GETARG_INT32(0);
}
