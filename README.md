# binvec

BINVEC is C extension for postgreSQL.

Purpouse of this extension is to sum array input with integer input (representing binary values) into one summary array.
Why not just SQL function ? Becouse C extensons are much faster than SQL functions, if you are working with grate amount of data 3x faster function can save a lot of time.

## Example

Binary integer 123 in plain bit representation looks like 0111 1011.
Every bit is read like this:

    Int k = (rhsNum & (1<<c));
    
where **rhsNum** is binary representation number *(123 = 0111 1011)*.
where **c** is bit position you want to check if it is set. every loop 1 is moved by one position to the left so in the next turn we are checking if rhsNum contains number 2, next for 4,8,16,...

!! The length of output array is 32 so loop checks max 32 bits.

Summary of input array and checked number looks like this:

    if(k>0) n = 1;
    else n = 0;
    retContent[c] = DatumGetInt32(lhsContent[c]) + n; 
    
**retContent** is output array.
**lhsContent** is input array.
We are doing some checking if **k** is > 1 becouse we want just add +1 to existing value in **lhsContent** on **c** position.

## Implementation

1. Download GIT repository
2. Find path to your sql instalation and replace #PATH# with your path e.g.(/opt/postgres95/9.5.9), there is a trick to find out what is your path. Execute SQL query *CREATE EXTENSION some_non_created_extansion;*, this query will return error with path to your SQL instalation, but it is harmless.
3. Run bash commands from binvec folder.

BASH

    # compile binvec.c to object
    cc -fPIC -c binvec.c -I #PATH#/include/server
    
    # create .so lib file
    cc -shared -o binvec.so binvec.o
    
    # copy .so into lib direcotry
    cp binvec.so #PATH#/lib/
    
    # copy control and sql files
    cp binvec.control #PATH#/share/extension/
    cp binvec--0.1.sql #PATH#/share/extension/
    
4. Create SQL extansion and aggragate function.

SQL

    CREATE EXTENSION binvec;

    DROP AGGREGATE bin2vec32(integer);
    CREATE AGGREGATE bin2vec32(integer) (
        SFUNC = vec_sum_bin,
        STYPE = integer[],
        INITCOND = '{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}'
    );
    
## Test

    SELECT vec_sum_bin(ARRAY[1,0,1,0,1,0],123);

Should returns {2,1,1,1,2,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

! ENJOY
