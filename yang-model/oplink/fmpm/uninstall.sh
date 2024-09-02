#!/bin/bash
SUB_TYPE=$1

/usr/local/bin/sysrepoctl -u com-oplink-alarm
/usr/local/bin/sysrepoctl -u org-openroadm-alarm;
/usr/local/bin/sysrepoctl -u com-oplink-pm
/usr/local/bin/sysrepoctl -u org-openroadm-pm;
/usr/local/bin/sysrepoctl -u org-openroadm-tca;
/usr/local/bin/sysrepoctl -u openconfig-telemetry;
/usr/local/bin/sysrepoctl -u openconfig-telemetry-types;
/usr/local/bin/sysrepoctl -u com-oplink-ocpms;
/usr/local/bin/sysrepoctl -u openconfig-transport-pms;


exit 0
