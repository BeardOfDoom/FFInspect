g++ -O2 miracl_benchmark_prime.cc /home/vagrant/miracl/zzn.o /home/vagrant/miracl/big.o /home/vagrant/miracl/miracl.a -I/home/vagrant/miracl -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -o miracl_benchmark_prime.o

g++ -O2 miracl_benchmark_poly.cc /home/vagrant/miracl/polymod.o /home/vagrant/miracl/poly.o /home/vagrant/miracl/ecn.o /home/vagrant/miracl/crt.o /home/vagrant/miracl/zzn.o /home/vagrant/miracl/big.o /home/vagrant/miracl/miracl.a -I/home/vagrant/miracl -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -o miracl_benchmark_poly.o

g++ -O2 miracl_benchmark_poly2.cc /home/vagrant/miracl/poly2mod.o /home/vagrant/miracl/poly2.o /home/vagrant/miracl/ec2.o /home/vagrant/miracl/gf2m.o /home/vagrant/miracl/ecn.o /home/vagrant/miracl/crt.o /home/vagrant/miracl/zzn.o /home/vagrant/miracl/big.o /home/vagrant/miracl/miracl.a -I/home/vagrant/miracl -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -o miracl_benchmark_poly2.o



g++ -O2 pari_benchmark_poly.cc -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -lpari -o pari_benchmark_poly.o

g++ -O2 pari_benchmark_poly2.cc -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -lpari -o pari_benchmark_poly2.o

g++ -O2 pari_benchmark_prime.cc -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -lpari -o pari_benchmark_prime.o



g++ -O2 flint_benchmark_poly.cc -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -lgmp -lflint -lmpfr -o flint_benchmark_poly.o

g++ -O2 flint_benchmark_poly2.cc -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -lgmp -lflint -lmpfr -o flint_benchmark_poly2.o

g++ -O2 flint_benchmark_prime.cc -std=c++11 -isystem /home/vagrant/benchmark/include/ -lbenchmark -lpthread -L/home/vagrant/benchmark/build/src -lgmp -lflint -lmpfr -o flint_benchmark_prime.o


./miracl_benchmark_prime.o "$@"
./miracl_benchmark_poly.o "$@"
./miracl_benchmark_poly2.o "$@"

./pari_benchmark_prime.o "$@"
./pari_benchmark_poly.o "$@"
./pari_benchmark_poly2.o "$@"

./flint_benchmark_prime.o "$@"
./flint_benchmark_poly.o "$@"
./flint_benchmark_poly2.o "$@"