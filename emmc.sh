#!/bin/bash


sudo mkdir -p /mnt/ori /mnt/dest
sudo mount /dev/sda2 /mnt/ori
sudo mount /dev/sda1 /mnt/ori/boot

set -e
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

EX_LIST="/mnt/ori/home/pi/RetroPie/retropiemenu/RetrOrangePi/exclude.txt"

cd  /mnt/ori/usr/lib/linux* && sudo dd if=u-boot-sunxi-with-spl.bin of=/dev/mmcblk1 bs=1024 seek=8
sudo dd if=/dev/sda1 of=/dev/mmcblk1p1
sudo mkfs -t ext4 -F /dev/mmcblk1p2
sudo mount /dev/mmcblk1p2 /mnt/dest

# sync emmc
sudo rsync -avrltD  --delete --exclude-from=$EX_LIST /mnt/ori/. /mnt/dest/

# update fstab 
sudo sed -i '/ext4/d' /mnt/dest/etc/fstab
sudo sed -i -e '$i /dev/mmcblk0p2 / ext4 defaults,noatime,nodiratime,commit=600,errors=remount-ro 0 1' /mnt/dest/etc/fstab
sudo sed -i '/vfat/d' /mnt/dest/etc/fstab
sudo sed -i -e '$i /dev/mmcblk0p1 /boot vfat defaults 0 2' /mnt/dest/etc/fstab

# mountig boot part
sudo mount /dev/mmcblk1p1 /mnt/dest/boot

## Usage: FexChange <fexfile> <section> <key> <value> 
## Description: Changes the value of a given key (or adds key=value) to a specific section of FEX file
FexChange()
{
	# Search in file ($1) in section ($2) for key ($3)
	found=$(sed -n -e "/^\[$2\]/,/^\[/{/^$3\s*=/p}" "$1")
  
	if [ -n "$found" ]; then
		# Replace in file ($1) in section ($2) key ($3) with value ($4)
		sed -i -e "/^\[$2\]/,/^\[/{s/^\($3\s*=\s*\).*/\1$4/}" "$1"
	else
		# Append in file ($1) in section ($2) key ($3) with value ($4)
		sed -i -e "/^\[$2\]/,/^\[/{/^\[/i$3 = $4\n" -e "}" "$1"
	fi
}

# Script.bin modification
sudo cp /mnt/dest/boot/script.bin /mnt/dest/boot/script.bin.$(date +%Y%m%d)
sudo bin2fex /mnt/dest/boot/script.bin /mnt/dest/boot/script.fex
FexChange "/mnt/dest/boot/script.fex" "mmc0_para" "sdc_detmode" "1" # Use Screen0 configs
sudo fex2bin /mnt/dest/boot/script.fex /mnt/dest/boot/script.bin

sudo sed -i -e 's/mmc 0/mmc 1/g' /mnt/dest/boot/boot-retro.cmd
sudo sed -i -e 's/mmc 0/mmc 1/g' /mnt/dest/boot/boot.kodi.cmd
sudo mkimage -C none -A arm -T script -d /mnt/dest/boot/boot-retro.cmd /mnt/dest/boot/boot.scr
sudo umount /mnt/dest/boot
sudo umount /mnt/dest
sudo umount /mnt/ori/boot
sudo umount /mnt/ori

sudo rm -r /mnt/dest /mnt/ori
