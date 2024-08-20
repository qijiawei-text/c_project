import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function setRestart() {
    let results = apiGet(uri.SETRESTART.type, uri.SETRESTART.uri);
    
    console.log("setRestart results=",results);
    return results;
}

export function setShutdown() {
    let results = apiGet(uri.SETSHUTDOWN.type, uri.SETSHUTDOWN.uri);   
    console.log("setShutdown results=",results);
    return results;
}



