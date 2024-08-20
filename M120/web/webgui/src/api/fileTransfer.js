import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function setDeleteFile(data) {
    let results = apiGet(uri.SETDELETEFILE.type, uri.SETDELETEFILE.uri,null,data);  
    console.log("setDeletefile results=",results);
    return results;
}

export function setShowFile(data) {
    let results = apiGet(uri.SETSHOWFILE.type, uri.SETSHOWFILE.uri,null,data);   
    console.log("setShowFile results=",results);
    return results;
}

export function setTransfer(data) {
    let results = apiGet(uri.SETTRANSFER.type, uri.SETTRANSFER.uri,null,data);   
    console.log("setTransfer results=",results);
    return results;
}







