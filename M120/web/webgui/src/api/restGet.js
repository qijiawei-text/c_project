import request from '@/utils/request'
import { generateSlotsRouter } from '@/router'
import uri from "@/const/uri";
import { asyncRouterMap } from "@/router";

export function getView(type, url, params, payload) {
  return request({
    headers: {
      'Content-Type': 'application/json'  //multipart/form-data  application/json application/x-www-form-urlencoded
    },
    url: url,
    method: type,
    params: params,
    data: payload
  })
}

// restapi get
export function apiGet(type, url, params, payload) {
  return new Promise((resolve, reject) => {
    getView(type, url, params, payload).then(response => {
      console.log("get response..............", response)
      resolve(response)
    }).catch(error => {
      console.log("error", error)
      reject(error)
    })
  })
}


export function apiGetAbandonment() {

  getView().then(response => {
    console.log("get response..............")
    const data = response
    if (data.roles && data.roles.length > 0) { // 验证返回的roles是否是一个非空数组        
    } else {
      console.log('api_get: roles must be a non-null array !')
      //reject('getInfo: roles must be a non-null array !')
    }
    console.log("data.name")
    //resolve(response)
  }).catch(error => {
    //reject(error)
    console.log('api_get: error !')
  })

}
var tmpslotcnt = 1;  //临时使用
export async function getSlotsInfo() {
  try {
    let res = await apiGet(uri.GETSLOTSINFO.type, uri.GETSLOTSINFO.uri + (tmpslotcnt++ % 2))
    // this.system = JSON.parse(JSON.stringify(res.system));
    //this.system = res.data.system    
    console.log("willgenerateSlotsRouter!!!!!!!!!!!!!!!!!!", res)
    return generateSlotsRouter(res);
  } catch (error) {
    console.log("getSlotsInfo err=", error)
    return new Promise((resolve, reject) => {
      reject(error)
    })
  }
}

export function getSlotsThenStore(_this) {
  let date = Date();
  console.log("will get slots info from device" + date);
  getSlotsInfo().then(
    (value) => {
      // success
      console.log("success,this", _this);
      console.log("save nodes to store,_this.nodes", _this.nodes);
      _this.$store.dispatch("setNodes", _this.nodes);
      console.log("store=", _this.$store);
    },
    (value) => {
      // failure
      console.log("failure");
    }
  );
}

