import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function getOpticalModules() {
  let results = apiGet(uri.GETOPTICALMODULE.type, uri.GETOPTICALMODULE.uri);
    
  console.log("getOpticalModules results=",results);
  return results
}

 export function getOpticalModulesThenStore(_this){
    let date = Date();
    console.log("will get OpticalModules info" + date);
    getOpticalModules().then(
      (value) => {
        // success
        console.log("save opticalmodules to store,_this.nodes",_this.nodes); 
        console.log("value=",value.data);
        _this.$store.dispatch("setOpticalModules", value.data);
        console.log("store=", _this.$store);
      },
      (value) => {
        // failure
        console.log("failure");
      }
    );
}