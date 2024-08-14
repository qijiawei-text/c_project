1. curl test command
curl -vk -H "Content-Type: application/json" -X POST --data-binary "@login.json" https://192.168.6.223/omt/user/login
curl -vk -H "TOKEN: ee331b86d68b1ee0fbb74cc73129335d2878a2b2dc1b5f8981fba907e9354e2f" -X GET https://172.16.166.244/omt/inventory

********* system *********
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/omt/config
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@system.json" https://172.16.166.244/omt/config/system
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X POST --data-binary "@pre-amp.json" https://172.16.166.244/omt/config/pre-amp
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X POST --data-binary "@booster-amp.json" https://172.16.166.244/omt/config/booster-amp

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X PUT --data-binary "@put_system.json" https://172.16.166.244/omt/config/system
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X PUT --data-binary "@put_pre-amp.json" https://172.16.166.244/omt/config/pre-amp
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X PUT --data-binary "@put_booster-amp.json" https://172.16.166.244/omt/config/booster-amp

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/omt/config/threshold
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@threshold.json" https://172.16.166.244/omt/config/threshold
********* user *********
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/omt/user
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST  --data-binary "@user.json"  https://172.16.166.244/omt/user
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST  --data-binary "@user2.json"  https://172.16.166.244/omt/user
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X DELETE https://172.16.166.244/omt/user/testUser1
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/omt/user/logout

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/omt/remote-node-info
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/omt/current-alarms
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/omt/past-pm-values
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/omt/current-pm-values

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X POST --data-binary "@restart.json" https://172.16.166.244/omt/operations/warmrestart
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X POST --data-binary "@restart.json" https://172.16.166.244/omt/operations/coldrestart

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X POST --data-binary "@download.json" https://172.16.166.244/omt/software/upgrade
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X POST --data-binary "@download.json" https://172.16.166.244/omt/software/download
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X POST --data-binary  "" https://172.16.166.244/omt/software/activate

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X POST --data "" https://172.16.166.244/omt/system/lamptest
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/omt/software/activate

/**************file tranfer directory map talbe*************************
/db-backups  /home/oplink/datastore1/
/log/saved   /home/oplink/log/
/sw-image    /tmp/download/
/otdr-sors   /home/oplink/datastore1/otdr-sors/
***********************************************************************/

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X POST --data-binary "@transfer-file.json" https://172.16.166.244/omt/operations/transfer-file

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/omt/slots
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "" https://172.16.166.244/omt/operations/auto-gain
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/omt/operations/auto-gain



curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@auto-control.json" https://172.16.166.244/omt/config/auto-control
********* ocm *********
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors
*********(1)pre-amp-input*********
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-input
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-input/channels
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-input/config
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-input/state
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-input/channels/channel=196062500,196137500/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-input/channels/channel=196062500,196137500/state
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-input/channels/channel=193887500,193962500/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-input/channels/channel=193887500,193962500/state

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X PUT --data-binary "@ocm_pa_in.json" https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-input/config

*********(2)pre-amp-output*********
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-output
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-output/channels
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-output/config
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-output/state
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-output/channels/channel=196062500,196137500/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-output/channels/channel=196062500,196137500/state
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-output/channels/channel=193887500,193962500/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-output/channels/channel=193887500,193962500/state

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X PUT --data-binary "@ocm_pa_out.json" https://172.16.166.244/data/channel-monitors/channel-monitor=pre-amp-output/config

*********(3)booster-amp-input*********
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-input
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-input/channels
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-input/config
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-input/state
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-input/channels/channel=196062500,196137500/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-input/channels/channel=196062500,196137500/state
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-input/channels/channel=193887500,193962500/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-input/channels/channel=193887500,193962500/state

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X PUT --data-binary "@ba_in_port_config.json" https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-input/config

*********(4)booster-amp-output*********
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-output
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-output/channels
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-output/config
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-output/state
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-output/channels/channel=196062500,196137500/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-output/channels/channel=196062500,196137500/state
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-output/channels/channel=193887500,193962500/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-output/channels/channel=193887500,193962500/state
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-input/channels/channel=193887500,193962500/state

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X PUT --data-binary "@ba_out_port_config.json" https://172.16.166.244/data/channel-monitors/channel-monitor=booster-amp-output/config

*********test not support TLS1.0 and TLS1.1*********
curl -vk --tlsv1.0 -H "Content-Type: application/json" -X POST --data-binary "@login.json" https://172.16.166.244/omt/user/login
curl -vk --tlsv1.1 -H "Content-Type: application/json" -X POST --data-binary "@login.json" https://172.16.166.244/omt/user/login

curl -vk -H "Content-Type: application/json" -X POST --data-binary "@loginerror.json" https://172.16.166.244/omt/user/login
curl -vk -H "Content-Type: application/json" -X POST --data-binary "@tacacs_user_or.json" https://172.16.166.244/omt/user/login
curl -vk -H "Content-Type: application/json" -X POST --data-binary "@tacacs_user_wr.json" https://172.16.166.244/omt/user/login

********************** otdr ***************************************
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/otdr-port=line-in/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/otdr-port=line-out/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/otdr-port=external/

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/otdr-port=line-in/state
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/otdr-port=line-out/state/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/otdr-port=external/state

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/otdr-port=line-in/config
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/otdr-port=line-out/config
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/otdr-port=external/config

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@otdr-all.json" https://172.16.166.244/data/optical-otdr
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@otdr-config.json" https://172.16.166.244/data/optical-otdr/config
[Not Support]curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@otdr-port-all.json" https://172.16.166.244/data/optical-otdr/otdr-port
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X PUT --data-binary "@otdr-line-in-top.json" https://172.16.166.244/data/optical-otdr/otdr-port=line-in
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@otdr-line-in.json" https://172.16.166.244/data/optical-otdr/otdr-port=line-in/config

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/otdr-port=line-in/state/otdr-event=1
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/otdr-port=line-out/state/otdr-event=1
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/otdr-port=external/state/otdr-event=1

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/pre-config=short/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/pre-config=medium/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/pre-config=long/

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-otdr/state/

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@rpc-otdr-scan.json" https://172.16.166.244/operations/otdr-scan/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@rpc-get-trace-data.json" https://172.16.166.244/operations/get-trace-data/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@rpc-trace-baseline.json" https://172.16.166.244/operations/trace-baseline/
********************** edfa ***************************************
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/omt/config
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X PUT --data-binary "@put_pre-amp.json" https://172.16.166.244/omt/config/pre-amp
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X PUT --data-binary "@put_booster-amp.json" https://172.16.166.244/omt/config/booster-amp
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X PUT --data-binary "@put_system.json" https://172.16.166.244/omt/config/system
*********edfa *********
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-amplifier/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-amplifier/amplifiers/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-amplifier/amplifiers/amplifier=BA/config/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X POST --data-binary "@ba_config.json" https://172.16.166.244/data/optical-amplifier/amplifiers/amplifier=BA/config/

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/optical-amplifier/amplifiers/amplifier=PA/config/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X POST --data-binary "@pa_config.json" https://172.16.166.244/data/optical-amplifier/amplifiers/amplifier=PA/config/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -H "Content-Type: application/json" -X POST --data-binary "@full_config.json" https://172.16.166.244/data/optical-amplifier/


********************** system ***************************************
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/system/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/system/authentication/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@user.json" https://172.16.166.244/data/system/authentication/user/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@user.json" https://172.16.166.244/data/system/authentication/user=guangjung_admin/

********************** system ***************************************
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/system/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/system/authentication/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@user.json" https://172.16.166.244/data/system/authentication/user/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X PUT --data-binary "@user.json" https://172.16.166.244/data/system/authentication/user=guangjung_001/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X DELETE  https://172.16.166.244/data/system/authentication/user=guangjung_001/

********************** syslog ***************************************
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/syslog/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@syslog_full.json" https://172.16.166.244/data/syslog/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/syslog/log-actions/remote/destination=1/udp/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@syslog_remote.json" https://172.16.166.244/data/syslog/log-actions/remote/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/syslog/log-actions/remote/destination=1/log-selector/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@syslog_log-selector.json" https://172.16.166.244/data/syslog/log-actions/remote/destination=2/log-selector/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X DELETE  https://172.16.166.244/data/syslog/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X DELETE  https://172.16.166.244/data/syslog/log-actions/remote/destination=1

******************rpc*************
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@led-control_rpc.json" https://172.16.166.244/operations/led-control/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@rpc_save-conf.json" https://172.16.166.244/operations/save-conf/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@rpc_transfer.json" https://172.16.166.244/operations/transfer/

********************** get all ***************************************
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/auth-mgnt/

curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/supervisory-channel/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/supervisory-channel/oscs=OSC1/osc/
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/tacacs
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X GET https://172.16.166.244/data/tacacs/server=2
curl -vk -H "TOKEN: 7d3197417df905628144fde705916b1e56c2fbe8a2aa0e4ee13c3eccd01841db" -X POST --data-binary "@tacacs.json" https://172.16.166.244/data/tacacs/server=2

********************** gnoi otdr ***************************************
curl -vk -H "Content-Type: application/json" -X POST --data-binary "@login.json" https://192.168.6.223/omt/user/login
curl -vk -H "TOKEN: ee331b86d68b1ee0fbb74cc73129335d2878a2b2dc1b5f8981fba907e9354e2f" -X GET https://192.168.6.223/data/optical-time-domain-reflectometer/config-common
curl -vk -H "TOKEN: ee331b86d68b1ee0fbb74cc73129335d2878a2b2dc1b5f8981fba907e9354e2f" -X GET https://192.168.6.223/data/optical-time-domain-reflectometer/config-common/scan-types=short
curl -vk -H "TOKEN: ee331b86d68b1ee0fbb74cc73129335d2878a2b2dc1b5f8981fba907e9354e2f" -X GET https://192.168.6.223/data/optical-time-domain-reflectometer/config-common/scan-types=medium
curl -vk -H "TOKEN: ee331b86d68b1ee0fbb74cc73129335d2878a2b2dc1b5f8981fba907e9354e2f" -X GET https://192.168.6.223/data/optical-time-domain-reflectometer/config-common/scan-types=long
curl -vk -H "TOKEN: ee331b86d68b1ee0fbb74cc73129335d2878a2b2dc1b5f8981fba907e9354e2f" -X GET https://192.168.6.223/data/optical-time-domain-reflectometer/otdrs
curl -vk -H "TOKEN: ee331b86d68b1ee0fbb74cc73129335d2878a2b2dc1b5f8981fba907e9354e2f" -X GET https://192.168.6.223/data/optical-time-domain-reflectometer/otdrs/otdr=line-in
curl -vk -H "TOKEN: ee331b86d68b1ee0fbb74cc73129335d2878a2b2dc1b5f8981fba907e9354e2f" -X GET https://192.168.6.223/data/optical-time-domain-reflectometer/otdrs/otdr=external
curl -vk -H "TOKEN: ee331b86d68b1ee0fbb74cc73129335d2878a2b2dc1b5f8981fba907e9354e2f" -H "Content-Type: application/json" -X POST --data-binary "@set-config.json" https://192.168.6.223/data/optical-time-domain-reflectometer/otdrs/otdr=external/config
curl -vk -H "TOKEN: ee331b86d68b1ee0fbb74cc73129335d2878a2b2dc1b5f8981fba907e9354e2f" -X POST --data-binary "@rpc-otdr-scan.json" https://192.168.6.223/operations/otdr-scan
curl -vk -H "TOKEN: ee331b86d68b1ee0fbb74cc73129335d2878a2b2dc1b5f8981fba907e9354e2f" -X POST --data-binary "@set-baseline.json" https://192.168.6.223/operations/set-baseline