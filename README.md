


git clone git://github.com/homer6/modeler.git
cd modeler
git submodule update --init --recursive

cd lib/jet/
cmake .
make
sudo ln -s `pwd`/build/libjet_shared.so /usr/lib/libjet_shared.so
sudo ln -s `pwd`/build/libjet_static.a /usr/lib/libjet_static.a

cd ../..
cmake .
make

sudo ln -s `pwd`/build/jm /usr/bin/jm

jm simple.jm
