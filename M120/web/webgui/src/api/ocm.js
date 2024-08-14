import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

//GET /data/channel-monitors/
export function getOcmList() {
    let results = apiGet(uri.GETOCMLIST.type, uri.GETOCMLIST.uri);
    
    console.log("getOCMList results=",results);
    return results;
}

//GET /data/channel-monitors/channel-monitor={}/config/
export function getOcmConfig(data) {
  console.log("getOcmConfig data=",data);
  // const url = uri.GETOCMCFG.uri.replace("{}","{"+data+"}")
  const url = uri.GETOCMCFG.uri.replace("{name}",data)
  console.log("getOcmConfig url = ",url);
  let results = apiGet(uri.GETOCMCFG.type, url);
  
  console.log("getOcmConfig results=",results);
  return results;
}

export function setOcmConfig(data,name) {
  console.log("setOcmConfig name=",name,"data=",JSON.stringify(data));       
  // const url = uri.SETOCMCFG.uri.replace("{}","{"+name+"}")
  const url = uri.SETOCMCFG.uri.replace("{name}",name)
  console.log("setOcmConfig url = ",url);
  let results = apiGet(uri.SETOCMCFG.type, url,null,data);

  console.log("setOcmConfig results=",results);
  return results;
}


export function getOcmChannels(data) {
  console.log("getOcmChannels data=",data);
  // const url = uri.getOcmChannels.uri.replace("{}","{"+data+"}")
  const url = uri.GETOCMCHANNELS.uri.replace("{name}",data)
  console.log("getOcmConfig url = ",url);
  let results = apiGet(uri.GETOCMCHANNELS.type, url);
  
  console.log("getOcmChannels results=",results);
  return results;
}

export function setOAList(data) {  
    let results = apiGet(uri.SETOALIST.type, uri.SETOALIST.uri,null,data);

    console.log("setOAList results=",results);
    return results;
}

export function getOA(name) {
  // const url = uri.GETOA.uri + "{" + name + "}"  
  const url = uri.GETOA.uri +  name + "/"  
  let results = apiGet(uri.GETOA.type, url); 
  // let results = apiGet(uri.GETOA.type, "/data/optical-amplifier/amplifiers/");   
  console.log("getOA results=",results);
  return results;
}

export function setOA(data,name) {
  const url = uri.SETOA.uri + "{" + name + "}"    
  let results = apiGet(uri.SETOA.type, url,null,data);

  console.log("setOA results=",results);
  return results;
}


