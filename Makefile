# this is the build file for project dynareadout
# it is autogenerated by the xmake build system.
# do not edit by hand.

ifneq ($(VERBOSE),1)
VV=@
endif

AS=/usr/bin/gcc
GC=/usr/bin/go
CXX=/usr/bin/gcc
MM=/usr/bin/gcc
CC=/usr/bin/gcc
MXX=/usr/bin/gcc
RC=/usr/bin/rustc

AR=/usr/bin/ar
RCAR=/usr/bin/rustc
GCAR=/usr/bin/go
LD=/usr/bin/g++
RCLD=/usr/bin/rustc
GCLD=/usr/bin/go
SH=/usr/bin/g++
RCSH=/usr/bin/rustc


dynareadout_CCFLAGS=-m64 -fvisibility=hidden -O3 -ansi -DTHREAD_SAFE -fPIC -DNDEBUG
dynareadout_ARFLAGS=-cr
dynareadout_cpp_CXXFLAGS=-m64 -fvisibility=hidden -fvisibility-inlines-hidden -O3 -std=c++17 -Isrc -DTHREAD_SAFE -fPIC -DNDEBUG
dynareadout_cpp_ARFLAGS=-cr

default:  dynareadout

all:  dynareadout dynareadout_cpp

.PHONY: default all  dynareadout dynareadout_cpp

dynareadout: build/linux/x86_64/release/libdynareadout.a
build/linux/x86_64/release/libdynareadout.a: build/.objs/dynareadout/linux/x86_64/release/src/line.c.o build/.objs/dynareadout/linux/x86_64/release/src/key.c.o build/.objs/dynareadout/linux/x86_64/release/src/sync.c.o build/.objs/dynareadout/linux/x86_64/release/src/extra_string.c.o build/.objs/dynareadout/linux/x86_64/release/src/binout_read.c.o build/.objs/dynareadout/linux/x86_64/release/src/binout_directory.c.o build/.objs/dynareadout/linux/x86_64/release/src/d3plot_part_nodes.c.o build/.objs/dynareadout/linux/x86_64/release/src/d3plot_state.c.o build/.objs/dynareadout/linux/x86_64/release/src/path_view.c.o build/.objs/dynareadout/linux/x86_64/release/src/d3plot.c.o build/.objs/dynareadout/linux/x86_64/release/src/binary_search.c.o build/.objs/dynareadout/linux/x86_64/release/src/binout.c.o build/.objs/dynareadout/linux/x86_64/release/src/d3_buffer.c.o build/.objs/dynareadout/linux/x86_64/release/src/d3plot_data.c.o build/.objs/dynareadout/linux/x86_64/release/src/multi_file.c.o build/.objs/dynareadout/linux/x86_64/release/src/binout_glob.c.o build/.objs/dynareadout/linux/x86_64/release/src/path.c.o
	@echo linking.release libdynareadout.a
	@mkdir -p build/linux/x86_64/release
	$(VV)$(AR) $(dynareadout_ARFLAGS) build/linux/x86_64/release/libdynareadout.a build/.objs/dynareadout/linux/x86_64/release/src/line.c.o build/.objs/dynareadout/linux/x86_64/release/src/key.c.o build/.objs/dynareadout/linux/x86_64/release/src/sync.c.o build/.objs/dynareadout/linux/x86_64/release/src/extra_string.c.o build/.objs/dynareadout/linux/x86_64/release/src/binout_read.c.o build/.objs/dynareadout/linux/x86_64/release/src/binout_directory.c.o build/.objs/dynareadout/linux/x86_64/release/src/d3plot_part_nodes.c.o build/.objs/dynareadout/linux/x86_64/release/src/d3plot_state.c.o build/.objs/dynareadout/linux/x86_64/release/src/path_view.c.o build/.objs/dynareadout/linux/x86_64/release/src/d3plot.c.o build/.objs/dynareadout/linux/x86_64/release/src/binary_search.c.o build/.objs/dynareadout/linux/x86_64/release/src/binout.c.o build/.objs/dynareadout/linux/x86_64/release/src/d3_buffer.c.o build/.objs/dynareadout/linux/x86_64/release/src/d3plot_data.c.o build/.objs/dynareadout/linux/x86_64/release/src/multi_file.c.o build/.objs/dynareadout/linux/x86_64/release/src/binout_glob.c.o build/.objs/dynareadout/linux/x86_64/release/src/path.c.o

build/.objs/dynareadout/linux/x86_64/release/src/line.c.o: src/line.c
	@echo compiling.release src/line.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/line.c.o src/line.c

build/.objs/dynareadout/linux/x86_64/release/src/key.c.o: src/key.c
	@echo compiling.release src/key.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/key.c.o src/key.c

build/.objs/dynareadout/linux/x86_64/release/src/sync.c.o: src/sync.c
	@echo compiling.release src/sync.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/sync.c.o src/sync.c

build/.objs/dynareadout/linux/x86_64/release/src/extra_string.c.o: src/extra_string.c
	@echo compiling.release src/extra_string.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/extra_string.c.o src/extra_string.c

build/.objs/dynareadout/linux/x86_64/release/src/binout_read.c.o: src/binout_read.c
	@echo compiling.release src/binout_read.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/binout_read.c.o src/binout_read.c

build/.objs/dynareadout/linux/x86_64/release/src/binout_directory.c.o: src/binout_directory.c
	@echo compiling.release src/binout_directory.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/binout_directory.c.o src/binout_directory.c

build/.objs/dynareadout/linux/x86_64/release/src/d3plot_part_nodes.c.o: src/d3plot_part_nodes.c
	@echo compiling.release src/d3plot_part_nodes.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/d3plot_part_nodes.c.o src/d3plot_part_nodes.c

build/.objs/dynareadout/linux/x86_64/release/src/d3plot_state.c.o: src/d3plot_state.c
	@echo compiling.release src/d3plot_state.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/d3plot_state.c.o src/d3plot_state.c

build/.objs/dynareadout/linux/x86_64/release/src/path_view.c.o: src/path_view.c
	@echo compiling.release src/path_view.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/path_view.c.o src/path_view.c

build/.objs/dynareadout/linux/x86_64/release/src/d3plot.c.o: src/d3plot.c
	@echo compiling.release src/d3plot.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/d3plot.c.o src/d3plot.c

build/.objs/dynareadout/linux/x86_64/release/src/binary_search.c.o: src/binary_search.c
	@echo compiling.release src/binary_search.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/binary_search.c.o src/binary_search.c

build/.objs/dynareadout/linux/x86_64/release/src/binout.c.o: src/binout.c
	@echo compiling.release src/binout.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/binout.c.o src/binout.c

build/.objs/dynareadout/linux/x86_64/release/src/d3_buffer.c.o: src/d3_buffer.c
	@echo compiling.release src/d3_buffer.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/d3_buffer.c.o src/d3_buffer.c

build/.objs/dynareadout/linux/x86_64/release/src/d3plot_data.c.o: src/d3plot_data.c
	@echo compiling.release src/d3plot_data.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/d3plot_data.c.o src/d3plot_data.c

build/.objs/dynareadout/linux/x86_64/release/src/multi_file.c.o: src/multi_file.c
	@echo compiling.release src/multi_file.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/multi_file.c.o src/multi_file.c

build/.objs/dynareadout/linux/x86_64/release/src/binout_glob.c.o: src/binout_glob.c
	@echo compiling.release src/binout_glob.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/binout_glob.c.o src/binout_glob.c

build/.objs/dynareadout/linux/x86_64/release/src/path.c.o: src/path.c
	@echo compiling.release src/path.c
	@mkdir -p build/.objs/dynareadout/linux/x86_64/release/src
	$(VV)$(CC) -c $(dynareadout_CCFLAGS) -o build/.objs/dynareadout/linux/x86_64/release/src/path.c.o src/path.c

dynareadout_cpp: build/linux/x86_64/release/libdynareadout_cpp.a
build/linux/x86_64/release/libdynareadout_cpp.a: build/linux/x86_64/release/libdynareadout.a build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot_part.cpp.o build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot_state.cpp.o build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot.cpp.o build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/binout.cpp.o build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/key.cpp.o
	@echo linking.release libdynareadout_cpp.a
	@mkdir -p build/linux/x86_64/release
	$(VV)$(AR) $(dynareadout_cpp_ARFLAGS) build/linux/x86_64/release/libdynareadout_cpp.a build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot_part.cpp.o build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot_state.cpp.o build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot.cpp.o build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/binout.cpp.o build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/key.cpp.o

build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot_part.cpp.o: src/cpp/d3plot_part.cpp
	@echo compiling.release src/cpp/d3plot_part.cpp
	@mkdir -p build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp
	$(VV)$(CXX) -c $(dynareadout_cpp_CXXFLAGS) -o build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot_part.cpp.o src/cpp/d3plot_part.cpp

build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot_state.cpp.o: src/cpp/d3plot_state.cpp
	@echo compiling.release src/cpp/d3plot_state.cpp
	@mkdir -p build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp
	$(VV)$(CXX) -c $(dynareadout_cpp_CXXFLAGS) -o build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot_state.cpp.o src/cpp/d3plot_state.cpp

build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot.cpp.o: src/cpp/d3plot.cpp
	@echo compiling.release src/cpp/d3plot.cpp
	@mkdir -p build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp
	$(VV)$(CXX) -c $(dynareadout_cpp_CXXFLAGS) -o build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot.cpp.o src/cpp/d3plot.cpp

build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/binout.cpp.o: src/cpp/binout.cpp
	@echo compiling.release src/cpp/binout.cpp
	@mkdir -p build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp
	$(VV)$(CXX) -c $(dynareadout_cpp_CXXFLAGS) -o build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/binout.cpp.o src/cpp/binout.cpp

build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/key.cpp.o: src/cpp/key.cpp
	@echo compiling.release src/cpp/key.cpp
	@mkdir -p build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp
	$(VV)$(CXX) -c $(dynareadout_cpp_CXXFLAGS) -o build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/key.cpp.o src/cpp/key.cpp

clean:  clean_dynareadout clean_dynareadout_cpp

clean_dynareadout:
	@rm -rf build/linux/x86_64/release/libdynareadout.a
	@rm -rf build/linux/x86_64/release/dynareadout.sym
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/line.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/key.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/sync.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/extra_string.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/binout_read.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/binout_directory.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/d3plot_part_nodes.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/d3plot_state.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/path_view.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/d3plot.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/binary_search.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/binout.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/d3_buffer.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/d3plot_data.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/multi_file.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/binout_glob.c.o
	@rm -rf build/.objs/dynareadout/linux/x86_64/release/src/path.c.o

clean_dynareadout_cpp:  clean_dynareadout
	@rm -rf build/linux/x86_64/release/libdynareadout_cpp.a
	@rm -rf build/linux/x86_64/release/dynareadout_cpp.sym
	@rm -rf build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot_part.cpp.o
	@rm -rf build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot_state.cpp.o
	@rm -rf build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/d3plot.cpp.o
	@rm -rf build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/binout.cpp.o
	@rm -rf build/.objs/dynareadout_cpp/linux/x86_64/release/src/cpp/key.cpp.o

