import Vue from 'vue'
import Vuex from 'vuex'
import app from './modules/app'
import user from './modules/user'
import common from './modules/common'
import getters from './getters'
import fullScreen from './modules/fullScreen'
import permission from './modules/permission'
import device from '@/store/modules/device'
import voa from '@/store/modules/voa'
import opticalModule from '@/store/modules/opticalModule'
import osc from '@/store/modules/osc'
import tagsView from './modules/tagsView'
Vue.use(Vuex)

export default new Vuex.Store({
  state: {
  },
  mutations: {
  },
  actions: {
  },
  modules: {
    app,
    user,
    common,
    fullScreen,
    permission,
    device,
    voa,
    opticalModule,
    osc,
    tagsView,
  },
  getters
})
