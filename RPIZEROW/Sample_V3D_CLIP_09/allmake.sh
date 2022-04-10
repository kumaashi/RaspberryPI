echo "make fs_copy_texture"
./vc4asm fs_copy_texture.qasm -C fs_copy_texture.h
cat fs_copy_texture.h

echo "make fs_normal_z"
./vc4asm fs_normal_z.qasm -C fs_normal_z.h
cat fs_normal_z.h

echo "make fs_normal_texture_z"
./vc4asm fs_normal_texture_z.qasm -C fs_normal_texture_z.h
cat fs_normal_texture_z.h

echo "make fs_add"
./vc4asm fs_add.qasm -C fs_add.h
cat fs_add.h

echo "make fs_depth"
./vc4asm fs_depth.qasm -C fs_depth.h
cat fs_depth.h




make && ./install.sh
