import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";
import {generateSlotsRouterByOpenRoadmDevice} from '@/router'

export async function getDevice() {
  try {
    let res = await apiGet(uri.GETDEVICE.type, uri.GETDEVICE.uri)
    // this.system = JSON.parse(JSON.stringify(res.system));
    // this.system = res.data.system    
    console.log("willgenerateSlotsRouter!!!!!!!!!!!!!!!!!!",res)
    return generateSlotsRouterByOpenRoadmDevice(res);
  } catch (error) {
    console.log("getSlotsInfo err=", error)
    return new Promise((resolve,reject) => {
      reject(error)
    })
  }
}

 export function getSlotsByOpenRoadmDeviceThenStore(_this){
    let date = Date();
    console.log("will get slots info from device !!!!!!!!!!!!!!!!!!" + date);
    getDevice().then(
      (value) => {
        // success
        console.log("success,this",_this);
        console.log("save nodes to store,_this.nodes........................",_this.nodes);
        // _this.$store.dispatch("setNodes", _this.nodes);
        //_this.$store.dispatch("setNodes", asyncRouterMap);       
        console.log("value=",value.data);
        _this.$store.dispatch("setNodes", value.data);
        console.log("store=", _this.$store);
      },
      (value) => {
        // failure
        console.log("failure");
      }
    );
}