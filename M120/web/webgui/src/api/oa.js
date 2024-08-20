import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function getOAList() {
    let results = apiGet(uri.GETOALIST.type, uri.GETOALIST.uri);
    
    console.log("getOAListPage results=",results);
    return results;
}

export function setOAList(data) {
    //const url = uri.USERADD.uri + "user={" + data.name + "}"    
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


