POPLITE
=======

Description
-----------
Poplite is a small C++ framework based on boost. It implements the POP (Parallel Object Programming) model for C++ objects. If you are interested in a more complete implementations you should have a look at the [POP-C++](https://github.com/pop-team/pop-cpp) framework developed by Tuan Ahn Nguyen and Pierre Kuonen.

### Philosophy

C++ objects can be transformed into parallel objects that can be executed on a distant machine

In consequence:
- Objects have their own executable and will run as a separate process.
- At each invocation of a method the input and output of the method will be serialized and communicated to the remote object.
- Method calls can become asynchronous on caller side (sync or async methods)
- Method calls must manage access to resources on callee side (conc, seq and mutex methods) to avoid collisions. 
- References to objects become a network address.

Installation
------------

Dependencies
- boost libraries
- for the parser
	- clang 6.0 (if 6.0 matches the pip clang package)
	- Python bindings for clang
	- astyle for alignement

```
	sudo apt-get install libboost-all-dev libclang-6.0-dev python-pip astyle
	sudo pip install clang
```

- other useful packages:
	- openssh-server
	- cmake-ncurser-gui

```
	sudo apt-get install openssh-server cmake-ncurser-gui
```

Additionnally you may have to edit this line to match your version of clang in **parser.py**:

	cindex.Config.set_library_path("/usr/lib/llvm-6.0/lib")

If you still get errors be sure to add a symlink inside that directory

```
cd /usr/lib/llvm-6.0/lib
sudo ln -s libclang-6.0.so.1 libclang.so
```

Example of parallel class
-------------------------
An example of a parallel class can be found in the directory **examples/ping_pong**.


### Broker and interface
A parallel object consists of an interface and a broker. The broker creates and launches the remote object and the interface is the local point of contact to this object.

### Serialization
All types used in parallel methods must be serialized. This is already the case for all basic types. A custom class must implement the **serialize** method used by the boost serialization library. See example for example the **gps_position** class in **examples/get_and_set**.

### Allocation of object
Since objects run now on their own process their creation requires to launch the object executable on the target machine. This is the function of the allocator. Depending on the envirnoment you can use either:
- A local allocator
- A remote allocator that uses ssh
- A manual allocator which only prints the command to run (for debug)

### Parser
The interface and broker of each class must be created by a parser. They will be added to the the generated files of CMake. As a consequence you need to add additional lines in CMakeLists.txt. See any example.



