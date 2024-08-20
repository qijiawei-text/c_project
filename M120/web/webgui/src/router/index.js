import Vue from 'vue'
import VueRouter from 'vue-router'
import Home from '../views/Home.vue'
//import Login from '../views/login/Index.vue'
import Main from '@/views/Main'

import Homepage from '../views/app/homepage/Homepage.vue'

import SlotsConfiguration from '../views/app/board/SlotsConfiguration.vue'
import SlotItem from '../views/app/board/SlotItem.vue'
import CardState from '../views/app/board/CardState.vue'
import CardConfiguration from '../views/app/board/CardConfiguration.vue'
import CardReset from '../views/app/board/CardReset.vue'

import System from '../views/app/netelement/system/System.vue'
import Inventory from '../views/app/netelement/system/SystemInventory.vue'
import SystemConfig from '../views/app/netelement/system/SystemConfig.vue'
import SystemFactoryDefault from '@/views/app/netelement/system/SystemFactoryDefault'
import ClockNtp from "../views/app/netelement/system/SystemClockNTP.vue"
import State from '@/views/app/netelement/system/SystemState'
import SystemRestart from '@/views/app/netelement/system/SystemRestart'
import SystemIPConfig from '@/views/app/netelement/system/SystemIPConfig'

import Monitor from '../views/app/netelement/monitor/Monitor.vue'
import AlarmSetting from '@/views/app/netelement/monitor/AlarmSetting'
import CurrentAlarms from '@/views/app/netelement/monitor/CurrentAlarms'
import HistoryAlarms from '@/views/app/netelement/monitor/HistoryAlarms'
import Logs from '@/views/app/netelement/monitor/Logs'
import PM from '@/views/app/netelement/monitor/PM'

import Security from '../views/app/netelement/security/Security.vue'
import Iptable from '@/views/app/netelement/security/Iptable'
import LocalUser from '@/views/app/netelement/security/LocalUser'
import Session from '@/views/app/netelement/security/Session'
import Snmp from '@/views/app/netelement/security/Snmp'
import SslCertificates from '@/views/app/netelement/security/SslCertificates'
import RadiusService from '@/views/app/netelement/security/RadiusService'
import TacacsService from '@/views/app/netelement/security/TacacsService'
import UserManagement from '@/views/app/netelement/security/UserManagement'

import Service from '@/views/app/netelement/service/Service'
import LocalUpgrade from '@/views/app/netelement/service/LocalUpgrade'
import RemoteUpgrade from '@/views/app/netelement/service/RemoteUpgrade'
import Syslog from '@/views/app/netelement/service/Syslog'
import Database from '@/views/app/netelement/service/Database'

import About from '../views/app/help/About.vue'
import LegalNotice from '../views/app/help/LegalNotice.vue'
import Support from '../views/app/help/Support.vue'
import devenums from '../utils/enum.js'
import DevOa from '@/views/app/board/dev/DevOa'
import DevVoa from '@/views/app/board/dev/DevVoa'
import DevWaveplan from '@/views/app/board/dev/DevWaveplan'
import DevOsc from '@/views/app/board/dev/DevOsc'
import DevOtdr from '@/views/app/board/dev/DevOtdr'
import DevOcm from '@/views/app/board/dev/DevOcm'
import Scc from '@/views/app/board/dev/Scc'
import OmtConfig from '@/views/app/netelement/system/OmtConfig'
//import DevOps from '../views/app/board/dev/DevOps.vue';
//import DevOa from '../views/app/board/dev/DevOa.vue';
Vue.use(VueRouter)

//   const routes: Array<RouteConfig> = [
//   {
//     path: '/',
//     name: 'Home',
//     component: Home
//   },
//   {
//     path: '/about',
//     name: 'About',
//     // route level code-splitting
//     // this generates a separate chunk (about.[hash].js) for this route
//     // which is lazy-loaded when the route is visited.
//     component: () => import(/* webpackChunkName: "about" */ '../views/About.vue')
//   }
// ]

// const router = new VueRouter({
//   routes
// })

export const constantRouterMap = [
  {
    path: '/',
    component: Main,
    // redirect: '/login',
    name: 'main',
    //hidden: true
  },
  //{ path: '/main',    component: "Main" ,    /*redirect: '/login' */ }, 
  { path: '/login', component: () => import('@/views/login/Index.vue'), name: 'login', hidden: true }
]

export const asyncRouterMap = [
  {
    path: '/',
    component: Main,
    name: 'main',
    leaf: false,
    iconCls: 'el-icon-message',//图标样式class
    redirect: 'homepage',
    children: [
      { path: 'homepage', component: Homepage, name: 'homepage', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Homepage' } },
    ]
  },
  {
    path: '/',
    component: Main,
    name: 'BoardManagement',
    leaf: false,
    // iconCls: 'el-icon-message',//图标样式class
    // redirect: { name: 'slotsconfiguration' },
    children: [
      // { path: 'slotsconfiguration', component: SlotsConfiguration, name: 'slotsconfiguration', leaf: true, iconCls: 'el-icon-message',meta: 'slotsconfiguration' },
      // {
      //   path: 'slot1',
      //   component: SlotItem,
      //   name: 'slot1',
      //   leaf:false,
      //   //hidden:true,
      //   //redirect:'test31',
      //   redirect:{name:'cardstate'},
      //   iconCls: 'el-icon-message',//图标样式class
      //   children: [
      //     { path: 'cardstate', component: CardState, name: 'cardstate',leaf:true,iconCls: 'el-icon-message'},        
      //     { path: 'cardconfiguration', component: CardConfiguration, name: 'cardconfiguration',leaf:true,iconCls: 'el-icon-message'},
      //     { path: 'cardreset', component: CardReset, name: 'cardreset',leaf:true,iconCls: 'el-icon-message'}
      //   ]
      // },
      // {
      //   path: 'slot2',
      //   component: SlotItem,
      //   name: 'slot2',
      //   leaf:false,
      //   //hidden:true,
      //   //redirect:'test31',
      //   redirect:{name:'cardstate'},
      //   iconCls: 'el-icon-message',//图标样式class
      //   children: [
      //     { path: 'cardstate', component: CardState, name: 'cardstate',leaf:true,iconCls: 'el-icon-message'},        
      //     { path: 'cardconfiguration', component: CardConfiguration, name: 'cardconfiguration',leaf:true,iconCls: 'el-icon-message'},
      //     { path: 'cardreset', component: CardReset, name: 'cardreset',leaf:true,iconCls: 'el-icon-message'}
      //   ]
      // },

    ]
  },
  {
    path: '/',
    component: Main,
    name: 'NE Management',
    leaf: false,
    iconCls: 'el-icon-message',//图标样式class
    redirect: 'system',
    children: [
      {
        path: 'system', component: System, name: 'system', leaf: false, iconCls: 'el-icon-s-grid', redirect: 'system/cfg', meta: { title: 'System' },
        children: [
          // { path: 'inventory', component: Inventory, name: 'inventory', leaf: true, iconCls: 'el-icon-menu', meta: 'Inventory' },
          { path: 'cfg', component: SystemConfig, name: 'cfg', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'System' } },
          { path: 'reset', component: SystemFactoryDefault, name: 'reset', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Reset' } },
          { path: 'clockntp', component: ClockNtp, name: 'clockntp', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Time/NTP' } },
          { path: 'state', component: State, name: 'state', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'State' } },
          { path: 'restart', component: SystemRestart, name: 'restart', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Restart/Shutdown' } },
          { path: 'ipconfig', component: SystemIPConfig, name: 'ipconfig', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'IP Config' } },

        ]
      },
      {
        path: 'monitor', component: Monitor, name: 'monitor', leaf: false, iconCls: 'el-icon-s-grid', redirect: 'monitor/currentalarms', meta: { title: 'Monitor' },
        children: [
          { path: 'currentalarms', component: CurrentAlarms, name: 'currentalarms', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Currentalarms' } },
          { path: 'historyalarms', component: HistoryAlarms, name: 'historyalarms', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Historyalarms' } },
          { path: 'alarmsetting', component: AlarmSetting, name: 'alarmsetting', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Alarmsetting' } },
          { path: 'pm', component: PM, name: 'pm', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'PM' } },
          { path: 'logs', component: Logs, name: 'logs', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Logs' } },
         ]
      },
      {
        path: 'security', component: Security, name: 'security', leaf: false, iconCls: 'el-icon-s-grid', redirect: 'security/localuser', meta: { title: 'Security' },
        children: [
          { path: 'localuser', component: LocalUser, name: 'localuser', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Localuser' } },
          { path: 'snmp', component: Snmp, name: 'snmp', leaf: true, iconCls: 'el-icon-menu', meta: 'Snmp' },
          // { path: 'session', component: Session, name: 'session', leaf: true, iconCls: 'el-icon-menu', meta: 'Session' },
          { path: 'sslcertificates', component: SslCertificates, name: 'sslcertificates', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Sslcertificates' } },
          { path: 'iptable', component: Iptable, name: 'iptable', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Iptable' } },
          { path: 'radius', component: RadiusService, name: 'radius', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'RadiusService' } },
          { path: 'tacacs', component: TacacsService, name: 'tacacs', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'TacacsService' } },
          { path: 'user', component: UserManagement, name: 'user', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'UserManagement' } },

        ]
      },
      {
        path: 'service', component: Service, name: 'service', leaf: false, iconCls: 'el-icon-s-grid', redirect: 'service/localupgrade', meta: { title: 'Service' },
        children: [
          { path: 'localupgrade', component: LocalUpgrade, name: 'localupgrade', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Localupgrade' } },
          { path: 'remoteupgrade', component: RemoteUpgrade, name: 'remoteupgrade', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Remoteupgrade' } },
          { path: 'syslog', component: Syslog, name: 'syslog', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Syslog' } },
          { path: 'database', component: Database, name: 'database', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Database' } },

        ]
      },
    ]
  },
  {
    path: '/',
    component: Main,
    name: 'About',
    leaf: false,
    iconCls: 'el-icon-menu',//图标样式class
    children: [
      { path: 'about', component: About, name: 'about', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'About' } },
      { path: 'legalnotice', component: LegalNotice, name: 'legalnotice', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Legalnotice' } },
      { path: 'support', component: Support, name: 'support', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Support' } },
      {
        path: '/apitest',
        name: 'apitest',
        hidden: false,
        component: () => import(/* webpackChunkName: "about" */ '../views/app/apitest/ApiTest.vue'),
        //component: Apitest,
        leaf: true,
        iconCls: 'el-icon-menu',
        meta: { title: 'Apitest' }
      },
      {
        path: '/ws',
        name: 'ws',
        hidden: false,
        component: () => import(/* webpackChunkName: "about" */ '../views/websocket/ws.vue'),
        //component: Apitest,
        leaf: true,
        iconCls: 'el-icon-menu',
        meta: { title: 'Websocket' }
      },
      { path: 'oapa', component: DevOa, name: 'oapa', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'OA-PA' } },
      { path: 'oaba', component: DevOa, name: 'oaba', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'OA-BA' } },
      { path: 'padding-voa', component: DevVoa, name: 'padding-voa', leaf: true, iconCls: 'el-icon-menu',meta: { title: 'Padding-voa' } },
      { path: 'osc1-voa', component: DevVoa, name: 'osc1-voa', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Osc1-voa' } },
      { path: 'pa-voa', component: DevVoa, name: 'pa-voa', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Pa-voa'} },
      { path: 'osc2-voa', component: DevVoa, name: 'osc2-voa', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Osc2-voa' } },
      { path: 'omtconfig', component: OmtConfig, name: 'omtconfig', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Omtconfig' } },
      { path: 'waveplan', component: DevWaveplan, name: 'waveplan', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Waveplan' } },
      { path: 'osc1', component: DevOsc, name: 'osc1', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Osc1' } }


    ]
  },

  { path: '/login', component: () => import('@/views/login/Index.vue'), name: 'login', hidden: true },

  {
    path: '/',
    component: () => import('@/views/errorPage/ErrorPage'),
    name: 'Error',
    leaf: false,
    iconCls: 'el-icon-menu',//图标样式class
    children: [
      { path: '/404', component: () => import('@/views/errorPage/404'), name: '404', hidden: true },
      { path: '/401', component: () => import('@/views/errorPage/401'), name: '401', hidden: true },
    ]
  }
]

//version0.1  use private protocol to render navigation menu
export function generateSlotsRouter(data) {
  return new Promise(resolve => {

    console.log("data=", data)
    console.log("data.data=", data.data)
    console.log("data.data.hash.usCount=", data.data.hash.usCount)
    console.log("data.data.hash.astArray=", data.data.hash.astArray)

    let soltcnt = data.data.hash.usCount;
    console.log("soltcnt=", soltcnt)
    let slots = data.data.hash.astArray;
    let boardrouter = asyncRouterMap[1].children;
    console.log('asyncRouterMap[1]=', asyncRouterMap[1])
    console.log('boardrouter0=', boardrouter)
    boardrouter.length = 0;
    let slotsconfiguration1 = { path: 'slotsconfiguration', component: SlotsConfiguration, name: 'slotsconfiguration', leaf: true, iconCls: 'el-icon-message', meta: { title: 'slotsconfiguration' } }
    boardrouter.push(slotsconfiguration1)
    for (let i = 0; i < soltcnt; i++) {

      let slotrouter = {
        path: 'slot1',
        component: SlotItem,
        name: 'slot1',
        leaf: false,
        //hidden:true,
        //redirect:'test31',
        redirect: '',//redirect: { name: 'cardstate' },
        iconCls: 'el-icon-success',//iconCls: 'el-icon-circle-check',//图标样式class
        meta: { title: '', level:2 , status:'' },
        children: [
          // { path: 'cardstate', component: cardstate, name: 'cardstate',leaf:true,iconCls: 'el-icon-message'},        
          // { path: 'cardconfiguration', component: cardconfiguration, name: 'cardconfiguration',leaf:true,iconCls: 'el-icon-message'},
          // { path: 'cardreset', component: cardreset, name: 'cardreset',leaf:true,iconCls: 'el-icon-message'},    

        ]
      }

      //add slot
      console.log("i=", i)
      let slotrouter1 = { ...slotrouter }
      console.log("slotrouter = ", slotrouter1)

      //slotrouter1.children.length = 0;
      slotrouter1.path = "Slot" + i + "" + slots[i].uiCardType;
      slotrouter1.name = "Slot" + i + "" + slots[i].uiCardType;
      slotrouter1.meta.title = "Slot" + i + ":" + slots[i].uiCardType;
      if (slots[i].uiCardType === 'Empty') {
        console.log("Slot is empty!!!!!!!!!!!!!!!!!", i)
        slotrouter1.leaf = true
        slotrouter1.iconCls = 'el-icon-warning-outline'
      }

      //add module 
      let uiDevNum = slots[i].uiDevNum;
      console.log("uiDevNum=", uiDevNum)
      for (let j = 0; j < uiDevNum; j++) {
        console.log("i=", i, "j=", j)
        let modulerouter = {
          path: 'cardstate', component: CardState, name: 'cardstate', leaf: true, iconCls: 'el-icon-message', meta: {title:'',level:3,status:''}
        }
        let modulerouter1 = { ...modulerouter }
        console.log("modulerouter = ", modulerouter)
        let t = Math.floor(Math.random() * 10);
        //modulerouter1.path = slots[i].astDevInfo[j].uiDevType+encodeURIComponent("?slot="+i+"&"+"devidx="+slots[i].astDevInfo[j].uiDevIdx)   // +"/"+slots[i].astDevInfo[j].uiDevIdx
        console.log("modulerouter1.path=", modulerouter1.path)
        //modulerouter1.name = slots[i].astDevInfo[j].uiDevType +"("+ slots[i].astDevInfo[j].uiFuncType+")"

        modulerouter1.path = slots[i].astDevInfo[j].uiDevType + slots[i].astDevInfo[j].uiFuncType
        modulerouter1.name = slots[i].astDevInfo[j].uiDevType + slots[i].astDevInfo[j].uiFuncType
        console.log("modulerouter1.name=", modulerouter1.name)

        modulerouter1.meta.title = slots[i].astDevInfo[j].uiDevType + "(" + slots[i].astDevInfo[j].uiFuncType + ")"
        // modulerouter1.meta.slot = i;
        // modulerouter1.meta.uiDevIdx=slots[i].astDevInfo[j].uiDevIdx;

        console.log("devenums.uiDevTypeEnum=", devenums.uiDevTypeEnum)
        console.log("enumdev type= ", devenums.uiDevTypeEnum.getLabel(slots[i].astDevInfo[j].uiDevType))
        console.log("enumdev value= ", devenums.uiDevTypeEnum.getvaluebyLabel(slots[i].astDevInfo[j].uiDevType))
        modulerouter1.component = devenums.uiDevTypeEnum.getvaluebyLabel(slots[i].astDevInfo[j].uiDevType)
        //modulerouter1.component = devoa
        console.log("modulerouter1=", modulerouter1)
        slotrouter1.children.push(modulerouter1)
        if (j === 0) {
          slotrouter1.redirect = modulerouter1.name;
        }
      }
      console.log("slotrouter1 = ", slotrouter1)
      boardrouter.push(slotrouter1);

    }
    console.log('boardrouter1=', boardrouter)
    console.log('asyncRouterMap[1]=', asyncRouterMap[1])
    console.log('asyncRouterMap=', asyncRouterMap)

    router.options.routes = asyncRouterMap
    router.addRoutes(asyncRouterMap) 

    console.log("addRoutes finish...", router)

    resolve(data);
  })
}

//version0.2  use openroadmdevice to render navigation menu
export function generateSlotsRouterByOpenRoadmDevice(data) {
  return new Promise(resolve => {

    console.log("data=", data)
    console.log("data.data=", data.data)

    let shelves = data.data['org-openroadm-device'].shelves[0];
    console.log("shelves[0]=", shelves)

    let slots = data.data['org-openroadm-device'].shelves[0].slots;
    console.log("slots=", slots)

    let circuitPacks = data.data['org-openroadm-device']['circuit-packs']
    console.log("circuit-packs=", circuitPacks)

    console.log(Array.isArray(slots));
    console.log(Array.isArray(circuitPacks));

    let soltcnt = slots.length;
    console.log("soltcnt=", soltcnt)
    let packcnt = circuitPacks.length
    console.log("packcnt=", packcnt)

    let boardrouter = asyncRouterMap[1].children;
    // console.log('asyncRouterMap[1]=', asyncRouterMap[1])
    // console.log('boardrouter0=', boardrouter)
    boardrouter.length = 0;
    let slotsconfiguration1 = { path: 'slotsconfiguration', component: SlotsConfiguration, name: 'slotsconfiguration', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Slots' } }
    boardrouter.push(slotsconfiguration1)
    for (let i = 0; i < soltcnt; i++) {

      let slotrouter = {
        path: 'slot1',
        component: SlotItem,
        name: 'slot1',
        leaf: false,
        //hidden:true,
        //redirect:'test31',
        redirect: '',//redirect: { name: 'cardstate' },
        iconCls: 'el-icon-success',//iconCls: 'el-icon-circle-check',//图标样式class
        meta: { title: '', level:2 , status:'' },
        children: [
          // { path: 'cardstate', component: cardstate, name: 'cardstate',leaf:true,iconCls: 'el-icon-message'},        
          // { path: 'cardconfiguration', component: cardconfiguration, name: 'cardconfiguration',leaf:true,iconCls: 'el-icon-message'},
          // { path: 'cardreset', component: cardreset, name: 'cardreset',leaf:true,iconCls: 'el-icon-message'},    
        ]
      }
      //add slot
      console.log("i=", i)
      let slotrouter1 = { ...slotrouter }
      console.log("slotrouter = ", slotrouter1)

      //slotrouter1.children.length = 0;
      slotrouter1.path = "Slot" + slots[i].label + "" + slots[i]['slot-name'];
      slotrouter1.name = "Slot" + slots[i].label + "" + slots[i]['slot-name'];
      slotrouter1.meta.title = "Slot" + slots[i].label + ":" + slots[i]['slot-name'];
      if (slots[i]['slot-status'] === 'empty-not-prov') {
        console.log("Slot is empty!!!!!!!!!!!!!!!!!", i)
        slotrouter1.leaf = true
        slotrouter1.iconCls = 'el-icon-warning-outline'
        // slotrouter1.component = DevOa
        // slotrouter1.hidden= true
      }

      circuitPacks.some(item => {
        if (item['circuit-pack-name'] == slots[i]['provisioned-circuit-pack']) {
          console.log("circuitPacks name = ", item['circuit-pack-name'], i);
          // alert('存在这个元素');
          console.log("circuit-pack-components=", item['circuit-pack-components']);
          if (item['circuit-pack-components']) {

            {
              let submoduleroutertmp = {
                path: 'cardstate', component: CardState, name: 'cardstate', leaf: true, iconCls: 'el-icon-message', meta: { title: '' },
              }
              submoduleroutertmp.path = item['circuit-pack-name'];
              submoduleroutertmp.name = item['circuit-pack-name'];
              submoduleroutertmp.meta.title = item['circuit-pack-name'];
              submoduleroutertmp.component = devenums.CircuitPackComponentsEnum.getvaluebyLabel(item['circuit-pack-name']);
              console.log("submoduleroutertmp=", submoduleroutertmp);
              slotrouter1.children.push(submoduleroutertmp);
            }

            console.log("uiDevNum=", item['circuit-pack-components'].length);
            // //add module 
            let uiDevNum = item['circuit-pack-components'].length;
            console.log("uiDevNum=", uiDevNum)

            for (let j = 0; j < uiDevNum; j++) {
              console.log("i=", i, "j=", j)
              let modulerouter = {
                path: 'cardstate', component: CardState, name: 'cardstate', leaf: true, iconCls: 'el-icon-message', meta: {title:''}
              }
              let modulerouter1 = { ...modulerouter }
              console.log("modulerouter = ", modulerouter)
              // let t = Math.floor(Math.random() * 10);
              //modulerouter1.path = slots[i].astDevInfo[j].uiDevType+encodeURIComponent("?slot="+i+"&"+"devidx="+slots[i].astDevInfo[j].uiDevIdx)   // +"/"+slots[i].astDevInfo[j].uiDevIdx
              console.log("modulerouter1.path=", modulerouter1.path)
              //modulerouter1.name = slots[i].astDevInfo[j].uiDevType +"("+ slots[i].astDevInfo[j].uiFuncType+")"

              modulerouter1.path = item['circuit-pack-components'][j]['component-name'] //"aa"+i+j
              modulerouter1.name = item['circuit-pack-components'][j]['component-name'] //"aa"+i+j
              console.log("modulerouter1.name=", modulerouter1.name)

              modulerouter1.meta.title = item['circuit-pack-components'][j]['component-name'] //"aa"+i+j
              // modulerouter1.meta.slot = i;
              // modulerouter1.meta.uiDevIdx=slots[i].astDevInfo[j].uiDevIdx;

              // console.log("devenums.CircuitPackComponentsEnum=", devenums.CircuitPackComponentsEnum)
              // console.log("enumdev type= ", devenums.CircuitPackComponentsEnum.getLabel(item['circuit-pack-components'][j]['component-name']))
              // console.log("enumdev value= ", devenums.CircuitPackComponentsEnum.getvaluebyLabel(item['circuit-pack-components'][j]['component-name']))
              modulerouter1.component = devenums.CircuitPackComponentsEnum.getvaluebyLabel(item['circuit-pack-components'][j]['component-name'])
              //modulerouter1.component = DevOa
              console.log("modulerouter1=", modulerouter1)
              slotrouter1.children.push(modulerouter1)
              // if (j === 0) {
              //   slotrouter1.redirect = modulerouter1.name;
              // }
            }
          }
          else {
            slotrouter1.leaf = true
            // slotrouter1.component = Scc
            slotrouter1.component = devenums.CircuitPackComponentsEnum.getvaluebyLabel(slots[i]['slot-name'])
            // console.log("tttttttttttttttttttt,slotrouter1=",slotrouter1);
            console.log("i", i, "slots[i]['slot-name']=", slots[i]['slot-name']);
            console.log("enumdev type= ", devenums.CircuitPackComponentsEnum.getLabel(slots[i]['slot-name']))
            // console.log(devenums.CircuitPackComponentsEnum.getvaluebyLabel(slots[i]['slot-name']);
          }
          return
        }
      })

      console.log("slotrouter1 = ", slotrouter1)
      boardrouter.push(slotrouter1);

    }
    console.log('boardrouter1=', boardrouter)
    console.log('asyncRouterMap[1]=', asyncRouterMap[1])
    console.log('asyncRouterMap=', asyncRouterMap)

    router.options.routes = asyncRouterMap
    console.log("addRoutes start...", router)
    router.addRoutes(asyncRouterMap) 

    console.log("addRoutes finish...", router)

    resolve(data);
  })
}

//v0.3 user openroadmdevice and opticalmodule to  render board navigation menu
export function generateSlotsRouterByOpenRoadmDeviceAndOpticalModules(device, optmodules) {
  return new Promise(resolve => {
    console.log("generateSlotsRouterByOpenRoadmDeviceAndOpticalModules...");
    console.log("device=", device);
    console.log("modules=", optmodules);

    let shelves = device['org-openroadm-device'].shelves[0];
    console.log("shelves[0]=", shelves)

    let slots = device['org-openroadm-device'].shelves[0].slots;
    console.log("slots=", slots)

    let circuitPacks = device['org-openroadm-device']['circuit-packs']
    console.log("circuit-packs=", circuitPacks)

    let modules = optmodules['optical-modules']
    console.log("optical-modules=", modules)

    console.log(Array.isArray(slots));
    console.log(Array.isArray(circuitPacks));
    console.log(Array.isArray(modules));

    let soltcnt = slots.length;
    console.log("soltcnt=", soltcnt)
    let packcnt = circuitPacks.length
    console.log("packcnt=", packcnt)

    console.log("modulecnt=", Object.getOwnPropertyNames(modules).length);
    // console.log("modulecnt=", Object.keys(modules).length);

    let modulecnt = Object.getOwnPropertyNames(modules).length
    console.log("modulecnt=", modulecnt)

    let boardrouter = asyncRouterMap[1].children;

    boardrouter.length = 0;
    let slotsconfiguration1 = { path: 'slotsconfiguration', component: SlotsConfiguration, name: 'slotsconfiguration', leaf: true, iconCls: 'el-icon-menu', meta: { title: 'Slots Overview',level:2,status:'' } }
    boardrouter.push(slotsconfiguration1)
    for (let i = 0; i < soltcnt; i++) {

      let slotrouter = {
        path: 'slot1',
        component: SlotItem,
        name: 'slot1',
        leaf: false,
        //hidden:true,
        redirect: { name: 'cardstate' },
        iconCls: 'el-icon-success',//iconCls: 'el-icon-circle-check',//图标样式class
        meta: { title: '', level:2 , status:'' },
        children: [
          // { path: 'cardstate', component: CardState, name: 'cardstate', leaf: true, hidden: false, iconCls: 'el-icon-message', meta: 'Card State' },
          // { path: 'cardconfiguration', component: CardConfiguration, name: 'cardconfiguration', leaf: true, hidden: false, iconCls: 'el-icon-message', meta: 'Card Configuration' },
          // { path: 'cardreset', component: CardReset, name: 'cardreset', leaf: true, hidden: false, iconCls: 'el-icon-message', meta: 'Card Reset' },
        ]
      }

      //add slot
      console.log("i=", i)
      let slotrouter1 = { ...slotrouter }
      console.log("slotrouter = ", slotrouter1)

      //slotrouter1.children.length = 0;
      slotrouter1.path = "Slot" + slots[i].label + "" + slots[i]['slot-name'];
      slotrouter1.name = "Slot" + slots[i].label + "" + slots[i]['slot-name'];
      slotrouter1.meta.title = "Slot" + slots[i].label + ":" + slots[i]['slot-name'];
      // let meta = "{ \"title\": \"" + "Slot" + slots[i].label + ":" + slots[i]['slot-name'] + "\"}";
      // slotrouter1.meta = JSON.parse(meta)
      if (slots[i]['slot-status'] === 'empty-not-prov') {
        console.log("Slot is empty!!!!!!!!!!!!!!!!!", i)
        slotrouter1.leaf = true
        slotrouter1.iconCls = 'el-icon-warning-outline'
        // slotrouter1.component = DevOa
        // slotrouter1.hidden= true
        //这里用continue提供性能，减少后续判断，但需要先加上后面两句，否在empty的slot将不显示在导航栏
        console.log("slotrouter1 = ", slotrouter1)
        boardrouter.push(slotrouter1);

        slotrouter1.meta.status = "deactivate";
        continue
      }
      else{
        slotrouter1.meta.status = "activate";
      }

      {
        //每个slot固定有的，用于板级信息显示
        let submoduleroutertmp = {
          path: 'cardstate', component: CardState, name: 'cardstate', leaf: true, iconCls: 'el-icon-set-up',meta: { title: 'CardState',level:3,status:''  }
        }
        // submoduleroutertmp.path = "CardState " + slots[i]['slot-name'];
        // submoduleroutertmp.name = "CardState " + slots[i].label;
        // submoduleroutertmp.meta = "Card State";       
        // submoduleroutertmp.component = CardState
        submoduleroutertmp.path = slots[i]['provisioned-circuit-pack'];
        submoduleroutertmp.name = slots[i]['provisioned-circuit-pack'];
        console.log("devenumsslotname=", devenums.CircuitPackComponentsEnum.getvaluebyLabel(slots[i]['slot-name']));
        submoduleroutertmp.component = devenums.CircuitPackComponentsEnum.getvaluebyLabel(slots[i]['slot-name'])
        console.log("submoduleroutertmp=", submoduleroutertmp);
        slotrouter1.children.push(submoduleroutertmp);
        slotrouter1.redirect = slotrouter1.name + "/" + slots[i]['provisioned-circuit-pack'];
      }

      //利用opticalmodules 生成二级（光模块）菜单，根据circuit-pack-name放置到相应slot下
      console.log("!!!!!!!!!modules cnt=", modulecnt);
      for (let key of Object.keys(modules)) {
        let module = modules[key];
        // ... do something with mealName
        console.log("key=", key);
        console.log("value=", module);

        const slotname = slots[i]['provisioned-circuit-pack']
        if (key == 'supervisory-channel') {
          console.log("current module = supervisory-channel");
          let curComponent = DevOsc;
          console.log(module.oscs);
          let flag = Array.isArray(module.oscs)
          let arr = module.oscs;
          moduleAdaptation(slotname, slotrouter1, arr, key, curComponent, flag, key)
        }
        else if (key == 'channel-monitor-modules') {
          console.log("current module = channel-monitor-modules");
          let curComponent = DevOcm;
          console.log(module.ocms);
          let flag = Array.isArray(module.ocms)
          let arr = module.ocms;
          moduleAdaptation(slotname, slotrouter1, arr, key, curComponent, flag, key)
        }
        else if (key == 'optical-attenuator') {
          console.log("current module = optical-attenuator");
          let curComponent = DevVoa;
          console.log(module.attenuators.attenuator);
          let flag = Array.isArray(module.attenuators.attenuator)
          let arr = module.attenuators.attenuator;
          moduleAdaptation(slotname, slotrouter1, arr, key, curComponent, flag, key)
        }
        else if (key == 'optical-amplifier') {
          console.log("current module = optical-amplifier");
          let curComponent = DevOa;
          console.log(module.amplifiers.amplifier);
          let flag = Array.isArray(module.amplifiers.amplifier)
          let arr = module.amplifiers.amplifier;
          moduleAdaptation(slotname, slotrouter1, arr, key, curComponent, flag, key)
        }
        else if (key == 'optical-otdr') {
          console.log("current module = optical-otdr");
          let curComponent = DevOtdr;
          console.log(module['circuit-pack-name']);
          //let item = module;
          let flag = Array.isArray(module)
          let arr = new Array(module)
          moduleAdaptation(slotname, slotrouter1, arr, key, curComponent, flag, key)
        }
        else if (key == 'optical-mux') {
          console.log("current module = optical-mux");
          let curComponent = DevOa;
          console.log(module['circuit-pack-name']);
          //let item = module;
          let flag = Array.isArray(module)
          let arr = new Array(module)
          moduleAdaptation(slotname, slotrouter1, arr, key, curComponent, flag, key)
        }
        else if (key == 'optical-mirror') {
          console.log("current module = optical-mirror");
          let curComponent = DevOa;
          console.log(module['circuit-pack-name']);
          // let item = module;
          let flag = Array.isArray(module)
          let arr = new Array(module)
          moduleAdaptation(slotname, slotrouter1, arr, key, curComponent, flag, key)
        }
        else if (key == 'default-waveplan') {
          console.log("current module = default-waveplan");
          let curComponent = DevWaveplan;
          console.log(module['circuit-pack-name']);
          // let item = module;
          let flag = Array.isArray(module)
          let arr = new Array(module)
          moduleAdaptation(slotname, slotrouter1, arr, key, curComponent, flag, key)
        }
      }

      console.log("slotrouter1 = ", slotrouter1)
      boardrouter.push(slotrouter1);

      // console.log('i=', i)
      // console.log("boardrouter=", boardrouter)
    }

    console.log('boardrouter1=', boardrouter)
    console.log('asyncRouterMap[1]=', asyncRouterMap[1])
    console.log('asyncRouterMap=', asyncRouterMap)

    router.options.routes = asyncRouterMap
    console.log("addRoutes start...", router)
    router.addRoutes(asyncRouterMap) 

    console.log("addRoutes finish...", router)

    resolve([device, optmodules]);
  })
}

function moduleAdaptation(slotname, slotrouter1, arr, key, curComponent, flag, devName) {
  arr.some((item, index) => {
    console.log("===item 0 ,mounted on=", item['circuit-pack-name']);
    if (item.mounted != true) {
      if (item['circuit-pack-name'] == slotname) {
        //找到了进行添加子菜单操作，这里应该可以封装一个函数
        console.log("key=", key);
        {
          let submoduleroutertmp = {
            path: 'cardstate', component: CardState, name: 'cardstate', leaf: true, iconCls: 'el-icon-cpu', meta: {title:'',level:3,status:''}
          }
          if (flag) {
            submoduleroutertmp.path = item.name;
            submoduleroutertmp.name = item.name;
            submoduleroutertmp.meta.title = item.name;
          } else {
            submoduleroutertmp.path = devName;
            submoduleroutertmp.name = devName;
            submoduleroutertmp.meta.tiltle = devName;          
          }
          // submoduleroutertmp.component = devenums.CircuitPackComponentsEnum.getvaluebyLabel(item['circuit-pack-name']);
          submoduleroutertmp.component = curComponent;
          console.log("submoduleroutertmp=", submoduleroutertmp);
          slotrouter1.children.push(submoduleroutertmp);
        }
        item.mounted = true;
      }
    }
  })
}
const router = new VueRouter({
  routes: asyncRouterMap
})
export default router
