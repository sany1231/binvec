# binvec

    cc -fPIC -c binvec.c -I /opt/postgres10/10.0/include/server;
    cc -shared -o binvec.so binvec.o;
    cp binvec.so /opt/postgres10/10.0/lib/

    CREATE EXTENSION binvec;

    DROP AGGREGATE bin2vec32(integer);
    CREATE AGGREGATE bin2vec32(integer) (
        SFUNC = vec_sum_bin,
        STYPE = integer[],
        INITCOND = '{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}'
    );
