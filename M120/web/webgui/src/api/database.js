import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function setDbBackup(data) {
    let results = apiGet(uri.SETDBBACKUP.type, uri.SETDBBACKUP.uri,null,data);  
    console.log("setDbBackup results=",results);
    return results;
}

export function setDbStore(data) {
    let results = apiGet(uri.SETDBRESTORE.type, uri.SETDBRESTORE.uri,null,data);   
    console.log("setDbStore results=",results);
    return results;
}

export function setDbActivate(data) {
    let results = apiGet(uri.SETDBACTIVATE.type, uri.SETDBACTIVATE.uri,null,data);   
    console.log("setDbActivate results=",results);
    return results;
}

export function setDbInit(data) {
    let results = apiGet(uri.SETDBINIT.type, uri.SETDBINIT.uri,null,data);   
    console.log("setDbInit results=",results);
    return results;
}

export function setCancelRollbackTimer(data) {
    let results = apiGet(uri.SETCANCELROLLBACKTIMER.type, uri.SETCANCELROLLBACKTIMER.uri,null,data);   
    console.log("setCancelRollbackTimer results=",results);
    return results;
}





