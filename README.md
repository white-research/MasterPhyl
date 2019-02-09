# README #

MasterPhyl is a program for phylogenetic analysis of serially homologous 
morphological characters.

Note: MasterPhyl is currently being rewritten in C++. The original C version can
be found in the legacy-c-version branch.

### Version ###

* 0.10.0

### How do I get set up? ###

Requires CMake >= 3.10

* Clone Git repository: `git clone https://github.com/dominicwhite/masterphyl.git`
* Configure using CMake: `cmake -H. -B_builds/Release -DCMAKE_BUILD_TYPE=Release`
* Build: `cmake --build _builds/Release`

To build for debugging:

* Configure using CMake: `cmake -H. -B_builds/Debug -DCMAKE_BUILD_TYPE=Debug`
* Build: `cmake --build _builds/Debug`
* Run tests: `./_builds/Debug/tests/unit_tests/Unit_Tests_run`

### Who do I talk to? ###

* Developed and maintained by Dominic White: dewhite4@gmail.com