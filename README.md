# memdb

### Description
An in-memory SQL-based relational database management system implemented on C++.

### Requirements

* CMake version 3.23 and newer
* C++20 compiler

### Building

* Cloning into repository
```
git clone https://github.com/Andromeddda/memdb.git
```
```
cd memdb
```

* Fast building _(for bash shell)_
```
./install
```

* Configuring manually
```
mkdir -p ./build
```
```
cmake -B ./build -S .
```

* Building manually
```
cmake --build ./build --target memdb -j 4
```

* Building tests
```
cmake --build ./build --target run_tests -j 4
```
