#!/bin/bash

SUB_TYPE=$1

POST=$SUB_TYPE

if [[ "$SUB_TYPE" == "OLSP"* ]] || [[ "$SUB_TYPE" == "NR1004" ]]; then
  /usr/local/bin/sysrepoctl -u ietf-snmp;
fi

exit 0
