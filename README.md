POPLITE
=======

Description
-----------
Poplite is a small C++ framework based on boost. It implements the POP (Parallel Object Programming) model for C++ objects. If you are interested in a more complete implementations you should have a look at the [POP-C++](https://github.com/pop-team/pop-cpp) framework developed by Tuan Ahn Nguyen and Pierre Kuonen.

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
	- clang 3.5
	- Python bindings for clang
	- astyle for alignement

	sudo apt-get install libboost-all-dev libclang-3.5-dev pip astyle
	sudo pip install clang

Example of parallel class
-------------------------
TODO

Broker and interface


Serialization

### Allocation of object
Since objects run now on their own process their creation requires to launch the object executable on the target machine. This is the function of the allocator. Depending on the envirnoment you can use either:
- A local allocator
- A remote allocator that uses ssh
- A manual allocator which only prints the command to run (for debug)

Parser
------
TODO



