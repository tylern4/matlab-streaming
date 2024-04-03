# matlab-streaming

 Attempting to see what works for streaming data to/from matlab

## Building

On Perlmutter first load modules

```bash
module load cpu matlab
export CC=gcc; export CXX=g++; 
```

```bash
git clone --recurse-submodules https://github.com/tylern4/matlab-streaming.git
mkdir matlab-streaming/build
cd matlab-streaming/build
cmake ..
make -j
```
