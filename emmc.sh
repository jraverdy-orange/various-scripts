#!/bin/bash

#sudo mkdir -p /mnt/ori /mnt/dest
#sudo mount /dev/sda2 /mnt/ori
#sudo mount /dev/sda1 /mnt/ori/boot

fdisk_cmd="sudo fdisk /dev/mmcblk1 <<-EOF"

# creating the new partition table

sudo fdisk /dev/mmcblk1 <<EOF
o
n
p
1
8192
+512M
t
e
n
p
2
8192
w
EOF

EX_LIST="/mnt/ori/home/pi/RetrOrangePi/exclude.txt"

cd  /mnt/ori/usr/lib/linux* && sudo dd if=u-boot-sunxi-with-spl.bin of=/dev/mmcblk1 bs=1024 seek=8
sudo dd if=/dev/sda1 of=/dev/mmcblk1p1
sudo mkfs -t ext4 -F /dev/mmcblk1p2
sudo mount /dev/mmcblk1p2 /mnt/dest

# sync emmc
sudo rsync -avrltD  --delete --exclude-from=$EX_LIST /mnt/ori/. /mnt/dest/

# update fstab 
sudo sed -i '/ext4/d' /mnt/dest/etc/fstab
sudo sed -i -e '$i /dev/mmcblk0p2 / ext4 defaults,noatime,nodiratime,commit=600,errors=remount-ro 0 1' /mnt/rootfs/etc/fstab
sudo sed -i '/vfat/d' /mnt/dest/etc/fstab
sudo sed -i -e '$i /dev/mmcblk0p1 /boot vfat defaults 0 2' /mnt/rootfs/etc/fstab

# mountig boot part
sudo mount /dev/mmcblk1p1 /mnt/dest/boot

