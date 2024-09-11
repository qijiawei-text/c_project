#!/bin/bash

SUB_TYPE=$1

/usr/local/bin/sysrepoctl -u openconfig-platform-psu
/usr/local/bin/sysrepoctl -u openconfig-platform-fan
/usr/local/bin/sysrepoctl -u openconfig-platform-cpu
/usr/local/bin/sysrepoctl -u openconfig-platform-transceiver
/usr/local/bin/sysrepoctl -u com-oplink-platform-ext
/usr/local/bin/sysrepoctl -u openconfig-transport-line-common
/usr/local/bin/sysrepoctl -u com-oplink-platform
/usr/local/bin/sysrepoctl -u openconfig-platform
/usr/local/bin/sysrepoctl -u openconfig-alarm-types

exit 0
