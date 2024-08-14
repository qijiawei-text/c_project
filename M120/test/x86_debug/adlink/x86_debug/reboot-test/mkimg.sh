# Create the disk and partition it

function mk_zero_img(){
	imname=$1
	echo "make image $imname.img"
	dd if=/dev/zero of=$imname.img bs=512 count=1843200
	parted --script $imname.img  mklabel gpt
	parted --script $imname.img  mkpart primary fat32 2048s 6143s
	parted --script $imname.img  name 1 grub
	parted --script $imname.img  set 1 bios_grub on
	parted --script $imname.img  mkpart primary fat32 6144s 131071s 
	parted --script $imname.img  name 2 boot
	parted --script $imname.img  set 2 boot on
	parted --script $imname.img  mkpart primary ext4 131072s 100%
	parted --script $imname.img  name 3 "wr_usb_boot"
}
function mk_partition(){
	imname=$1
	echo "image nane: $imname"
	# Set the partition variables
	loopdevice=`losetup -f --show $imname.img`
	partx -va $loopdevice
	sleep 5
	device=${loopdevice}
	bootp=${device}p2
	rootp=${device}p3
	echo "boot partition:$bootp"
	echo "root partition:$rootp"
	mkfs.vfat $bootp
	mkfs.ext4 -O ^flex_bg $rootp
	e2label $rootp "wr_usb_boot"
	sync
	partprobe

	partx -vd $loopdevice
	losetup -vd $loopdevice
}

function cp_image(){
src_imname=$1
dst_imnane=$2
src_boot_dir=$3
src_root_dir=$4
dst_boot_dir=$5
dst_root_dir=$6
silent=$7
echo "src_imname: $src_imname"
echo "dst_imnane: $dst_imnane"
echo "src_boot_dir: $src_boot_dir"
echo "src_root_dir: $src_root_dir"
echo "dst_boot_dir: $dst_boot_dir"
echo "dst_root_dir: $dst_root_dir"

if [ ! $silent ];then
read -p  "Sure to continue ?:No(Default),Yes " issure
if [ ! $issure == "Yes" ]; then
  exit 1
fi
fi
src_device=`losetup -f --show $src_imname.img`
partx -va $src_device
export SRC_DEVICE=$src_device
src_bootp=${src_device}p1
src_rootp=${src_device}p2
mount $src_bootp $src_boot_dir
mount $src_rootp $src_root_dir
dst_device=`losetup -f --show $dst_imnane.img`
partx -va $dst_device
export DST_DEVICE=$dst_device
dst_bootp=${dst_device}p2
dst_rootp=${dst_device}p3
mount $dst_bootp $dst_boot_dir
mount $dst_rootp $dst_root_dir

echo "Start sync..."
rsync -HPavz -q $src_boot_dir/ $dst_boot_dir/
rsync -HPavz -q $src_root_dir/ $dst_root_dir/
sync

##install grub
grub-install --efi-directory=$dst_boot_dir --boot-directory=$dst_boot_dir $dst_device
}


function modify_boot_cfg(){
boot_dir=$1
#mkdir ${boot_dir}
silent=$2
if [ ! $silent ];then
read -p  "Sure to continue in ${boot_dir}?:No(Default),Yes " issure
if [ ! $issure == "Yes" ]; then
  clean
  exit 1
fi
fi
#syslinux.cfg
cp ${boot_dir}/syslinux.cfg ${boot_dir}/syslinux.cfg_bak
sed -i "16,22d" ${boot_dir}/syslinux.cfg
sed -i "s/ttyS0/ttyS2/g" ${boot_dir}/syslinux.cfg
sed -i "/timeout/"d ${boot_dir}/syslinux.cfg
sed -i "/F1/itimeout 20" ${boot_dir}/syslinux.cfg
sed -i "/append/"d ${boot_dir}/syslinux.cfg
sed -i "/^initrd/a append rootwait max_loop=255 ro clock=pit console=ttyS2,115200 root=LABEL=wr_usb_boot selinux=0 enforcing=0 pcie_aspm=off pcie_ports=native" ${boot_dir}/syslinux.cfg

##grub.cfg
cp ${boot_dir}/EFI/BOOT/grub.cfg ${boot_dir}/EFI/BOOT/grub.cfg_bak
sed -i "1,7d" ${boot_dir}/EFI/BOOT/grub.cfg
sed -i "1a\set timeout=1" ${boot_dir}/EFI/BOOT/grub.cfg
sed -i "s/ttyS0/ttyS2/g" ${boot_dir}/EFI/BOOT/grub.cfg
cp ${boot_dir}/EFI/BOOT/grub.cfg ${boot_dir}/grub/grub.cfg
#rm -r ${boot_dir}
}


function modify_login(){
root_dir=$1
silent=$2
if [ ! $silent ];then
read -p  "Sure to continue in ${root_dir}?:No(Default),Yes " issure
if [ ! $issure == "Yes" ]; then
  clean
  exit 1
fi
fi

#mount $1 ${root_dir}
cp ${root_dir}/etc/rc.local ${root_dir}/etc/rc.local_bak
sed -i "12a\echo =========== loader =========== > /dev/ttyS2" ${root_dir}/etc/rc.local
sed -i "13a\mount /dev/sda4 /home/oplink/boot/" ${root_dir}/etc/rc.local
sed -i "14a\sleep 1s" ${root_dir}/etc/rc.local
sed -i "15a\chmod 777 /home/oplink/boot/loader" ${root_dir}/etc/rc.local
sed -i "16a\/home/oplink/boot/loader > /dev/ttyS2" ${root_dir}/etc/rc.local
sed -i "17a\source /home/oplink/boot/boot.sh" ${root_dir}/etc/rc.local
sed -i "/0/s/x//g" ${root_dir}/etc/passwd
sed -i "1d" ${root_dir}/etc/shadow
sed -i "1i\root::17392:0:99999:7:::" ${root_dir}/etc/shadow
sed -i "s/console/--autologin root console/" ${root_dir}/etc/inittab
sed -i "/eth0/"d ${root_dir}/etc/network/interfaces
sed -i "/eth1/"d ${root_dir}/etc/network/interfaces
mkdir -p ${root_dir}/home/oplink/boot
touch ${root_dir}/home/oplink/boot/boot.sh
#umount $1
#rm -r ${root_dir}
}


function install_sdvlib(){
    root_dir=$1
    sdv_dir=$2
    lib64_dir=${root_dir}/lib64
    cp ${sdv_dir}/libft4222.so ${lib64_dir}
    chmod +x ${lib64_dir}/libft4222.so
    cp ${sdv_dir}/libsdv.so ${lib64_dir}
    chmod +x ${lib64_dir}/libsdv.so
    cp ${sdv_dir}/libSUSI-4.00.so.1 ${lib64_dir}
    chmod +x ${lib64_dir}/libSUSI-4.00.so.1
}

function install_python34(){
    root_dir=$1
    py34_dir=$2
    mkdir -p ${root_dir}/usr/lib/python3.4
	cp -r ${py34_dir}/python3* ${root_dir}/usr/bin/
	cp -r ${py34_dir}/lib/*   ${root_dir}/usr/lib/python3.4
	#chroot $root_dir
	#ln -s ${root_dir}/usr/bin/python3.4 ${root_dir}/usr/bin/python3
}


function clean(){
    umount $src_boot_dir
	umount $src_root_dir
	umount $root_dir
	umount $boot_dir
	
	partx -vd $SRC_DEVICE
	losetup -vd $SRC_DEVICE
	partx -vd $DST_DEVICE
	losetup -vd $DST_DEVICE
}
silent=1
new_imname="test"
#This image is built from wr
src_imname="usb_1.01"
src_boot_dir="${PWD}/src_boot_dir"
src_root_dir="${PWD}/src_root_dir"
boot_dir="${PWD}/boot_dir"
root_dir="${PWD}/root_dir"
sdv_dir="${PWD}/sdv_install"
py34_dir="${PWD}/sdv_install/python3.4"
# Create file systems
mk_zero_img "test"
mk_partition "test"
cp_image $src_imname $new_imname $src_boot_dir $src_root_dir $boot_dir $root_dir $silent
modify_boot_cfg $boot_dir $silent
modify_login    $root_dir $silent
install_sdvlib $root_dir $sdv_dir
install_python34 $root_dir $py34_dir
clean