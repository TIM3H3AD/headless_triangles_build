Triangles (TRI) "Black Pharao" Version 4.2.1.0 Release, 

Triangles is a cool new crypto currency that features TOR implementation and secure messaging. 

This wallet supports the staking=0 option in the triangles.conf file to disable the stake miner thread for pool and exchange operators.

Headless Build Instructions:
(currently only working on ubuntu 16.04) working on SSL downgrade options 11/5/2020
See dependencies in doc/build-unix.txt

cd src/leveldb

chmod 755 *

cd ..

make -f makefile.unix

