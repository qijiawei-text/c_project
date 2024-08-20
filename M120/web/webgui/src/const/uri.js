export default {
     GETSYSINFO:     { 'type': 'get', 'uri': '/omt/config' },
     GETSLOTSINFO:   { 'type': 'get', 'uri': '/omt/slots/' } ,
     GETCLOCKINFO:   { 'type': 'get', 'uri': '/data/system/clock/' } ,
     SETCLOCKINFO:   { 'type': 'post','uri': '/data/system/clock/' } ,
     GETNTPINFO:     { 'type': 'get', 'uri': '/data/system/ntp/' } ,
     SETNTPINFO:     { 'type': 'post','uri': '/data/system/ntp/' } ,
     GETSYSTEMSTATE: { 'type': 'get', 'uri': '/data/system-state/' } ,
     GETSYSTEMCONFIG:{ 'type': 'get', 'uri': '/data/system/' } ,
     SETSYSTEMCONFIG:{ 'type': 'post', 'uri': '/data/system/' } ,
     GETRADIUS      :{ 'type': 'get', 'uri': '/data/system/radius/' } ,
     SETRADIUS      :{ 'type': 'post', 'uri': '/data/system/radius/' } ,
     SETRADIUSSERVER:{ 'type': 'post', 'uri': '/data/system/radius/server/' } ,
     DELRADIUSSERVER:{ 'type': 'delete', 'uri': '/data/system/radius/server/' } ,

     GETTACACS      :{ 'type': 'get', 'uri': '/data/system/' } ,
     SETTACACS      :{ 'type': 'post', 'uri': '/data/system/' } ,
     SETCURDATETIME :{ 'type': 'post', 'uri': '/operations/set-current-datetime/' } ,
     SETRESTART     :{ 'type': 'post', 'uri': '/operations/system-restart/' } ,
     SETSHUTDOWN    :{ 'type': 'post', 'uri': '/operations/system-shutdown/' } ,

     GETUSERLIST    :{ 'type': 'get', 'uri': '/data/system/authentication/' } ,
     USERADD        :{ 'type': 'put', 'uri': '/data/system/authentication/' } , //POST /data/system/authentication/user/
     USERDEL        :{ 'type': 'delete', 'uri': '/data/system/authentication/' } ,
     USEREDIT       :{ 'type': 'put', 'uri': '/data/system/authentication/' },
     GETIPCONFIG    :{ 'type': 'get', 'uri': '/data/org-openroadm-device/info/ip-interfaces/' },
    
     GETOALIST      :{ 'type': 'get', 'uri': '/data/optical-amplifier/amplifiers/' },
     SETOALIST      :{ 'type': 'post', 'uri': '/data/optical-amplifier/amplifiers/' },
     GETOA           :{ 'type': 'get', 'uri': '/data/optical-amplifier/amplifiers/amplifier=' },
     SETOA           :{ 'type': 'put', 'uri': '/data/optical-amplifier/amplifiers/amplifier=' },

     GETDEVICE       :{ 'type': 'get', 'uri': '/data/org-openroadm-device/' },

     GETACTIVEALARM  :{ 'type': 'get', 'uri': '/data/active-alarm-list/' },

     SETDEFAULT      :{ 'type': 'post', 'uri': '/operations/restore-default/' },
     SETSAVECONF     :{ 'type': 'post', 'uri': '/operations/save-conf/' },

     GETSYSLOGLIST   :{ 'type': 'get', 'uri': '/data/syslog/' },
     SETSYSLOG       :{ 'type': 'put', 'uri': '/data/syslog/log-actions/remote/destination=' }, 
     DELSYSLOG       :{ 'type': 'delete', 'uri': '/data/syslog/log-actions/remote/destination=' }, 

     GETOCMLIST      :{ 'type': 'get', 'uri': '/data/channel-monitors' },
     GETOCMCFG       :{ 'type': 'get', 'uri': '/data/channel-monitors/channel-monitor={name}/config/' },
     SETOCMCFG       :{ 'type': 'post', 'uri': '/data/channel-monitors/channel-monitor={name}/config/' },
     GETOCMCHANNELS  :{ 'type': 'get', 'uri': '/data/channel-monitors/channel-monitor={name}/channels/' },
   
     GETVOALIST      :{ 'type': 'get', 'uri': '/data/channe/data/optical-attenuator/' },
     GETVOA          :{ 'type': 'get', 'uri': '/data/optical-attenuator/attenuators/attenuator={name}/' },
     SETVOA          :{ 'type': 'post', 'uri': '/data/optical-attenuator/attenuators/attenuator={name}/' },
     SETVOACFG       :{ 'type': 'post', 'uri': '/data/optical-attenuator/attenuators/attenuator={name}/config/' },

     GETOTDR         :{ 'type': 'get', 'uri': '/data/optical-otdr/' },
     GETOTDRPORT     :{ 'type': 'get', 'uri': '/data/optical-otdr/otdr-port={port-name}/' }, 
     GETOTDRCFG      :{ 'type': 'get', 'uri': '/data/optical-otdr/config/' },
     SETOTDR         :{ 'type': 'post', 'uri': '/data/optical-otdr/' },
     SETOTDRPORTCFG  :{ 'type': 'post', 'uri': '/data/optical-otdr/otdr-port={port-name}/' },
     SETOTDRCFG      :{ 'type': 'post', 'uri': '/data/optical-otdr/config/' },

     GETOPTICALMODULE:{ 'type': 'get', 'uri': '/data/optical-control/optical-modules/' },

     GETOSCLIST:      { 'type': 'get', 'uri': '/data/supervisory-channel/' },
     GETOSC:          { 'type': 'get', 'uri': '/data/supervisory-channel/oscs={name}/osc/' },
     SETOSC:          { 'type': 'post', 'uri': '/data/supervisory-channel/oscs={name}/osc/' },
     GETOSCCFG:       { 'type': 'get', 'uri': '/data/supervisory-channel/oscs={name}/osc/osc-tx-config/' },  
     SETOSCCFG:       { 'type': 'post', 'uri': '/data/supervisory-channel/oscs={name}/osc/osc-tx-config/' },
   
     SETDBBACKUP:     { 'type': 'post', 'uri': '/operations/db-backup/' },
     SETDBRESTORE:    { 'type': 'post', 'uri': '/operations/db-restore/' },
     SETDBACTIVATE:   { 'type': 'post', 'uri': '/operations/db-activate/' },
     SETDBINIT:       { 'type': 'post', 'uri': '/operations/database-init/' },
     SETCANCELROLLBACKTIMER: { 'type': 'post', 'uri': '/operations/cancel-rollback-timer/' },
  
     SETDELETEFILE:   { 'type': 'post', 'uri': '/operations/delete-file/' },
     SETSHOWFILE:     { 'type': 'post', 'uri': '/operations/show-file/' },
     SETTRANSFER:     { 'type': 'post', 'uri': '/operations/transfer/' },     
    }