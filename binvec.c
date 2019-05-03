#include "postgres.h"
#include "fmgr.h"
#include "utils/array.h"
#include "catalog/pg_type.h"
#include "utils/lsyscache.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(vec_sum_bin);

Datum
vec_sum_bin(PG_FUNCTION_ARGS)
{
  Oid elemTypeId;
  int16 elemTypeWidth;
  bool elemTypeByValue;
  char elemTypeAlignmentCode;
  int lhsLength, c;
  int32 rhsNum, k;
  ArrayType *lhsArray, *retArray;
  Datum *lhsContent, *retContent;
  bool *lhsNulls, *rhsNulls, *retNulls;
  int dims[1];
  int lbs[1];

  if (PG_ARGISNULL(0) || PG_ARGISNULL(1)) {
    PG_RETURN_NULL();
  }

  lhsArray = PG_GETARG_ARRAYTYPE_P(0);
  rhsNum = PG_GETARG_INT32(1);

  if (ARR_NDIM(lhsArray) == 0) {
    PG_RETURN_NULL();
  }
  if (ARR_NDIM(lhsArray) > 1) {
    ereport(ERROR, (errmsg("vec_add: one-dimensional arrays are required")));
  }

  elemTypeId = ARR_ELEMTYPE(lhsArray);

  if (elemTypeId != INT2OID && elemTypeId != INT4OID && elemTypeId != INT8OID && elemTypeId != FLOAT4OID && elemTypeId != FLOAT8OID) {
    ereport(ERROR, (errmsg("vec_add input must be array of SMALLINT, INTEGER, BIGINT, REAL, or DOUBLE PRECISION")));
  }

  get_typlenbyvalalign(elemTypeId, &elemTypeWidth, &elemTypeByValue, &elemTypeAlignmentCode);

  deconstruct_array(lhsArray, elemTypeId, elemTypeWidth, elemTypeByValue, elemTypeAlignmentCode, &lhsContent, &lhsNulls, &lhsLength);

  retContent = palloc0(sizeof(Datum) * lhsLength);
  retNulls = palloc0(sizeof(bool) * lhsLength);
  
  for (c = 0; c <= 31; c++)
  {
    k = rhsNum & (1<<c);
    if(k>0) k = 1;
    else k = 0;
    retContent[c] = DatumGetInt32(lhsContent[c]) + k;
  }

  dims[0] = lhsLength;
  lbs[0] = 1;

  retArray = construct_md_array(retContent, retNulls, 1, dims, lbs, elemTypeId, elemTypeWidth, elemTypeByValue, elemTypeAlignmentCode);

  PG_RETURN_ARRAYTYPE_P(retArray);
  
}
