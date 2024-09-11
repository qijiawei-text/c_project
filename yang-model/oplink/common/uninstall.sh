#!/bin/bash

#New-sysrepo, some module are internal for netopeer2 and sysrepo, 
# They can not be uninstalled
#/usr/local/bin/sysrepoctl -u ietf-netconf-monitoring
#/usr/local/bin/sysrepoctl -u ietf-netconf-with-defaults
#/usr/local/bin/sysrepoctl -u ietf-netconf-notifications
#/usr/local/bin/sysrepoctl -u nc-notifications
#/usr/local/bin/sysrepoctl -u notifications
#/usr/local/bin/sysrepoctl -u ietf-netconf
#/usr/local/bin/sysrepoctl -u ietf-yang-library
#/usr/local/bin/sysrepoctl -u ietf-datastores
#/usr/local/bin/sysrepoctl -u ietf-netconf-acm
/usr/local/bin/sysrepoctl -u com-oplink-ipc
/usr/local/bin/sysrepoctl -u org-openroadm-interfaces
/usr/local/bin/sysrepoctl -u openconfig-transport-types
/usr/local/bin/sysrepoctl -u openconfig-platform-types
###

/usr/local/bin/sysrepoctl -u ietf-netconf-server
/usr/local/bin/sysrepoctl -u ietf-tcp-server
/usr/local/bin/sysrepoctl -u ietf-tcp-client
/usr/local/bin/sysrepoctl -u ietf-tls-server
/usr/local/bin/sysrepoctl -u ietf-tls-common
/usr/local/bin/sysrepoctl -u ietf-ssh-server
/usr/local/bin/sysrepoctl -u ietf-ssh-common
/usr/local/bin/sysrepoctl -u iana-crypt-hash
/usr/local/bin/sysrepoctl -u ietf-tcp-common
/usr/local/bin/sysrepoctl -u ietf-truststore
/usr/local/bin/sysrepoctl -u ietf-keystore
/usr/local/bin/sysrepoctl -u ietf-x509-cert-to-name
/usr/local/bin/sysrepoctl -u ietf-crypto-types
/usr/local/bin/sysrepoctl -u nc-notifications
/usr/local/bin/sysrepoctl -u notifications
/usr/local/bin/sysrepoctl -u ietf-netconf-monitoring
/usr/local/bin/sysrepoctl -u ietf-netconf-nmda

#/usr/local/bin/sysrepoctl -u ietf-netconf
/usr/local/bin/sysrepoctl -u ietf-netconf-acm
#/usr/local/bin/sysrepoctl -u notifications
exit 0
