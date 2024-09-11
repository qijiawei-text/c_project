#!/bin/bash

/usr/local/bin/sysrepoctl -u com-oplink-lldp-ext;
/usr/local/bin/sysrepoctl -u openconfig-lldp;
/usr/local/bin/sysrepoctl -u org-openroadm-rstp;

if [[ "$1" == "OLSP" ]]; then
    exit 0
fi

cd bgp
./uninstall.sh
cd -

exit 0
