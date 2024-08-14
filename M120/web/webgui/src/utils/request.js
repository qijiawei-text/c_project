import axios from 'axios'
import { Message, MessageBox } from 'element-ui'
import store from '../store'
import { getToken } from '@/utils/auth'
import codeMessage from "@/const/codeMessage";
// 创建axios实例
const service = axios.create({
  baseURL: process.env.BASE_API,
  timeout: 15000 // 请求超时时间
})

// request拦截器
service.interceptors.request.use(config => {
  if (store.getters.token) {
    config.headers['Token'] = getToken() 
  }
  return config
}, error => {
  // Do something with request error
  //console.log(error) // for debug
  Promise.reject(error)
})

// respone拦截器
service.interceptors.response.use(
  response => {

    console.log("get response.........", response)
    console.log(response.headers)
    console.log(response.data)
    // const res = response.data
    // console.log(res.code)
    // if (res.code !== 0) {
    //   Message({
    //     message: res.message,
    //     type: 'error',
    //     duration: 5 * 1000
    //   })
    //   console.log("data parse error")
    //   return Promise.reject('error')
    // } else {
    //   console.log("return response data......")
    //   return response.data
    // }

    console.log("return response ......")
    return response
  },
  error => {
    // console.log('err' + error)// for debug
    Message({
      message: error.message,
      // message: codeMessage[error.response.status],
      type: 'error',
      duration: 5 * 1000
    })
    return Promise.reject(error)
  }
)

export default service
