import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";
import { generateSlotsRouterByOpenRoadmDeviceAndOpticalModules } from '@/router'

export async function getSlotsByOpenRoadmDeviceAndOpticalModules(_this) {
  try {
    console.log("getSlotsByOpenRoadmDeviceAndOpticalModules start...");
    const device = apiGet(uri.GETDEVICE.type, uri.GETDEVICE.uri);
    const opticalModules = apiGet(uri.GETOPTICALMODULE.type, uri.GETOPTICALMODULE.uri);

    const [_device, _opticalModules] = await Promise.all([device, opticalModules])
    console.log("getSlotsByOpenRoadmDeviceAndOpticalModules finish...");
    console.log(_device)
    console.log(_opticalModules)
    // _this.$store.dispatch("setNodes", _device.data);
    // _this.$store.dispatch("setOpticalModules", _opticalModules.data);
    return generateSlotsRouterByOpenRoadmDeviceAndOpticalModules(_device.data, _opticalModules.data);
  } catch (error) {
    console.log("getSlotsInfo err=", error)
    return new Promise((resolve, reject) => {
      reject(error)
    })
  }
}

export function getSlotsByOpenRoadmDeviceAndOpticalModulesThenStore(_this) {
  return new Promise(resolve => {
    let date = Date();
    console.log("will get slots info from device and opticalmodules" + date);
    getSlotsByOpenRoadmDeviceAndOpticalModules(_this).then(
      (value) => {
        _this.$store.dispatch("setNodes",value[0]);
        _this.$store.dispatch("setOpticalModules", value[1]);
      },
      (value) => {
        // failure
        console.log("failure");
      }
    );
    resolve();
  })
}