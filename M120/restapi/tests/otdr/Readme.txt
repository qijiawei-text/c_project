Index             File                            REST 
1        otdr_config_all.json               post /data/optical-otdr/otdrs
2        otdr_config_all_put.json           put /data/optical-otdr/otdrs    
3.       otdr_config.json                   post /data/optical-otdr/otdrs/otdr=OTDR/config 
4.       otdr-p_config.json                 post /data/optical-otdr/otdrs/otdr=OTDR-P/config
5.       otdr_config_put.json               put /data/optical-otdr/otdrs/otdr=OTDR/config 
6.       otdr-p_config_put.json             put /data/optical-otdr/otdrs/otdr=OTDR-P/config                          

-----------------------------------------------------------------------------------
NOTE：
1. NOT supported URL  
get /data/optical-otdr/otdrs/otdr=otdr

2. supported URL (test OK)
get/post/put /data/optical-otdr/otdrs
get /data/optical-otdr/otdrs/otdr=OTDR
get /data/optical-otdr/otdrs/otdr=OTDR-P

get /data/optical-otdr/otdrs/otdr=OTDR/config

get /data/optical-otdr/otdrs/otdr=OTDR/otdr-port=line-in/line-out/external
get /data/optical-otdr/otdrs/otdr=OTDR/pre-config=short/medium/long

get /data/optical-otdr/otdr/sor-files
get /data/optical-otdr/otdr-p/sor-files
mkdir /home/oplink/datastore1/otdr-sors/
mkdir /home/oplink/datastore1/otdr-p-sors/
