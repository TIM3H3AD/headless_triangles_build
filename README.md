Triangles (TRI) "Black Pharao" Version 4.2.1.0 Release, 

Triangles is a cool new crypto currency that features TOR implementation and secure messaging. 

This wallet supports the staking=0 option in the triangles.conf file to disable the stake miner thread for pool and exchange operators.

(To compile on 18.04 you will need to downgrade SSL. Skip these REMOVE & INSTALL SSL commands if installing on 16.04, just BUILD)

REMOVE & INSTALL SSL 
---
sudo apt remove libssl-dev

sudo apt install libssl1.0-dev

BUILD
---
cd src/leveldb

chmod 755 *

cd ..

make -f makefile.unix

