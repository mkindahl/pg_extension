
#ifdef __cplusplus
extern "C" {
#endif

#include <postgres.h>

#include <utils/fmgrprotos.h>

#ifdef __cplusplus
}
#endif

#include "function.h"

Name convert(const char* message) {
  pgext::function<Name(const char*)> do_convert = namein;
  return do_convert(message);
}

int main() {}
