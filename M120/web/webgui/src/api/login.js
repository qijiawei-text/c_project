import request from '@/utils/request'
// {
//   "username":    "admin",
//   "password":     "m4X!izP#"
// }
export function login(username, password) {
  return request({
    //url: 'http://localhost:8081/user/login', 
    url: '/omt/user/login',
    method: 'post',
    data: {
      "username": username,
      "password": password,
    }
  })
}

export function getInfo(token) {
  return request({
    url: '/user/info',
    method: 'get',
    params: { token }
  })
}

export function logout() {
  return request({
    url: '/omt/user/logout',
    method: 'post'
  })
}
