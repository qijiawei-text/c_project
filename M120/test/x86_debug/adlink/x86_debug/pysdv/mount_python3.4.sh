
mkdir /home/oplink/bootdisk
mount /dev/sda8 /home/oplink/bootdisk
mkdir /usr/lib/python3.4
cp -r /home/oplink/bootdisk/python3.4/python3* /usr/bin/
cp -r /home/oplink/bootdisk/python3.4/lib/*   /usr/lib/python3.4
ln -s /usr/bin/python3.4 /usr/bin/python3
