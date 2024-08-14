import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function getOscList() {
    let results = apiGet(uri.GETOSCLIST.type, uri.GETOSCLIST.uri);
    
    console.log("getOsc results=",results);
    return results;
}

export function getOscListThenStore(_this){
let date = Date();
console.log("will get osc info" + date);
getOscList().then(
  (value) => {
    // success
    console.log("successed,save osc to storethis",_this);
    console.log("save osc to store,_this.nodes",_this.nodes);   
    console.log("value=",value.data);
    _this.$store.dispatch("setOscs", value.data);
    console.log("store=", _this.$store);
  },
  (value) => {
    // failure
    console.log("failure");
  }
);
}

export function getOsc(data) {
  console.log("getOsc data=",data);
  // const url = uri.GETOCMCFG.uri.replace("{}","{"+data+"}")
  const url = uri.GETOSC.uri.replace("{name}",data)
  console.log("getOsc url = ",url);
  let results = apiGet(uri.GETOSC.type, url);
  
  console.log("getOsc results=",results);
  return results;
}

export function getOscCfg(data) {
  console.log("getOscCfg data=",data);
  // const url = uri.GETOCMCFG.uri.replace("{}","{"+data+"}")
  const url = uri.GETOSCCFG.uri.replace("{name}",data)
  console.log("getOscCfg url = ",url);
  let results = apiGet(uri.GETOSCCFG.type, url);
  
  console.log("getOscCfg results=",results);
  return results;
}

export function setOsc(data) {
  let payload =  JSON.parse("{ \"osc\":" + JSON.stringify(data) +  "}") 
  let results = apiGet(uri.SETOSC.type, uri.SETOSC.uri,null,payload);
  console.log("setOsc results=",results);
  return results;
}


export function setOscCfg(data) {
  let payload =  JSON.parse("{ \"config\":" + JSON.stringify(data) +  "}") 
  let results = apiGet(uri.SETOSCCFG.type, uri.SETOSCCFG.uri,null,payload);
  console.log("setOscCfg results=",results);
  return results;
}





