cat <<EOF >/root/.profile
cd /home/oplink/boot/test-script
python3.4 run_test.py
EOF
chmod +x /root/.profile
exit 0


