#/bin/sh
export LD_LIBRARY_PATH=../../Greet-core/bin
make -C ../../Greet-core && make && exec bin/Sandbox.a
