#!/bin/bash

echo "Testing"

set -e

source /opt/confd/confdrc
cd src

CONFD_OPTS="--fail-on-warnings"
CONFD_OPTS=""

echo "Testing compilations of VNFD."
confdc -c $CONFD_OPTS -o /opt/confd/etc/confd/etsi-nfv-vnfd.fxs etsi-nfv-vnfd.yang
echo "Testing compilations of PNFD."
confdc -c $CONFD_OPTS -o /opt/confd/etc/confd/etsi-nfv-pnfd.fxs etsi-nfv-pnfd.yang
echo "Testing compilations of NSD."
confdc -c $CONFD_OPTS -o /opt/confd/etc/confd/etsi-nfv-nsd.fxs etsi-nfv-nsd.yang
echo "Testing compilations of descriptors."
confdc -c $CONFD_OPTS -o /opt/confd/etc/confd/etsi-nfv-descriptors.fxs etsi-nfv-descriptors.yang

echo "Starting ConfD"
confd

echo "Loading Data for VNFD"
confd_load -l -m nfv-vnfd.xml

echo "Loading Data for PNFD"
confd_load -l -m nfv-pnfd.xml

echo "Loading Data for NSD"
confd_load -l -m nfv-nsd.xml

echo "Loading Data for descriptors"
confd_load -l -m nfv.xml
echo "Loading data for the complex Vnfd example"
confd_load -l -m complex-vnfd.xml


# Don't do this in the actual test, just waste of cycles
# echo "Stopping ConfD"
# confd --stop
