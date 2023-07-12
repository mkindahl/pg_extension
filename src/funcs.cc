#include <cstring>

extern "C" {
#include <postgres.h>
#include <fmgr.h>

#include <utils/geo_decls.h>
}

extern "C" {

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(add_one);

Datum add_one(PG_FUNCTION_ARGS) {
  int32 arg = PG_GETARG_INT32(0);

  PG_RETURN_INT32(arg + 1);
}

/* by reference, fixed length */

PG_FUNCTION_INFO_V1(add_one_float8);

extern "C" Datum add_one_float8(PG_FUNCTION_ARGS) {
  float8 arg = PG_GETARG_FLOAT8(0);

  PG_RETURN_FLOAT8(arg + 1.0);
}

PG_FUNCTION_INFO_V1(makepoint);

extern "C" Datum makepoint(PG_FUNCTION_ARGS) {
  /* Here, the pass-by-reference nature of Point is not hidden. */
  Point *pointx = PG_GETARG_POINT_P(0);
  Point *pointy = PG_GETARG_POINT_P(1);
  Point *new_point = (Point *)palloc(sizeof(Point));

  new_point->x = pointx->x;
  new_point->y = pointy->y;

  PG_RETURN_POINT_P(new_point);
}

/* by reference, variable length */

PG_FUNCTION_INFO_V1(copytext);

extern "C" Datum copytext(PG_FUNCTION_ARGS) {
  text *t = PG_GETARG_TEXT_PP(0);

  /*
   * VARSIZE_ANY_EXHDR is the size of the struct in bytes, minus the
   * VARHDRSZ or VARHDRSZ_SHORT of its header.  Construct the copy with a
   * full-length header.
   */
  text *new_t = (text *)palloc(VARSIZE_ANY_EXHDR(t) + VARHDRSZ);
  SET_VARSIZE(new_t, VARSIZE_ANY_EXHDR(t) + VARHDRSZ);

  /*
   * VARDATA is a pointer to the data region of the new struct.  The source
   * could be a short datum, so retrieve its data through VARDATA_ANY.
   */
  memcpy(VARDATA(new_t),        /* destination */
         VARDATA_ANY(t),        /* source */
         VARSIZE_ANY_EXHDR(t)); /* how many bytes */
  PG_RETURN_TEXT_P(new_t);
}

extern "C" Datum concat_text(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(concat_text);

Datum concat_text(PG_FUNCTION_ARGS) {
  text *arg1 = PG_GETARG_TEXT_PP(0);
  text *arg2 = PG_GETARG_TEXT_PP(1);
  int32 arg1_size = VARSIZE_ANY_EXHDR(arg1);
  int32 arg2_size = VARSIZE_ANY_EXHDR(arg2);
  int32 new_text_size = arg1_size + arg2_size + VARHDRSZ;
  text *new_text = (text *)palloc(new_text_size);

  SET_VARSIZE(new_text, new_text_size);
  memcpy(VARDATA(new_text), VARDATA_ANY(arg1), arg1_size);
  memcpy(VARDATA(new_text) + arg1_size, VARDATA_ANY(arg2), arg2_size);
  PG_RETURN_TEXT_P(new_text);
}
}
