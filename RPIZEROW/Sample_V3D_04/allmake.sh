./vc4asm fs_normal_z.qasm -C fs_normal_z.h
./vc4asm fs_add.qasm -C fs_add.h
cat fs_normal_z.h
cat fs_add.h
make && ./install.sh

