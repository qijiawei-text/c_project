import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function getActiveAlarm() {
    let results = apiGet(uri.GETACTIVEALARM.type, uri.GETACTIVEALARM.uri);
    
    console.log("getActiveAlarm results=",results);
    return results;
}


