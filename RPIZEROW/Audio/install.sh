mkdir mnt
sudo mount -t vfat /dev/sdb1 ./mnt/
sudo cp kernel.img ./mnt/
sync
sudo umount ./mnt

