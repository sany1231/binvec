# binvec

BASH

    cc -fPIC -c binvec.c -I /opt/postgres95/9.5.4/include/server
    cc -shared -o binvec.so binvec.o
    cp binvec.so /opt/postgres95/9.5.4/lib/
    
    cp binvec.control /opt/postgres95/9.5.4/share/extension/
    cp binvec--0.1.sql /opt/postgres95/9.5.4/share/extension/

SQL

    CREATE EXTENSION binvec;

    DROP AGGREGATE bin2vec32(integer);
    CREATE AGGREGATE bin2vec32(integer) (
        SFUNC = vec_sum_bin,
        STYPE = integer[],
        INITCOND = '{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}'
    );
