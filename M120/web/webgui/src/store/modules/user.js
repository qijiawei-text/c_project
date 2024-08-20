import { login, logout, getInfo } from '@/api/login'
import { getToken, setToken, removeToken } from '@/utils/auth'

const user = {
  state: {
    token: getToken(),
    name: '',
    avatar: '',
    roles: []
  },

  mutations: {
    SET_TOKEN: (state, token) => {
      state.token = token
    },
    SET_NAME: (state, name) => {
      state.name = name
    },
    SET_AVATAR: (state, avatar) => {
      state.avatar = avatar
    },
    SET_ROLES: (state, roles) => {
      state.roles = roles
    }

  },

  actions: {
    // 登录
    Login({ commit }, userInfo) {
      const username = userInfo.username.trim()
      return new Promise((resolve, reject) => {
        login(username, userInfo.password).then(response => {
          console.log(response.headers)
          const token = response.headers['token']
          console.log("will save token... ", token)
          if (null == token) {
            console.log("token is null!")
            reject("token is null!")
          }
          setToken(token)
          commit('SET_TOKEN', token)
          resolve()
        }).catch(error => {
          reject(error)
        })
      })
    },



    SetInfoFromOauth({ commit }, data) {
      return new Promise((resolve, reject) => {
        commit('SET_TOKEN', "admin")
        setToken("admin")
        console.log("SetInfoFromOauth,data.login=" + data.login)
        console.log("SetInfoFromOauth,data.avatar_url=" + data.avatar_url)
        commit('SET_NAME', data.login)
        commit('SET_AVATAR', data.avatar_url)
        commit('SET_ROLES', "admin")
      })
    },
    // RemoveInfoFromOauth({ commit }) {
    //   return new Promise((resolve, reject) => {
    //     commit('SET_TOKEN', "")
    //     removeToken()
    //     commit('SET_NAME', "")
    //     commit('SET_AVATAR', "")
    //     console.log("logout for oauth2 account.....")
    //     this.$token.deleteToken();
    //   })
    // },



    // 获取用户信息
    GetInfo({ commit, state }) {
      return new Promise((resolve, reject) => {
        getInfo(state.token).then(response => {
          const data = response
          if (data.roles && data.roles.length > 0) { // 验证返回的roles是否是一个非空数组
            commit('SET_ROLES', data.roles)
          } else {
            reject('getInfo: roles must be a non-null array !')
          }
          console.log("9999999999999999999999999999999999data.name" + data.name)
          commit('SET_NAME', data.name)
          commit('SET_AVATAR', data.avatar)
          resolve(response)
        }).catch(error => {
          reject(error)
        })
      })
    },

    // 登出
    LogOut({ commit, state }) {
      return new Promise((resolve, reject) => {
        logout(state.token).then(() => {
          commit('SET_TOKEN', '')
          commit('SET_ROLES', [])
          removeToken()

          sessionStorage.setItem("username", null);
          sessionStorage.setItem("access_token", null);
          sessionStorage.setItem("enabled", null);
          sessionStorage.setItem("expires_in", null);
          sessionStorage.setItem("refresh_token", null);
          sessionStorage.setItem("token_type", null);

          resolve()
        }).catch(error => {
          reject(error)
        })
      })
    },

    // 前端 登出
    FedLogOut({ commit }) {
      return new Promise(resolve => {
        commit('SET_TOKEN', '')
        removeToken()
        resolve()
      })
    },
    // 动态修改权限
    ChangeRoles({ commit }, role) {
      return new Promise(resolve => {
        commit('SET_TOKEN', role)
        setToken(role)
        getInfo(role).then(response => {
          const data = response
          commit('SET_ROLES', data.roles)
          commit('SET_NAME', data.name)
          commit('SET_AVATAR', data.avatar)
          resolve()
        })
      })
    }
  }
}

export default user
