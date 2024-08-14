<template>
  <div>
    <h1>DEV_OPS</h1>
  </div>
</template>

<script>
import { urlParse } from "@/utils/util";
export default {
  mounted() {
    console.log("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!ocm mounted!!!!!!!!!");
    console.log(this.$route);
    console.log(this.$route.params);
    console.log(this.$route.query);

    // let res=this.urlParse()
    // let res = urlParse(decodeURIComponent(this.$route.path))
    // console.log('res=',res)
  },
  methods: {
    //parse  ?id=1234&value=456 => {id:1234,value:456}
    urlParse() {
      let obj = {};
      let reg = /[?&][^?&]+=[^?&%]+/g;
      let url = this.$route.path; //window.location.search
      console.log("url=", url);
      let arr = url.match(reg);
      console.log("arr=", arr);
      arr.forEach((item) => {
        let tempArr = item.substring(1).split("=");
        let key = decodeURIComponent(tempArr[0]);
        let val = decodeURIComponent(tempArr[1]);
        obj[key] = val;
      });
      console.log("obj=", obj);
      return obj;
    },
  },
};
</script>

<style lang="scss" scoped></style>
