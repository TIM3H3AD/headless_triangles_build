Triangles (TRI) "Black Pharao" Version 4.2.1.0 Release, 

Triangles is a cool new crypto currency that features TOR implementation and secure messaging. 

This wallet supports the staking=0 option in the triangles.conf file to disable the stake miner thread for pool and exchange operators.

HEADLESS BUILD:
---
(dependencies in doc/build-unix.txt)
Compile as is on ubuntu 16.04. To compile on 18.04 you will need to downgrade SSL:

sudo apt remove libssl-dev

sudo apt install libssl1.0-dev

---
NEXT RUN:
---

cd src/leveldb

chmod 755 *

cd ..

make -f makefile.unix

