# README #

MasterPhyl is a program for phylogenetic analysis of serially homologous 
morphological characters.

Note: MasterPhyl is currently being rewritten in C++. The original C version can
be found in the legacy-c-version branch.

### Version ###

* 0.10.0

[![Build Status](https://travis-ci.org/dominicwhite/MasterPhyl.svg?branch=master)](https://travis-ci.org/dominicwhite/MasterPhyl)

### How do I get set up? ###

Requires CMake >= 3.10

* Clone Git repository: `git clone https://github.com/dominicwhite/masterphyl.git`
* Configure using CMake: `cmake -H. -B_builds/Release -DCMAKE_BUILD_TYPE=Release`
* Build: `cmake --build _builds/Release`

To build for debugging:

* Configure using CMake: `cmake -H. -B_builds/Debug -DCMAKE_BUILD_TYPE=Debug`
* Build: `cmake --build _builds/Debug`
* Run tests: `./_builds/Debug/tests/unit_tests/Unit_Tests_run`

### Contibuting ###

MasterPhyl follows the Google C++ style guide, and uses the following Git commit message conventions:

Tag | Purpose
--- | ---
FEAT | for new feature implementing commit
FIX | for bug fix commit
SECURE | for security issue fix commit
PERF | for performance issue fix commit
REFACT | for refactoring commit
DOCS | for documentation commit
EXAMPLE | for example code commit
TEST | for testing commit
DEPEND | for dependencies upgrading or downgrading commit
CONFIG | for configuration commit
BUILD | for packaging or bundling commit
RELEASE | for publishing commit
WIP | for work in progress commit
CHORE | for other operations commit

### Who do I talk to? ###

* Developed and maintained by Dominic White: dewhite4@gmail.com