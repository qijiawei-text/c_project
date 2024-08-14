import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function getSystemConfig() {
    let results = apiGet(uri.GETSYSTEMCONFIG.type, uri.GETSYSTEMCONFIG.uri);
    
    console.log("getSystemConfig results=",results);
    return results;
}

export function setSystemConfig(data) {
    console.log("system=",data);
    let results = apiGet(uri.SETSYSTEMCONFIG.type, uri.SETSYSTEMCONFIG.uri,null,data);
    
    console.log("setSystemConfig results=",results);
    return results;
}



