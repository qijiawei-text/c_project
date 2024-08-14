//通过every+return false、some或者return true来实现终止循环提升性能
import { asyncRouterMap } from "@/router";

export function findRouteFromTab(arr, route) {
    let ret = -1;
    console.log(" router=", route);
    console.log(" arr=", arr);
    arr.some((item, index) => {
        console.log("###item.name=", item.name, "index=", index);
        if (item.name == route) {
            console.log("finded,index=", index);
            ret = true;
            return true;
        }
        if (item.children) {
            console.log("has children");
            //this.findRouteFromTab(item.children,route)
            item.children.some((value) => {
                if (value.name == route) {
                    console.log("finded1");
                    ret = 1;
                    return true;
                }
            });
        }
    });
    return ret;
}

export function findRouteFromWhole(routers, route, curtab) {
    let ret = -1;
    console.log("routerslength=", routers.length);
    for (let i = 0; i < routers.length; i++) {
        // console.log("highlevel name=",routers[i].name, "route=",route);
        // if (routers[i].name == route) {
        //     console.log("finded toplevel,i=", i);
        //     ret = i;
        //     break;
        // }
        let arr = new Array(routers[i]);
        console.log("arr=", arr[0].children);
        if (arr[0].children) {
            ret = findRouteFromTab(arr[0].children, route);
        }
        console.log("ret = ", ret);
        if (ret == true) {
            ret = i;
            break;
        }
    }
    console.log("findRouteFromWholeret = ", ret);
    return ret;
}

export function findRouteFromCurTab(route, curtab) {
    let ret = -1;
    console.log(" router=", route);
    for (let i in asyncRouterMap[curtab]) {
        console.log("i=", i, asyncRouterMap[i]);
        //console.log(JSON.stringify(asyncRouterMap[i]));
        if (JSON.stringify(asyncRouterMap[i]).search(route) !== -1) {
            ret = i;
            break;
        }
    }
    if (ret !== -1) {
        console.log("ret=", ret, "curtab=", curtab);
        if (ret == curtab) {
            console.log("ret === curtab");
            ret = 0;
        }
    }
    console.log("ret=", ret);
    return ret;
}

export function routeExisted(route) {
    let ret = -1;
    console.log(" router=", route);
    for (let i in asyncRouterMap) {
        console.log("i=", i, asyncRouterMap[i]);
        //console.log(JSON.stringify(asyncRouterMap[i]));
        if (JSON.stringify(asyncRouterMap[i]).search(route) !== -1) {
            console.log("break", route);
            ret = i;
            break;
        }
    }
    return ret;
}