# Concepts

## g++
// TODO: got further on the rules and other level
### optimization related
* `-O0`: No Optimization (Default in Debug Mode)
  * [Pros] Fast compilation.
  * [Pros] Easy debugging - line numbers match source.
  * [Cons] Code runs slower
  * [Cons] Larger binaries
* `-O2`: High-Level Optimization(Balanced for Speed)
  * Enable a broad set of optimizations that improve performance without significantly increasing compile time.
  * [Pros] Run much faster.
  * [Pros] Good balance between speed and compilation time.
  * [Pros] Safe - does not trade correctness for performance
  * [Cons] Debugging is harder(optimizations may reorder or remove code)
  * [Cons] Slightly slower compilation.
