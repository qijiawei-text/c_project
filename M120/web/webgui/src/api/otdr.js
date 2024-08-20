import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function getOtdr() {
    let results = apiGet(uri.GETOTDR.type, uri.GETOTDR.uri);
    
    console.log("getOtdrList results=",results);
    return results;
}

// Get /data/optical-otdr/otdr-port={port-name}/
export function getOtdrPort(data) {
  console.log("getOtdrPort data=",data);
  // const url = uri.GETOCMCFG.uri.replace("{}","{"+data+"}")
  const url = uri.GETOTDRPORT.uri.replace("{port-name}",data)
  console.log("getOtdrPort url = ",url);
  let results = apiGet(uri.GETOTDRPORT.type, url);
  
  console.log("getOtdrPort results=",results);
  return results;
}

export function getOtdrCfg(data) {
  console.log("getOtdrCfg data=",data);
  // const url = uri.GETOCMCFG.uri.replace("{}","{"+data+"}")
  const url = uri.GETOTDRCFG.uri.replace("{port-name}",data)
  console.log("getOtdrCfg url = ",url);
  let results = apiGet(uri.GETOTDRCFG.type, url);
  
  console.log("getOtdrCfg results=",results);
  return results;
}

export function setOtdr(data) {
  let results = apiGet(uri.SETOTDR.type, uri.SETOTDR.uri,null,data);
  console.log("setOtdr results=",results);
  return results;
}

// Post /data/optical-otdr/otdr-port={port-name}/config/
export function setOtdrPortCfg(data,name) {
  console.log("setOtdrPortCfg name=",name,"data=",JSON.stringify(data));       
  // const url = uri.SETOCMCFG.uri.replace("{}","{"+name+"}")

  let payload =  JSON.parse("{ \"config\":" + JSON.stringify(data) +  "}") 
  const url = uri.SETOTDRPORTCFG.uri.replace("{port-name}",name)
  console.log("setOtdrPortCfg url = ",url);
  let results = apiGet(uri.SETOTDRPORTCFG.type, url,null,payload);

  console.log("setOtdrPortCfg results=",results);
  return results;
}

export function setOtdrCfg(data) {
  let payload =  JSON.parse("{ \"config\":" + JSON.stringify(data) +  "}") 
  let results = apiGet(uri.SETOTDRCFG.type, uri.SETOTDRCFG.uri,null,payload);
  console.log("setOtdrCfg results=",results);
  return results;
}





