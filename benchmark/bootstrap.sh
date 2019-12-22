echo '--------------------------------------------------------------------'
echo 'START BOOTSTRAP'
echo '--------------------------------------------------------------------'
sudo apt update

sudo apt install gcc cmake clang g++ -y

# Check out the library.
git clone https://github.com/google/benchmark.git
# Benchmark requires Google Test as a dependency. Add the source tree as a subdirectory.
git clone https://github.com/google/googletest.git benchmark/googletest
# Go to the library root directory
cd benchmark
# Make a build directory to place the build output.
mkdir build && cd build
# Generate a Makefile with cmake.
# Use cmake -G <generator> to generate a different file type.
sudo cmake ../
# Build the library.
# Use make -j<number_of_parallel_jobs> to speed up the build process, e.g. make -j8 .
sudo make install

echo '--------------------------------------------------------------------'
echo 'END BOOTSTRAP'
echo '--------------------------------------------------------------------'