# opsd - Optical Protecton Switch Daemon.
1) openconfig-line-transport-protection.yang backend, i.e., callbacks for config change and data provider.
2) For bidirectional APS, passing the APS protocol message for OPS modules between local and remote node via OSC.
3) Compatible with unidirectional and bidirectional APS.

#Design Notes:
 - Move sysrepo callbacks from csmd
 - Define ASP OCS message in GPB proto
 - Threads to send/receive APS periodical messages to/from remote node with 5 seconds interval. This is similar to auto-control process.
