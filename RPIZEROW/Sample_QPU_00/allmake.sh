#!/bin/sh -x
./vc4asm cp.qasm -C copy.h && cat copy.h && make && ./install.sh 
