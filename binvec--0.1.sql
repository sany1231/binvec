\echo Use "CREATE EXTENSION binvec" to load this file. \quit

CREATE OR REPLACE FUNCTION
vec_sum_bin(anyarray, int32)
RETURNS anyarray
AS 'binvec', 'vec_sum_bin'
LANGUAGE c;