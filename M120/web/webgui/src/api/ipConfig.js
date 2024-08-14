import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function getIPConfig() {
    let results = apiGet(uri.GETIPCONFIG.type, uri.GETIPCONFIG.uri);
    
    console.log("getIPConfig results=",results);
    return results;
}

export function setIPConfig(data) {
    console.log("system=",data);
    let results = apiGet(uri.SETIPCONFIG.type, uri.SETIPCONFIG.uri,null,data);
    
    console.log("setIPConfig results=",results);
    return results;
}



