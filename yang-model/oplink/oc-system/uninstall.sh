#!/bin/bash

SUB_TYPE=$1

/usr/local/bin/sysrepoctl -u com-oplink-system-logging
/usr/local/bin/sysrepoctl -u com-oplink-system-ext
/usr/local/bin/sysrepoctl -u openconfig-system
/usr/local/bin/sysrepoctl -u openconfig-system-logging
sudo /usr/local/bin/sysrepoctl -u openconfig-acl

exit 0
