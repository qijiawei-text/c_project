1. curl test command
curl -vk -H "Content-Type: application/json" -X POST --data-binary "@./auth/login.json" https://172.16.166.221/omt/user/login


curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/inventory

********* system *********
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/config
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Expect:" -X POST --data-binary "@./omt_config/fullConfig.json" https://172.16.166.221/omt/config
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@./omt_config/system.json" https://172.16.166.221/omt/config/system
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X POST --data-binary "@./omt_config/pre-amp.json" https://172.16.166.221/omt/config/pre-amp
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X POST --data-binary "@./omt_config/booster-amp.json" https://172.16.166.221/omt/config/booster-amp

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X PUT --data-binary "@./omt_config/put_system.json" https://172.16.166.221/omt/config/system
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X PUT --data-binary "@./omt_config/put_pre-amp.json" https://172.16.166.221/omt/config/pre-amp
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X PUT --data-binary "@./omt_config/put_booster-amp.json" https://172.16.166.221/omt/config/booster-amp

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/config/threshold
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@threshold.json" https://172.16.166.221/omt/config/threshold


curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X PUT --data-binary "@./omt_config/put_tilt.json" https://172.16.166.221/omt/config/pre-amp
********* user *********
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/user
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST  --data-binary "@user/ro_user.json"  https://172.16.166.221/omt/user
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST  --data-binary "@user/wr_user.json"  https://172.16.166.221/omt/user
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X DELETE https://172.16.166.221/omt/user/readonly_001
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/user/logout

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/remote-node-info
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/current-alarms
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/past-pm-values
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/current-pm-values


******************* reset*******************************************
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X POST --data-binary "@restart/restart_amp.json" https://172.16.166.221/omt/operations/warmrestart
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X POST --data-binary "@restart/restart_amp.json" https://172.16.166.221/omt/operations/coldrestart


******************* upgrade *******************************************
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X POST --data-binary "@./upgrade/download.json" https://172.16.166.221/omt/software/upgrade
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X POST --data-binary "@./upgrade/download.json" https://172.16.166.221/omt/software/download
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X POST --data-binary  "" https://172.16.166.221/omt/software/activate

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/software/upgrade
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/software/activate

******************* lamptest *******************************************
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X POST --data "" https://172.16.166.221/omt/system/lamptest
/**************file tranfer directory map talbe*************************
/db-backups  /home/oplink/datastore1/
/log/saved   /home/oplink/log/
/sw-image    /tmp/download/
/otdr-sors   /home/oplink/datastore1/otdr-sors/
***********************************************************************/

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X POST --data-binary "@transfer-file.json" https://172.16.166.221/omt/operations/transfer-file

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/slots
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "" https://172.16.166.221/omt/operations/auto-gain
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/operations/auto-gain



curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@auto-control.json" https://172.16.166.221/omt/config/auto-control
********* ocm *********
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors
*********(1)pre-amp-input*********
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-input
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-input/channels
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-input/config
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-input/state
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-input/channels/channel=196062500,196137500/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-input/channels/channel=196062500,196137500/state
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-input/channels/channel=193887500,193962500/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-input/channels/channel=193887500,193962500/state

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X PUT --data-binary "@ocm_pa_in.json" https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-input/config

*********(2)pre-amp-output*********
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-output
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-output/channels
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-output/config
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-output/state
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-output/channels/channel=196062500,196137500/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-output/channels/channel=196062500,196137500/state
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-output/channels/channel=193887500,193962500/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-output/channels/channel=193887500,193962500/state

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X PUT --data-binary "@ocm_pa_out.json" https://172.16.166.221/data/channel-monitors/channel-monitor=pre-amp-output/config

*********(3)booster-amp-input*********
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-input
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-input/channels
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-input/config
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-input/state
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-input/channels/channel=196062500,196137500/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-input/channels/channel=196062500,196137500/state
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-input/channels/channel=193887500,193962500/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-input/channels/channel=193887500,193962500/state

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X PUT --data-binary "@ba_in_port_config.json" https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-input/config

*********(4)booster-amp-output*********
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-output
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-output/channels
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-output/config
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-output/state
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-output/channels/channel=196062500,196137500/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-output/channels/channel=196062500,196137500/state
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-output/channels/channel=193887500,193962500/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-output/channels/channel=193887500,193962500/state
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-input/channels/channel=193887500,193962500/state

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X PUT --data-binary "@ba_out_port_config.json" https://172.16.166.221/data/channel-monitors/channel-monitor=booster-amp-output/config

*********test not support TLS1.0 and TLS1.1*********
curl -vk --tlsv1.0 -H "Content-Type: application/json" -X POST --data-binary "@login.json" https://172.16.166.221/omt/user/login
curl -vk --tlsv1.1 -H "Content-Type: application/json" -X POST --data-binary "@login.json" https://172.16.166.221/omt/user/login

curl -vk -H "Content-Type: application/json" -X POST --data-binary "@loginerror.json" https://172.16.166.221/omt/user/login
curl -vk -H "Content-Type: application/json" -X POST --data-binary "@tacacs_user_or.json" https://172.16.166.221/omt/user/login
curl -vk -H "Content-Type: application/json" -X POST --data-binary "@tacacs_user_wr.json" https://172.16.166.221/omt/user/login

********************** otdr ***************************************
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/otdr-port=line-in/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/otdr-port=line-out/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/otdr-port=external/

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/otdr-port=line-in/state
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/otdr-port=line-out/state/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/otdr-port=external/state

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/otdr-port=line-in/config
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/otdr-port=line-out/config
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/otdr-port=external/config

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@otdr-all.json" https://172.16.166.221/data/optical-otdr
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@otdr-config.json" https://172.16.166.221/data/optical-otdr/config
[Not Support]curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@otdr-port-all.json" https://172.16.166.221/data/optical-otdr/otdr-port
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X PUT --data-binary "@otdr-line-in-top.json" https://172.16.166.221/data/optical-otdr/otdr-port=line-in
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@otdr-line-in.json" https://172.16.166.221/data/optical-otdr/otdr-port=line-in/config

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/otdr-port=line-in/state/otdr-event=1
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/otdr-port=line-out/state/otdr-event=1
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/otdr-port=external/state/otdr-event=1

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/pre-config=short/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/pre-config=medium/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/pre-config=long/

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-otdr/state/

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@rpc-otdr-scan.json" https://172.16.166.221/operations/otdr-scan/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@rpc-get-trace-data.json" https://172.16.166.221/operations/get-trace-data/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@rpc-trace-baseline.json" https://172.16.166.221/operations/trace-baseline/
********************** edfa ***************************************
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/config
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X PUT --data-binary "@put_pre-amp.json" https://172.16.166.221/omt/config/pre-amp
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X PUT --data-binary "@put_booster-amp.json" https://172.16.166.221/omt/config/booster-amp
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X PUT --data-binary "@put_system.json" https://172.16.166.221/omt/config/system
*********edfa *********
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-amplifier/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-amplifier/amplifiers/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-amplifier/amplifiers/amplifier=BA/config/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X POST --data-binary "@ba_config.json" https://172.16.166.221/data/optical-amplifier/amplifiers/amplifier=BA/config/

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/optical-amplifier/amplifiers/amplifier=PA/config/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X POST --data-binary "@pa_config.json" https://172.16.166.221/data/optical-amplifier/amplifiers/amplifier=PA/config/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X POST --data-binary "@full_config.json" https://172.16.166.221/data/optical-amplifier/


********************** system ***************************************
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/system/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/system/authentication/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@user.json" https://172.16.166.221/data/system/authentication/user/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@user.json" https://172.16.166.221/data/system/authentication/user=guangjung_admin/

********************** system ***************************************
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/system/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/system/authentication/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@user.json" https://172.16.166.221/data/system/authentication/user/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X PUT --data-binary "@user.json" https://172.16.166.221/data/system/authentication/user=guangjung_001/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X DELETE  https://172.16.166.221/data/system/authentication/user=guangjung_001/

********************** syslog ***************************************
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/syslog/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@syslog_full.json" https://172.16.166.221/data/syslog/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/syslog/log-actions/remote/destination=1/udp/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@syslog_remote.json" https://172.16.166.221/data/syslog/log-actions/remote/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/syslog/log-actions/remote/destination=1/log-selector/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@syslog_log-selector.json" https://172.16.166.221/data/syslog/log-actions/remote/destination=2/log-selector/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X DELETE  https://172.16.166.221/data/syslog/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X DELETE  https://172.16.166.221/data/syslog/log-actions/remote/destination=1

******************rpc*************
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@led-control_rpc.json" https://172.16.166.221/operations/led-control/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@rpc_save-conf.json" https://172.16.166.221/operations/save-conf/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@rpc_transfer.json" https://172.16.166.221/operations/transfer/

********************** get all ***************************************
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/auth-mgnt/

curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/supervisory-channel/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/supervisory-channel/oscs=OSC1/osc/
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/tacacs
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/tacacs/server=2
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X POST --data-binary "@tacacs.json" https://172.16.166.221/data/tacacs/server=2

------------------------------ops--------------------------------------------------------------------------------
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/data/aps/aps-modules
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X POST --data-binary "@./ops/ops_config.json" https://172.16.166.221/data/aps/aps-modules/aps-module=OLP/config

------------------------------ipacl--------------------------------------------------------------------------------
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -X GET https://172.16.166.221/omt/system/ipacl
curl -vk -H "TOKEN: 6c3b911cdc23b95d39dcda79718d3dc7ba8409da3b0b50ef37038f215ab633a5" -H "Content-Type: application/json" -X POST --data-binary "@./ipacl/ipcal.json" https://172.16.166.221/omt/system/ipacl