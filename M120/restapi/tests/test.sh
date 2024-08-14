#!/bin/bash
# curl -vk -H "Content-Type: application/json" -X POST --data-binary "@login.json" https://172.16.166.236/omt/user/login
LOGIN_TOKEN="6ff815600651633afc6c4329f2828e84a718b76e7ecd94f0ac9a4471871a18a3"
DEVICE_IP="172.16.166.222"

function rest_curl_cmd()
{
	http_method=$1
	echo "++ $1 $2 $3"
	url_path=$2
	json_config_path=$3
	
	
	if [ "$1" == "get" ]; then
		# GET URL
		curl -vk -H "TOKEN: $LOGIN_TOKEN" -X GET https://$DEVICE_IP$url_path
	elif [ "$1" == "post" ]; then 
	    # POST URL JSON
		if [[ $json_config_path != "" ]]; then 
			
			if [ ! -f "$json_config_path" ]; then
				echo "***************NOT find $json_config_path ********************"
			fi
			curl -vk -H "TOKEN: $LOGIN_TOKEN" -H "Content-Type: application/json" -H "Expect:" -X POST --data-binary "@$json_config_path" https://$DEVICE_IP$url_path
		else
			curl -vk -H "TOKEN: $LOGIN_TOKEN" -H "Content-Type: application/json" -H "Expect:" -X POST --data "" https://$DEVICE_IP$url_path
		fi
	elif [ "$1" == "put" ]; then 
		# POST URL JSON
		if [ ! -f "$json_config_path" ]; then
			echo "***************NOT find $json_config_path ********************"
		fi
		curl -vk -H "TOKEN: $LOGIN_TOKEN" -H "Content-Type: application/json" -H "Expect:" -X PUT --data-binary "@$json_config_path" https://$DEVICE_IP$url_path
	elif [ "$1" == "delete" ]; then 
		# DELETE URL
		curl -vk -H "TOKEN: $LOGIN_TOKEN" -X DELETE https://$DEVICE_IP$url_path
	else
		echo "--Not find: $1 $2 $3 "
        exit -1
	fi
	
	val=`echo $?`
	echo ""
	if test $val -ne 0;then
        echo "-- $1 $2 $3 , Failed"
        exit -1
    fi
	
	echo "-- $1 $2 $3 , Successful"
	#date
	sleep 1
	
}

function test_otdr() {

	rest_curl_cmd "get" "/data/optical-otdr/"
	rest_curl_cmd "post" "/data/optical-otdr/" "./otdr/otdr-all.json"
	rest_curl_cmd "get" "/data/optical-otdr/"
	rest_curl_cmd "post" "/data/optical-otdr/" "./otdr/otdr-all-2.json"
	#rest_curl_cmd "get" "/data/optical-otdr/"
	
	rest_curl_cmd "get" "/data/optical-otdr/config/"
	rest_curl_cmd "post" "/data/optical-otdr/config/" "./otdr/otdr-config.json"
	rest_curl_cmd "get" "/data/optical-otdr/config/"
	rest_curl_cmd "post" "/data/optical-otdr/config/" "./otdr/otdr-config-2.json"
	#rest_curl_cmd "get" "/data/optical-otdr/config/"
	
	rest_curl_cmd "get" "/data/optical-otdr/otdr-port=line-in/"
	rest_curl_cmd "put" "/data/optical-otdr/otdr-port=line-in/" "./otdr/otdr-line-in-top.json"
	rest_curl_cmd "get" "/data/optical-otdr/otdr-port=line-in/"
	rest_curl_cmd "put" "/data/optical-otdr/otdr-port=line-in/" "./otdr/otdr-line-in-top-2.json"
	#rest_curl_cmd "get" "/data/optical-otdr/otdr-port=line-in/"
	
	rest_curl_cmd "get" "/data/optical-otdr/otdr-port=line-in/config/"
	rest_curl_cmd "put" "/data/optical-otdr/otdr-port=line-in/config/" "./otdr/otdr-line-in.json"
	rest_curl_cmd "get" "/data/optical-otdr/otdr-port=line-in/config/"
	rest_curl_cmd "put" "/data/optical-otdr/otdr-port=line-in/config/" "./otdr/otdr-line-in-2.json"
	#rest_curl_cmd "get" "/data/optical-otdr/otdr-port=line-in/config/"
	
	rest_curl_cmd "get" "/data/optical-otdr/pre-config=short/"
	rest_curl_cmd "get" "/data/optical-otdr/pre-config=medium/"
	rest_curl_cmd "get" "/data/optical-otdr/pre-config=long/"
	
	rest_curl_cmd "post" "/operations/otdr-scan/" "./otdr/rpc-otdr-scan-external.json"
	rest_curl_cmd "post" "/operations/otdr-scan/" "./otdr/rpc-otdr-scan-line-in.json"
	rest_curl_cmd "post" "/operations/otdr-scan/" "./otdr/rpc-otdr-scan-line-out.json"
	
	echo "waiting 10s..."
	sleep 10
	#rest_curl_cmd "get" "/data/optical-otdr/otdr-port=line-in/state/"
	rest_curl_cmd "get" "/data/optical-otdr/otdr-port=line-out/state/"
	rest_curl_cmd "get" "/data/optical-otdr/"
	
	rest_curl_cmd "get" "/data/optical-otdr/state/"
	
	rest_curl_cmd "get" "/data/optical-otdr/"
	
	rest_curl_cmd "post" "/operations/trace-baseline" "./otdr/rpc-trace-baseline-get.json"
	rest_curl_cmd "post" "/operations/trace-baseline" "./otdr/rpc-trace-baseline-set.json"
	
}

function test_omt_config() {
	
	echo "**********************begin: test_omt_config ********************"
	rest_curl_cmd "get" "/omt/config"
	rest_curl_cmd "post" "/omt/config" "./omt_config/fullConfig.json"
	rest_curl_cmd "get" "/omt/config"
	rest_curl_cmd "post" "/omt/config" "./omt_config/fullConfig-2.json"
	rest_curl_cmd "get" "/omt/config"
	
	rest_curl_cmd "post" "/omt/config/system" "./omt_config/system.json"
	rest_curl_cmd "post" "/omt/config/pre-amp" "./omt_config/pre-amp.json"
	rest_curl_cmd "post" "/omt/config/booster-amp" "./omt_config/booster-amp.json"
	rest_curl_cmd "get" "/omt/config"
	
	rest_curl_cmd "post" "/omt/config/system" "./omt_config/system-2.json"
	rest_curl_cmd "post" "/omt/config/pre-amp" "./omt_config/pre-amp-2.json"
	rest_curl_cmd "post" "/omt/config/booster-amp" "./omt_config/booster-amp-2.json"
	rest_curl_cmd "get" "/omt/config"
	
	rest_curl_cmd "put" "/omt/config/system" "./omt_config/put_system.json"
	rest_curl_cmd "put" "/omt/config/system" "./omt_config/put_system-2.json"
	
	rest_curl_cmd "put" "/omt/config/pre-amp" "./omt_config/put_pre-amp.json"
	rest_curl_cmd "put" "/omt/config/pre-amp" "./omt_config/put_pre-amp-2.json"
	
	rest_curl_cmd "get" "/omt/inventory"
	
	echo "**********************end: test_omt_config ********************"
}

function test_omt_system() {

	rest_curl_cmd "get" "/omt/system/apsd"
	rest_curl_cmd "post" "/omt/system/apsd" "./omt_system/apsd.json"
	rest_curl_cmd "get" "/omt/system/apsd"
	rest_curl_cmd "post" "/omt/system/apsd" "./omt_system/apsd-2.json"
	rest_curl_cmd "get" "/omt/system/apsd" 

	rest_curl_cmd "post" "/omt/system/clear-auto-alarms" ""
	
	rest_curl_cmd "post" "/omt/system/lamptest" ""
	
}


function test_mux() {

	rest_curl_cmd "get" "/data/optical-mux/config"
	rest_curl_cmd "post" "/data/optical-mux/config" "./mux/mux-config.json"
	rest_curl_cmd "get" "/data/optical-mux/config"
	rest_curl_cmd "post" "/data/optical-mux/config" "./mux/mux-config-2.json"
	rest_curl_cmd "get" "/data/optical-mux/config"

	rest_curl_cmd "get" "/data/optical-mux/state"

}

function test_ipacl() {

	rest_curl_cmd "get" "/omt/system/ipacl"
	rest_curl_cmd "post" "/omt/system/ipacl" "./ipacl/ipacl.json"
	rest_curl_cmd "get" "/omt/system/ipacl"
	rest_curl_cmd "post" "/omt/system/ipacl" "./ipacl/ipacl-2.json"
	rest_curl_cmd "get" "/omt/system/ipacl"

	rest_curl_cmd "post" "/omt/system/ipacl" "./ipacl/ipacl-empty1.json"
        rest_curl_cmd "get" "/omt/system/ipacl"
	rest_curl_cmd "post" "/omt/system/ipacl" "./ipacl/ipacl-2.json"
        rest_curl_cmd "get" "/omt/system/ipacl"

	rest_curl_cmd "post" "/omt/system/ipacl" "./ipacl/ipacl-empty2.json"
        rest_curl_cmd "get" "/omt/system/ipacl"
        rest_curl_cmd "post" "/omt/system/ipacl" "./ipacl/ipacl-1.json"
        rest_curl_cmd "get" "/omt/system/ipacl"

	 rest_curl_cmd "post" "/omt/system/ipacl" "./ipacl/ipacl-empty3.json"
	 rest_curl_cmd "get" "/omt/system/ipacl"
         rest_curl_cmd "post" "/omt/system/ipacl" "./ipacl/ipacl-1.json"
         rest_curl_cmd "get" "/omt/system/ipacl"


}

function test_pm() {

	rest_curl_cmd "get" "/omt/current-pm-values"
}

function test_ocm() {
	rest_curl_cmd "get" "/data/channel-monitors/channel-monitor=pre-amp-input/config"
	rest_curl_cmd "get" "/data/channel-monitors/channel-monitor=pre-amp-output/config"
	rest_curl_cmd "get" "/data/channel-monitors/channel-monitor=booster-amp-input/config"
	rest_curl_cmd "get" "/data/channel-monitors/channel-monitor=booster-amp-output/config"
	
	rest_curl_cmd "put" "/data/channel-monitors/channel-monitor=pre-amp-input/config" "./ocm/ocm_pa_in.json"
	rest_curl_cmd "put" "/data/channel-monitors/channel-monitor=pre-amp-output/config" "./ocm/ocm_pa_out.json"
	#rest_curl_cmd "put" "/data/channel-monitors/channel-monitor=booster-amp-input/config"
	#rest_curl_cmd "put" "/data/channel-monitors/channel-monitor=booster-amp-output/config"
	
	rest_curl_cmd "get" "/data/channel-monitors/config"
	rest_curl_cmd "post" "/data/channel-monitors/config" "./ocm/ocm_mode_auto.json"
	rest_curl_cmd "get" "/data/channel-monitors/state"
	rest_curl_cmd "post" "/data/channel-monitors/config" "./ocm/ocm_mode_75G.json"
	rest_curl_cmd "post" "/data/channel-monitors/config" "./ocm/ocm_mode_150G.json"
	rest_curl_cmd "get" "/data/channel-monitors/state"
	
}

function test_otdr_timeout_question() {
	# scan-type == long
    rest_curl_cmd "get" "/data/optical-otdr/"
	echo "*********set long****"
	rest_curl_cmd "post" "/data/optical-otdr/config/" "./otdr/otdr_timeout_question/otdr-config-1.json"
	rest_curl_cmd "post" "/operations/otdr-scan/" "./otdr/rpc-otdr-scan-external.json"
	rest_curl_cmd "get" "/data/optical-otdr/"
	
	echo "[1]waiting 70s..."
	sleep 70
	# scan-type == short
	rest_curl_cmd "get" "/data/optical-otdr/"
	echo "*********set short****"
	rest_curl_cmd "post" "/data/optical-otdr/config/" "./otdr/otdr_timeout_question/otdr-config-2.json"
	rest_curl_cmd "post" "/operations/otdr-scan/" "./otdr/rpc-otdr-scan-line-out.json"
	rest_curl_cmd "get" "/data/optical-otdr/"
	
	
	echo "[2]waiting 70s..."
	sleep 70
	# scan-type == medium
	echo "*********set medium****"
	rest_curl_cmd "get" "/data/optical-otdr/"
	rest_curl_cmd "post" "/data/optical-otdr/config/" "./otdr/otdr_timeout_question/otdr-config-3.json"
	rest_curl_cmd "post" "/operations/otdr-scan/" "./otdr/rpc-otdr-scan-line-in.json"
	rest_curl_cmd "get" "/data/optical-otdr/"
	
	echo "[3]waiting 70s..."
	sleep 70
	rest_curl_cmd "get" "/data/optical-otdr/"

}
function test_transfer_file() {
	rest_curl_cmd "post" "/omt/operations/transfer-file" "./tranfer_file/tranfer-otdr-baseline_line-out-sor.json"
	rest_curl_cmd "post" "/omt/operations/transfer-file" "./tranfer_file/tranfer-otdr-external-sor.json"
	
}
function test_ocm_osa() {
	rest_curl_cmd "get" "/data/ocm/osa=pre-amp-input/raw-data"
	rest_curl_cmd "get" "/data/ocm/osa=pre-amp-output/raw-data"
	rest_curl_cmd "get" "/data/ocm/osa=booster-amp-input/raw-data"
	rest_curl_cmd "get" "/data/ocm/osa=booster-amp-output/raw-data"
	rest_curl_cmd "get" "/data/ocm/osa=external/raw-data"
}

function rest_login() {
        login_ok="HTTP/1.1 200 OK"
        login_response_txt="login_response.txt"
		http_ok_flag=0
        login_cmd=$(curl -vk -H "Content-Type: application/json" -X POST --data-binary "@./auth/login.json" https://$DEVICE_IP/omt/user/login 2>&1 1>$login_response_txt)
        #login_cmd=`curl -vk -H "Content-Type: application/json" -X POST --data-binary "@login.json" https://$DEVICE_IP/omt/user/login `
        #excute_login_cmd=`echo $login_cmd`
        login_cmd_result=`echo "$login_cmd" >$login_response_txt`
		
        #echo "********************$login_cmd_result**333***"
        if [ ! -f "$login_response_txt" ]; then
                echo "****** Login failed: NOT find $login_response_txt ******"
                exit 1
        fi
        sleep 1
        
		#cat login_response.txt | while read line
	    while read line
		do
			#echo "********111************"
			#echo "$line"
			
			if [ $http_ok_flag -eq 1 ]
			then
				result=$(echo $line | grep "< TOKEN: ")
				
				if [[ "$result" != "" ]]
				then
				   #echo "result:$result, Find: $line"
				   LOGIN_TOKEN=`echo "${line##*< TOKEN: }"`
				   LOGIN_TOKEN=${LOGIN_TOKEN:0:64}
				fi
			fi
			
			result=$(echo $line | grep "< HTTP/1.1 200 OK")
			if [[ "$result" != "" ]]
			then
			   #echo "result:$result, Find: $line"
			   http_ok_flag=1
			fi
			
		done < login_response.txt
		echo "http_ok_flag:$http_ok_flag"
		if [ $http_ok_flag -eq 0 ]
		then
			echo "********Login fail********"
			exit 0
		fi
		
		rm -f login_response.txt
}

function auto_loop_test() {
	while true
	do 	
		test_otdr
		test_omt_config
		test_omt_system
		test_mux
		test_ipacl
		test_pm 
		test_ocm
		#break
	done
}

function auto_control_test() {

    # fix mantis-
	#1. BA InputCBandPower=-3.44dBm (carriers from 5 to 65), to test local and remote carries number are different
    #2. BA InputCBandPower=0.5dBm (carriers from 18 to 40), to test remote PA gain qeustion

	n=1
	while [ $n -lt 20 ]
	do
		# set carries from 40 to 18
		echo "++++++++++++ $n  ++++++++++++"
		cat "./auto_control/auto_config.json"
		rest_curl_cmd "put"  "/omt/config/system" "./auto_control/auto_config.json"
	    rest_curl_cmd "post" "/omt/operations/auto-gain" ""

		sleep 17
		rest_curl_cmd "get"  "/omt/operations/auto-gain"
		echo "------------ $n ------------"
		
		echo "------------ [try again]: $n ------------"
		rest_curl_cmd "post" "/omt/operations/auto-gain" ""

		sleep 17
		rest_curl_cmd "get"  "/omt/operations/auto-gain"
		
		let n++
		
		echo "++++++++++++ $n  ++++++++++++"
		cat "./auto_control/auto_config02.json"
		rest_curl_cmd "put"  "/omt/config/system" "./auto_control/auto_config02.json"
	    rest_curl_cmd "post" "/omt/operations/auto-gain" ""

		sleep 17
		rest_curl_cmd "get"  "/omt/operations/auto-gain"
		echo "------------ $n ------------"
	
		echo "------------ [try again]: $n ------------"
		rest_curl_cmd "post" "/omt/operations/auto-gain" ""

		sleep 17
		rest_curl_cmd "get"  "/omt/operations/auto-gain"
		
		let n++
	done

}


function auto_control_test02() {

    # fix mantis-
	#1. BA InputCBandPower=-3.44dBm (carriers from 5 to 65), to test local and remote carries number are different
    #2. BA InputCBandPower=0.5dBm (carriers from 18 to 40), to test remote PA gain qeustion

	n=1
	while [ $n -lt 30 ]
	do
		# set carries from 40 to 18
		echo "++++++++++++ $n  ++++++++++++"
		cat "./auto_control/auto_config.json"
		rest_curl_cmd "put"  "/omt/config/system" "./auto_control/auto_config.json"
	    rest_curl_cmd "post" "/omt/operations/auto-gain" ""

		sleep 15
		rest_curl_cmd "get"  "/omt/operations/auto-gain"
		echo "------------ $n ------------"

		let n++
		
		echo "++++++++++++ $n  ++++++++++++"
		cat "./auto_control/auto_config02.json"
		rest_curl_cmd "put"  "/omt/config/system" "./auto_control/auto_config02.json"
	    rest_curl_cmd "post" "/omt/operations/auto-gain" ""

		sleep 15
		rest_curl_cmd "get"  "/omt/operations/auto-gain"
		echo "------------ $n ------------"
		
		let n++
	done

}

function test_openconfig_otdr() {
	rest_curl_cmd "get" "/data/optical-time-domain-reflectometer/config-common"
	rest_curl_cmd "get" "/data/optical-time-domain-reflectometer/config-common/fiber-profile"
	rest_curl_cmd "get" "/data/optical-time-domain-reflectometer/config-common/scan-types=short"
	rest_curl_cmd "get" "/data/optical-time-domain-reflectometer/config-common/scan-types=medium"
	rest_curl_cmd "get" "/data/optical-time-domain-reflectometer/config-common/scan-types=long"
	
	# NOT get state data, why? TODO
	rest_curl_cmd "get" "/data/optical-time-domain-reflectometer/otdrs/otdr=line-in"
	rest_curl_cmd "get" "/data/optical-time-domain-reflectometer/otdrs/otdr=line-out"
	rest_curl_cmd "get" "/data/optical-time-domain-reflectometer/otdrs/otdr=external"
	#rest_curl_cmd "get" "/data/optical-time-domain-reflectometer/otdrs/otdr=external/state"

	rest_curl_cmd "post" "/operations/otdr-scan" "./oc-otdr/rpc-otdr-scan.json"

	# failed (TODO)
	rest_curl_cmd "post" "/operations/set-baseline" "./oc-otdr/set-baseline.json"
}

function test_openconfig_ocs() {
# POST - add
# PUT  - replace
    
	#rest_curl_cmd "delete" "/data/connections" 
	rest_curl_cmd "get" "/data/connections"    
	#rest_curl_cmd "post" "/data/connections" "./oc-connections/set_connections_16x16.json"

	#rest_curl_cmd "put" "/data/connections" "./oc-connections/update_connections.json"
    
	
	#rest_curl_cmd "post" "/data/connections/connection" "./oc-connections/set_one_connection.json"

	#rest_curl_cmd "get" "/data/connections/connection=20"
	#rest_curl_cmd "get" "/data/connections/connection=20/config"
	#rest_curl_cmd "get" "/data/connections/connection=20/state"
    
	#rest_curl_cmd "delete" "/data/connections/connection=20"

	#rest_curl_cmd "post" "/data/connections/connection=30/config" "./oc-connections/set_one_connection_config.json"
	#rest_curl_cmd "delete" "/data/connections/connection=30/config" 

	# Unsupported API
	# put /data/connections/connection={index}
	# put /data/connections/connection={index}/config

}

function test_ocs_basefunction() {
	
	echo "**********************begin: test_ocs_basefunction ********************"
	rest_curl_cmd "get" "/omt/config"
	rest_curl_cmd "post" "/omt/config" "./omt_config/fullConfig-ocs.json"
	rest_curl_cmd "get" "/omt/config"
	#rest_curl_cmd "post" "/omt/config" "./omt_config/fullConfig-2.json"
	#rest_curl_cmd "get" "/omt/config"
	
	#rest_curl_cmd "post" "/omt/config/system" "./omt_config/system.json"
	#rest_curl_cmd "get" "/omt/config"
	
	#rest_curl_cmd "get" "/omt/inventory"
	
	echo "**********************end: test_ocs_basefunction ********************"
}

function test_oc_amplifier() {
	
	echo "**********************begin: test_oc_amplifier ********************"
	#rest_curl_cmd "get" "/data/optical-amplifier"
	#rest_curl_cmd "post" "/data/optical-amplifier" "./oc-amplifier/optical-amplifier-top.json"
	#rest_curl_cmd "post" "/data/optical-amplifier" "./oc-amplifier/optical-amplifier-top-02.json"

	#rest_curl_cmd "post" "/data/optical-amplifier/amplifiers" "./oc-amplifier/amplifiers.json"
	#rest_curl_cmd "post" "/data/optical-amplifier/amplifiers" "./oc-amplifier/amplifiers-02.json"
	#rest_curl_cmd "get" "/data/optical-amplifier/amplifiers"

	#rest_curl_cmd "get" "/data/optical-amplifier/amplifiers/amplifier=PA"
	#rest_curl_cmd "post" "/data/optical-amplifier/amplifiers/amplifier=PA/config" "./oc-amplifier/amplifier-pa-config.json"
	rest_curl_cmd "get" "/data/optical-amplifier/amplifiers/amplifier=PA/config"
	rest_curl_cmd "get" "/data/optical-amplifier/amplifiers/amplifier=PA/state"

	#rest_curl_cmd "get" "/data/optical-amplifier/amplifiers/amplifier=BA"
	#rest_curl_cmd "post" "/data/optical-amplifier/amplifiers/amplifier=BA/config" "./oc-amplifier/amplifier-ba-config.json"
	rest_curl_cmd "get" "/data/optical-amplifier/amplifiers/amplifier=BA/config"
	rest_curl_cmd "get" "/data/optical-amplifier/amplifiers/amplifier=BA/state"

    #rest_curl_cmd "get" "/data/optical-amplifier/amplifiers/amplifier=OSC"
	#rest_curl_cmd "post" "/data/optical-amplifier/amplifiers/amplifier=OSC/config" "./oc-amplifier/amplifier-osc-config.json"
	rest_curl_cmd "get" "/data/optical-amplifier/amplifiers/amplifier=OSC/config"
	rest_curl_cmd "get" "/data/optical-amplifier/amplifiers/amplifier=OSC/state"

	#rest_curl_cmd "get" "/data/optical-amplifier/supervisory-channels"
	#rest_curl_cmd "post" "/data/optical-amplifier/supervisory-channels" "./oc-amplifier/supervisory-channels.json"
    #rest_curl_cmd "get" "/data/optical-amplifier/supervisory-channels/supervisory-channel=OSC"
	#rest_curl_cmd "post" "/data/optical-amplifier/supervisory-channels/supervisory-channel=OSC/config" "./oc-amplifier/supervisory-channel-osc.json"
	#rest_curl_cmd "get" "/data/optical-amplifier/supervisory-channels/supervisory-channel=OSC/config"


	rest_curl_cmd "get" "/data/optical-amplifier/supervisory-channels/supervisory-channel=OSC/state"

	echo "**********************end: test_oc_amplifier ********************"
}

function test_dfb() {
	
	echo "**********************begin: test_dfb ********************"
	#rest_curl_cmd "get" "/data/optical-dfb"
	#rest_curl_cmd "get" "/data/optical-dfb/dfbs"
	#rest_curl_cmd "get" "/data/optical-dfb/dfbs/dfb=dfb-hp"
	#rest_curl_cmd "get" "/data/optical-dfb/dfbs/dfb=dfb-hp/config"
	#rest_curl_cmd "get" "/data/optical-dfb/dfbs/dfb=dfb-hp/state"
	
    #rest_curl_cmd "post" "/data/optical-dfb" "./dfb/dfb-top.json"
	#rest_curl_cmd "get" "/data/optical-dfb/dfbs/dfb=dfb-hp/config"
	#rest_curl_cmd "post" "/data/optical-dfb" "./dfb/dfb-top-02.json"
	#rest_curl_cmd "get" "/data/optical-dfb/dfbs/dfb=dfb-hp/config"

    #rest_curl_cmd "post" "/data/optical-dfb/dfbs/dfb=dfb-hp/config" "./dfb/dfb-config.json"
	rest_curl_cmd "post" "/data/optical-dfb/dfbs" "./dfb/dfbs.json"
	rest_curl_cmd "get" "/data/optical-dfb/dfbs/dfb=dfb-hp/config"
	echo "**********************end: test_dfb ********************"
}

function test_oc_attenuator() {
	
	echo "**********************begin: test_oc_attenuator ********************"
	#rest_curl_cmd "get" "/data/optical-attenuator"
	#rest_curl_cmd "post" "/data/optical-attenuator" "./oc-attenuator/optical-attenuator-top.json"
	#rest_curl_cmd "get" "/data/optical-attenuator/attenuators"
	#rest_curl_cmd "post" "/data/optical-attenuator/attenuators" "./oc-attenuator/attenuators.json"

	#rest_curl_cmd "get" "/data/optical-attenuator/attenuators/attenuator=PADDING-VOA"
	#rest_curl_cmd "post" "/data/optical-attenuator/attenuators/attenuator=PADDING-VOA/config" "./oc-attenuator/attenuator-padding-voa-config.json"
	#rest_curl_cmd "get" "/data/optical-attenuator/attenuators/attenuator=PADDING-VOA/config"
	#rest_curl_cmd "get" "/data/optical-attenuator/attenuators/attenuator=PADDING-VOA/state"
	
    #rest_curl_cmd "get" "/data/optical-attenuator/attenuators/attenuator=PA-VOA"
	#rest_curl_cmd "post" "/data/optical-attenuator/attenuators/attenuator=PA-VOA/config" "./oc-attenuator/attenuator-pa-voa-config.json"
	#rest_curl_cmd "get" "/data/optical-attenuator/attenuators/attenuator=PA-VOA/config"
	#rest_curl_cmd "get" "/data/optical-attenuator/attenuators/attenuator=PA-VOA/state"


    #rest_curl_cmd "get" "/data/optical-attenuator/attenuators/attenuator=OSC-TX-VOA"
	#rest_curl_cmd "post" "/data/optical-attenuator/attenuators/attenuator=OSC-TX-VOA/config" "./oc-attenuator/attenuator-osc-tx-voa-config.json"
	#rest_curl_cmd "get" "/data/optical-attenuator/attenuators/attenuator=OSC-TX-VOA/config"
	#rest_curl_cmd "get" "/data/optical-attenuator/attenuators/attenuator=OSC-TX-VOA/state"
    
	# NOT support to set OSC-VOA
	rest_curl_cmd "get" "/data/optical-attenuator/attenuators/attenuator=OSC-VOA"
	rest_curl_cmd "get" "/data/optical-attenuator/attenuators/attenuator=OSC-VOA/state"

	echo "**********************end: test_oc_attenuator ********************"
}

function test_laser_safty() {
	
	#echo "**********************begin: test_laser_safty ********************"
	rest_curl_cmd "get" "/data/laser-safty"
	rest_curl_cmd "get" "/data/laser-safty/config"
	rest_curl_cmd "get" "/data/laser-safty/state"

	#rest_curl_cmd "put" "/data/laser-safty" "./laser-safty/laser-safty-top.json"
    #rest_curl_cmd "get" "/data/laser-safty"
	#rest_curl_cmd "put" "/data/laser-safty/config" "./laser-safty/laser-safty-config.json"
	#rest_curl_cmd "get" "/data/laser-safty"
	#echo "**********************end: test_laser_safty ********************"
}
function manual_test() {
	#rest_curl_cmd "post" "/omt/user/login" "login.json"
	
	#add test case
	#test_otdr
	#test_omt_config
	#test_omt_system
	#test_mux
	#test_ipacl
	#test_pm 
	#test_ocm
	#test_otdr_timeout_question
	#test_ocm_osa
	#test_openconfig_otdr
	#auto_control_test
	#auto_control_test02
	#test_openconfig_ocs
	#test_ocs_basefunction
	#test_dfb
	#test_oc_amplifier
	#test_oc_attenuator
	test_laser_safty
}

function main_entry() {
	#################login command####################
	rest_login 
	echo "LOGIN_TOKEN:$LOGIN_TOKEN"
	
	manual_test

	#################logout command####################
	rest_curl_cmd "get" "/omt/user/logout"

}


#excute entry
main_entry


exit 0
