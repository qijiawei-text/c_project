import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function getSystemState() {
    let results = apiGet(uri.GETSYSTEMSTATE.type, uri.GETSYSTEMSTATE.uri);
    
    console.log("getSystemState results=",results);
    return results;
}


