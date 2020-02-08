# Tiny CPU caches benchmark

Main purpose of this application is to show, sizes, and speedup of using cpu caches.
Special thanks for [google-benchmark][1] library developers, which makes it easy to measure.
Also thanks for [google_benchmark_plot][2] library for an easy implementation of results plotting in python.


## My own machine configuration and results

```
Laptop Intel i9 2,3 GHz, 8 cores, 16 threads(x2 hyper-threading)

CPU Caches:
  L1 Data 32K (x8)
  L1 Instruction 32K (x8)
  L2 Unified 262K (x8)
  L3 Unified 16777K (x1)
```



## Build

The recommended way is to use conan package manager.

* Firstly, make a build directory and call `conan install <path-to-conanfile.txt>` 
to install all dependencies(There is only one - google benchmark library).
Some users might want to check it's a truth,
it's really easy with call `conan info <path-to-conanfile.txt>`.

* Then configure cmake. For example, in your build directory
call `cmake -G Ninja <path-to-root-CMakeLists.txt> -DCMAKE_BUILD_TYPE=Release`.

* Final step is to call `ninja` or yours own generator in build directory
to build executable and run it with `./cache-benchmark` command.

## Run and generate plot

* Execute `./cache-benchmark` and watch console output.

* If you want to see plots, execute you benchmark 
with `./cache-benchmark --benchmark_format=csv > benchmark.csv` to save results into csv.
Then run patched `plot.py` from `google_benchmark_plot` directory.
Example call is `plot.py -f benchmark.csv --logx`.

[1]: https://github.com/google/benchmark
[2]: https://github.com/lakshayg/google_benchmark_plot.git
