# Cache Simulator

You can edit `cache.c`, rebuild it with GCC, and run the simulator on Linux x86-64. The simulator's entry point and support code are provided in the prebuilt shared library `libcache_support.so`.

1. Clone or download this repository. Open a terminal in the directory containing `cache.c`, `common.h`, and `libcache_support.so`.
2. Compile `cache.c` and link it against the shared library:

   ```sh
   gcc -O1 -std=gnu11 cache.c -L. -lcache_support -Wl,-rpath,'$ORIGIN' -o cache-simulator
   ```

   `-L.` tells the linker to look in the current directory. The quoted `$ORIGIN` tells the runtime loader to look in the executable's directory for the library.

3. Run the randomized test with seed 1:

   ```sh
   ./cache-simulator -r 1
   ```

   A successful run prints `Random test pass!`.

4. Edit `cache.c`, then repeat steps 2 and 3 to test your changes. Keep `common.h` and `libcache_support.so` alongside it.

To check which library the executable uses, run:

```sh
ldd ./cache-simulator | grep libcache_support.so
```

The output should point to the `libcache_support.so` beside `cache-simulator`.
