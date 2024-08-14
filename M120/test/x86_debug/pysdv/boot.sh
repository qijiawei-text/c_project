pushd
cd /home/oplink/boot/python3.4/
source ./install.sh
cd ../
cp ./libsdv.so /lib64/
cp ./libft4222.so /lib64/
cp ./libSUSI-4.00.so.1 /lib64/
source ./test_device.sh
popd

