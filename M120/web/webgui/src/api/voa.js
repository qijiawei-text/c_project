import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function getVoaList() {
  let results = apiGet(uri.GETVOALIST.type, uri.GETVOALIST.uri);

  console.log("getVoaList results=", results);
  return results;
}

export function getVoaListThenStore(_this) {
  let date = Date();
  console.log("will get voa info from device" + date);
  getVoaList().then(
    (value) => {
      // success
      console.log("successed,save voa to storethis", _this);
      console.log("save voa to store,_this.nodes", _this.nodes);
      console.log("value=", value.data);
      _this.$store.dispatch("setVoas", value.data);
      console.log("store=", _this.$store);
    },
    (value) => {
      // failure
      console.log("failure");
    }
  );
}

//GET /data/optical-attenuator/attenuators/attenuator={name}/
export function getVoa(data) {
  console.log("getVoa data=", data);
  // const url = uri.GETOCMCFG.uri.replace("{}","{"+data+"}")
  const url = uri.GETVOA.uri.replace("{name}", data)
  console.log("getVoa url = ", url);
  let results = apiGet(uri.GETVOA.type, url);

  console.log("getVoa results=", results);
  return results;
}

export function setVoa(data, name) {
  console.log("setVoaConfig name=", name, "data=", JSON.stringify(data));
  // const url = uri.SETOCMCFG.uri.replace("{}","{"+name+"}")
  const url = uri.SETVOA.uri.replace("{name}", name)
  console.log("setVoa url = ", url);
  let results = apiGet(uri.SETOCMCFG.type, url, null, data);

  console.log("setVoa results=", results);
  return results;
}

export function setVoaConfig(data, name) {
  console.log("setVoaConfig name=", name, "data=", JSON.stringify(data));
  // const url = uri.SETOCMCFG.uri.replace("{}","{"+name+"}")

  let payload = JSON.parse("{ \"config\":" + JSON.stringify(data) + "}")
  const url = uri.SETVOACFG.uri.replace("{name}", name)
  console.log("setVoaConfig url = ", url);
  let results = apiGet(uri.SETVOACFG.type, url, null, payload);

  console.log("setVoaConfig results=", results);
  return results;
}




