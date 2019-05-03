# binvec

cc -fPIC -c binvec.c -I /opt/postgres10/10.0/include/server;
cc -shared -o binvec.so binvec.o;
cp binvec.so /opt/postgres10/10.0/lib/
