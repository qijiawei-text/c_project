import Vue from 'vue'
import App from './App.vue'
import router from './router'
import store from './store'
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
//import 'element-ui/lib/theme-default/index.css'
import locale from 'element-ui/lib/locale/lang/en' 
import i18n from './lang'

import '@/styles/index.scss' // global css
import '@/gfilter'
Vue.config.productionTip = false
Vue.use(ElementUI, { locale })
Vue.use(ElementUI, {
  size: 'medium', // set element-ui default size
  i18n: (key, value) => i18n.t(key, value)
})

new Vue({
  router,
  store,
  i18n,
  render: h => h(App)
}).$mount('#app')
