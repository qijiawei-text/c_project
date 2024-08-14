import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";

export function getClockNtpInfo() {
    let clock = apiGet(uri.GETCLOCKINFO.type, uri.GETCLOCKINFO.uri);
    let ntp = apiGet(uri.GETNTPINFO.type, uri.GETNTPINFO.uri);

    let results =   Promise.all([clock, ntp])
    // .then(function (results) {
    //  let [data1, data2] = results 
    //  console.log("data1=",data1) 
    //  console.log("data2=",data2) 
    // }, function (e) {
    // console.log('getClockNtpInfo failed')  
    // })

    console.log("getClockNtpInfo results=",results);
    return results;
}

export function setClockNtpInfo(clock,ntp,datetime) {

    console.log("datetime=",datetime);
    let _clock = apiGet(uri.SETCLOCKINFO.type, uri.SETCLOCKINFO.uri,null,clock);
    let _ntp = apiGet(uri.SETCLOCKINFO.type, uri.SETCLOCKINFO.uri,null,ntp);
    let _datetime = apiGet(uri.SETCURDATETIME.type, uri.SETCURDATETIME.uri,null,datetime);
    
    let results =   Promise.all([_clock, _ntp, _datetime])
    // .then(function (results) {
    //  let [data1, data2] = results 
    //  console.log("data1=",data1) 
    //  console.log("data2=",data2) 
    // }, function (e) {
    // console.log('setClockNtpInfo failed')  
    // })

    console.log("setClockNtpInfo results=",results);
    return results;
}

// export function setCurrentDateTime(data) {

//     console.log("system=",data);
//     let results = apiGet(uri.SETCURDATETIME.type, uri.SETCURDATETIME.uri,null,data);
    
//     console.log("setCurrentDateTime results=",results);
//     return results;
// }
