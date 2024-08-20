import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function getRadius() {
    let results = apiGet(uri.GETRADIUS.type, uri.GETRADIUS.uri);
    
    console.log("getRadius results=",results);
    return results;
}

export function setRadius(data) {
    console.log("setRadius param=",data);
    let results = apiGet(uri.SETRADIUS.type, uri.SETRADIUS.uri,null,data);
    
    console.log("stRadius results=",results);
    return results;
}

export function addRadiusServer(data) {
    console.log("addRadiusServer param=",data);
    const url = uri.SETRADIUSSERVER.uri + "user=" + data.name
    let results = apiGet(uri.SETRADIUSSERVER.type, url,null,data);
    
    console.log("addRadiusServer results=",results);
    return results;
}

export function removeRadiusServer(data) {
  console.log("removeRadiusServer data=",data);
  //let results = apiGet(uri.USERDEL.type, uri.USERDEL.uri,null,data);
  const url = uri.DELRADIUSSERVER.uri + "user={" + data.name + "}"
  //const url = uri.USERDEL.uri + "user=" + data.name
  //const url = uri.USERDEL.uri
  let results = apiGet(uri.DELRADIUSSERVER.type, url,null,data);

  console.log("removeRadiusServer results=",results);
  return results;
}

export function editRadiusServer(data,oldname) {
  console.log("editeditRadiusServerUser data=",data,"oldname=",oldname);
  //let results = apiGet(uri.USEREDIT.type, uri.USEREDIT.uri,null,data);
  //const url = uri.USEREDIT.uri + "user={" + data.name + "}"
  const url = uri.SETRADIUSSERVER.uri + "user=" + oldname
  let results = apiGet(uri.SETRADIUSSERVER.type, url,null,data);

  console.log("editRadiusServer results=",results);
  return results;
}


