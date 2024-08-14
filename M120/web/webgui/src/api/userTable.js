import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

//GET /data/system/authentication/
export function getUserListPage() {
    let results = apiGet(uri.GETUSERLIST.type, uri.GETUSERLIST.uri);
    
    console.log("getUserListPage results=",results);
    return results;
}

//POST /data/system/authentication/user/ or PUT /data/system/authentication/user={name}/
export function addUser(data) {
    const url = uri.USERADD.uri + "user=" + data.name
    let results = apiGet(uri.USERADD.type, url,null,data);

    console.log("addUser results=",results);
    return results;
}

//DELETE /data/system/authentication/user={name}
export function removeUser(data) {
  console.log("removeUser data=",data);
  //let results = apiGet(uri.USERDEL.type, uri.USERDEL.uri,null,data);
  // const url = uri.USERDEL.uri + "user={" + data.name + "}"
  const url = uri.USERDEL.uri + "user=" + data.name + "/"
  //const url = uri.USERDEL.uri + "user=" + data.name
  //const url = uri.USERDEL.uri
  let results = apiGet(uri.USERDEL.type, url,null,data);

  console.log("removeUser results=",results);
  return results;
}

export function editUser(data,oldname) {
  console.log("editUser data=",data,"oldname=",oldname);
  //let results = apiGet(uri.USEREDIT.type, uri.USEREDIT.uri,null,data);
  //const url = uri.USEREDIT.uri + "user={" + data.name + "}"
  const url = uri.USEREDIT.uri + "user=" + oldname
  let results = apiGet(uri.USEREDIT.type, url,null,data);

  console.log("editUser results=",results);
  return results;
}


export function batchRemoveUser(param) {
  console.log("param=",param);

}

