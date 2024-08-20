#!/usr/bin bash
dd if=./usb_1.01_python.img of=/dev/sdc
sync
partprobe

parted /dev/sdc print Fix
mount /dev/sdc2 ./boot_dir
grub-install --efi-directory=./boot_dir --boot-directory=./boot_dir /dev/sdc
#cp ./boot_dir/EFI/BOOT/grub.cfg ./boot_dir/grub/
sync
partprobe
umount /dev/sdc2