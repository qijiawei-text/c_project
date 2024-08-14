import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function setDefault(data) {
    let results = apiGet(uri.SETDEFAULT.type, uri.SETDEFAULT.uri,null,data);  
    console.log("setDefault results=",results);
    return results;
}

export function setSetConf() {
    let results = apiGet(uri.SETSAVECONF.type, uri.SETSAVECONF.uri);   
    console.log("setSetConf results=",results);
    return results;
}



