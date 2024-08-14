mkdir /home/oplink/bootdisk
mount /dev/sda8 /home/oplink/bootdisk
if [ -f /home/oplink/bootdisk/start.sh ]; then
    echo 'Run /home/oplink/bootdisk/start.sh...'
    source /home/oplink/bootdisk/start.sh
else
   echo 'Can not find /home/oplink/bootdisk/start.sh...'
fi
