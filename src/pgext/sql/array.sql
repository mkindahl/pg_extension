CREATE FUNCTION test_sum(anyarray) RETURNS anyelement
    AS 'MODULE_PATHNAME', 'test_sum'
    LANGUAGE C IMMUTABLE STRICT;

