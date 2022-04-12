sudo mount -t vfat /dev/sdb1 ./mnt/
mkdir backup
sudo cp ./mnt/* ./backup -R
sync
sudo umount ./mnt

