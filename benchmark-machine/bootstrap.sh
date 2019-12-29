echo '--------------------------------------------------------------------'
echo 'START BOOTSTRAP'
echo '--------------------------------------------------------------------'

echo '--------------------------------------------------------------------'
echo 'START PREINSTALLS'
echo '--------------------------------------------------------------------'

sudo apt update

sudo apt install gcc cmake clang g++ gcc-multilib g++-multilib build-essential git libboost-all-dev libgmp3-dev libgmp-dev libssl-dev libprocps-dev pkg-config perl unzip -y

echo '--------------------------------------------------------------------'
echo 'START MIRACL INSTALL'
echo '--------------------------------------------------------------------'

#install MIRACL
mkdir miracl
cd miracl
wget https://github.com/miracl/MIRACL/archive/master.zip
sudo unzip -j -aa -L -o master.zip
sudo bash linux64
g++ -c -m64 -O poly2.cpp
g++ -c -m64 -O poly2Mod.cpp
g++ -c -m64 -O gf2m.cpp
cd ..

echo '--------------------------------------------------------------------'
echo 'START PARI INSTALL'
echo '--------------------------------------------------------------------'

#install PARI
sudo apt install libpari-dev -y

echo '--------------------------------------------------------------------'
echo 'START LIBFF INSTALL'
echo '--------------------------------------------------------------------'

#install libff
git clone https://github.com/scipr-lab/libff.git
cd libff
sudo git submodule init && git submodule update
mkdir build
cd build
sudo cmake ..
sudo make
sudo make install
cd ../..

echo '--------------------------------------------------------------------'
echo 'START FLINT INSTALL'
echo '--------------------------------------------------------------------'

#install flint
sudo apt install libflint-dev -y

echo '--------------------------------------------------------------------'
echo 'START MPFQ INSTALL'
echo '--------------------------------------------------------------------'

#install mpfq
wget mpfq.gforge.inria.fr/mpfq-1.1.tar.gz
tar -xvzf mpfq-1.1.tar.gz
cd mpfq-1.1
mkdir build_dir
cd build_dir
sudo cmake ../src
sudo make
sudo make install
cd ../..

echo '--------------------------------------------------------------------'
echo 'START NTL INSTALL'
echo '--------------------------------------------------------------------'

#install NTL
wget https://www.shoup.net/ntl/ntl-11.4.1.tar.gz
tar -xvzf ntl-11.4.1.tar.gz
cd ntl-11.4.1/src
sudo bash configure
sudo make
sudo make install
cd ../..

echo '--------------------------------------------------------------------'
echo 'START GOOGLE BENCHMARK INSTALL'
echo '--------------------------------------------------------------------'

install google benchmark
git clone https://github.com/google/benchmark.git
git clone https://github.com/google/googletest.git benchmark/googletest
cd benchmark
mkdir build && cd build
sudo cmake ../
sudo make install
cd ../..

echo '--------------------------------------------------------------------'
echo 'END BOOTSTRAP'
echo '--------------------------------------------------------------------'