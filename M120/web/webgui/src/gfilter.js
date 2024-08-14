import router from './router'
import store from './store'
import NProgress from 'nprogress' // Progress 进度条
import 'nprogress/nprogress.css'// Progress 进度条样式
import { Message } from 'element-ui'
import { getToken } from '@/utils/auth' 
import { setTitle } from '@/utils/util'; 
import website from '@/const/website'
import { asyncRouterMap } from "@/router";
import { findRouteFromWhole,findRouteFromCurTab } from "@/utils/path";
// permission filter and judge function
function hasPermission(roles, permissionRoles) {
  if (roles.indexOf('admin') >= 0) return true // admin permission passed directly
  if (!permissionRoles) return true
  return roles.some(role => permissionRoles.indexOf(role) >= 0)
}
const whiteList = website.whiteList  
router.beforeEach((to, from, next) => {
  NProgress.start()
  if (getToken()) {
    /* has token*/
    if (to.path === '/') {
      next({ path: '/homepage' })
      NProgress.done() 
    } else {
      store.dispatch('getNowRoutes', to);
      console.log("to", to)
      // 没有动态改变权限的需求可直接next() 删除下方权限判断   
      //todo 
      let tmp = to.path.split('/')
      if (findRouteFromWhole(asyncRouterMap, tmp[tmp.length-1]) != -1) {//if (routeExisted(tmp[tmp.length-1]) != -1) {
        next()
      } 
      else{
        next('/') // 判断此跳转路由的来源路由是否存在，存在的情况跳转到来源路由，否则跳转到404页面或主页面
        NProgress.done()
      }  

    }
  } else {
    if (whiteList.indexOf(to.path) !== -1) {
      next()
    } else {
      next('/login')
      console.log("login finished......")
      NProgress.done()
    }
  }
})

router.afterEach(() => {
  NProgress.done() // 结束Progress
  setTimeout(() => {
    const browserHeaderTitle = store.getters.browserHeaderTitle
    setTitle(browserHeaderTitle)
  }, 0)
})


