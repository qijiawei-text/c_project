import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function getTacacs() {
    let results = apiGet(uri.GETTACACS.type, uri.GETTACACS.uri);
    
    console.log("getTacacs results=",results);
    return results;
}

export function setTacacs(data) {
    console.log("param=",data);
    let results = apiGet(uri.SETTACACS.type, uri.SETTACACS.uri,null,data);
    
    console.log("setTacacs results=",results);
    return results;
}



