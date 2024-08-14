/**
 * 动态插入css
 */

export const loadStyle = url => {
  const link = document.createElement('link')
  link.type = 'text/css'
  link.rel = 'stylesheet'
  link.href = url
  const head = document.getElementsByTagName('head')[0]
  head.appendChild(link)
}
/**
 * 设置浏览器头部标题
 */
export const setTitle = function(title) {
  title = title ? `${title}` : 'Demo'
  window.document.title = title
}

export function urlParse(url) {
  let obj ={}
  let reg = /[?&][^?&]+=[^?&%]+/g;
  // let url = this.$route.path//window.location.search
  console.log('url=',url)
  let arr =url.match(reg)
   console.log('arr=',arr) 
  arr.forEach((item)=>{
      let tempArr=item.substring(1).split('=');
      let key=decodeURIComponent(tempArr[0])
      let val=decodeURIComponent(tempArr[1])
      obj[key]=val
  })
  console.log('obj=',obj) 
  return obj
}

function deepCopy(obj) {
  if (typeof obj != 'object') {
    return obj;
  }
  var newobj = {};
  for (var attr in obj) {
    newobj[attr] = deepCopy(obj[attr]);
  }
  return newobj;
}


