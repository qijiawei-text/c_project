#!/bin/bash

SUB_TYPE=$1

/usr/local/bin/sysrepoctl -u com-oplink-rpc;
/usr/local/bin/sysrepoctl -u com-oplink-service;
/usr/local/bin/sysrepoctl -u com-oplink-auth;
/usr/local/bin/sysrepoctl -u org-openroadm-database;
/usr/local/bin/sysrepoctl -u org-openroadm-de-operations;
/usr/local/bin/sysrepoctl -u org-openroadm-file-transfer;
/usr/local/bin/sysrepoctl -u org-openroadm-swdl;
/usr/local/bin/sysrepoctl -u ietf-aaa-tacacs;
/usr/local/bin/sysrepoctl -u ietf-system;
/usr/local/bin/sysrepoctl -u org-openroadm-syslog;

exit 0
