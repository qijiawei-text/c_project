import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

//GET /data/syslog
export function getSyslogListPage() {
    let results = apiGet(uri.GETSYSLOGLIST.type, uri.GETSYSLOGLIST.uri);
    
    console.log("getSyslogListPage results=",results);
    return results;
}

//  PUT /data/syslog/log-actions/remote/destination={name}
export function addDestination(data) {
    console.log("addDestination data=",data);
    const url = uri.SETSYSLOG.uri + "{" + data.name + "}"
    let results = apiGet(uri.SETSYSLOG.type, url,null,data);

    console.log("addDestination results=",results);
    return results;
}

//DELETE /data/syslog/log-actions/remote/destination={name}
export function removeDestination(data) {
  console.log("removeDestination data=",data);
  const url = uri.DELSYSLOG.uri + "{" + data.name + "}"
  let results = apiGet(uri.DELSYSLOG.type, url,null,data);

  console.log("removeDestination results=",results);
  return results;
}

export function editDestination(data,oldname) {
  console.log("editDestination data=",data,"oldname=",oldname);
  const url = uri.SETSYSLOG.uri + "{" + oldname + "}"
  let results = apiGet(uri.SETSYSLOG.type, url,null,data);

  console.log("editDestination results=",results);
  return results;
}



