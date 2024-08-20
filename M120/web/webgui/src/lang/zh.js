
export default {
 
  // 导航栏
  navbar: {
    title: '管理系统',
    languageSwitch: '语言切换',
    theme: '主题'
  },
  skin: {
    Blue: '蓝',
    Green: '绿',
    Red: '红',
    Purple: '紫',
    Default: '默认'
  },
  route: {
    LogOut:"注销",
    "System Settings":"系统设置",
    System: '系统信息',
    Inventory:'清单' ,
    Version:'版本',
    Slot1:'插槽1',
    Slot2:'插槽2',
    Module:'模块',
    Control:'控制',
    "Control Module":'控制模块',
    Power:'电源',
    FAN:'风扇',
    Network:'网络',
    Authentication:'认证',
    Help:'帮助',
    // About:'关于',

    errorPages: '错误页面',
    page401: '401',
    page404: '404',

    homepage:'主页',

    Slotsconfiguration:'slots信息',

    system:'系统',
    Reset:'重置',
    'Clock/NTP': '时钟/NTP',
    State:'状态',
    'Restart/Shutdown':'重启/关机',
    'IP Config':'IP配置',

    Monitor:'监控',
    Currentalarms:'当前告警',
    Historyalarms:'历史告警',
    Alarmsetting:'告警设置', 
    Logs:'日志',  

    Security:'安全',
    Localuser:'当前用户',
    Snmp:'简单网络管理协议',
    Session:'Session',
    Sslcertificates:'SSL证书',
    Iptable:'防火墙',
    RadiusService:'Radius服务',
    TacacsService:'Tacacs服务',
    UserManagement:'用户管理',
    Service:'服务',

    Upgrade:'升级',
    Localupgrade:'本地升级',
    Remoteupgrade:'远程升级',
    Syslog:'系统日志',

    About:'关于',
    Legalnotice:'开源信息',
    Support:'支持',
    Apitest:'API测试',
    Websocket:'Websocket测试',
    "OA-PA":"PA测试",
    "OA-BA":"BA测试",
    Omtconfig:"Omt配置",

    Home: '主页',
    Homepage:'主界面',
    'Board Management':'板级管理',
    'NE Management':'网元管理',
 
    Activate:'Activate',
    Deactivate:'Deactivate'
  },
  permission: {
    roles: '你的权限',
    switchRoles: '切换权限'
  },
  excel: {
    export: '导出',
    selectedExport: '导出已选择项',
    placeholder: '请输入文件名(默认excel-list)'
  },
  zip: {
    export: '导出',
    placeholder: '请输入文件名(默认file)'
  },
  components: {

  },

  invalid:{
    ip: '错误的IP地址格式,请输入 xxx.xxx.xxx.xxx ',
    port: '错误的Port地址格式',
    netmask: '错误的netmask地址格式',
    defaultgw: '错误的default网关地址格式'

    
  },
  tagsView: {
    close: '关闭',
    closeOthers: '关闭其它',
    closeAll: '关闭所有'
  },
  form:{
    'Query': '查询',
    'Add': '添加',  

    'os-version': '系统版本',
    'os-name':'系统名称',
    'os-release':'系统发布',
    'machine':'机器',
    'boot-datetime':'启动时间',
    'current-datetime':'当前时间',

    'hostname': '主机名',
    'contact':'联系方式',
    'location':'地址'

  }
}
