<template>
  <div class="app-wrapper">
    <the-header></the-header>

    <the-navbar-hor
      ref="thenavbarhor"
      @childByValue="childByValue"
    ></the-navbar-hor>

    <!-- <el-col :span="24" class="main"> -->
    <the-sidebar
      :nodes="nodes"
      :curtab="curtab"
      class="sidebar-container"
    ></the-sidebar>

    <!-- <section class="content-container"> -->
    <the-main class="main-container"></the-main>
    <!-- </section> -->
    <!-- </el-col> -->
  </div>
</template>

<script>
import { mapGetters } from "vuex";
import { generateTitle } from "@/utils/i18n";
import { asyncRouterMap } from "@/router";
import router from "@/router";
import { apiGet, getSlotsInfo, getSlotsThenStore } from "@/api/restGet";
import { getSlotsByOpenRoadmDeviceThenStore } from "@/api/device";
import {
  getSlotsByOpenRoadmDeviceAndOpticalModulesThenStore,
  getSlotsByOpenRoadmDeviceAndOpticalModules,
} from "@/api/navbar";
import { getVoaListThenStore } from "@/api/voa";
import { getOpticalModulesThenStore } from "@/api/opticalModule";

import TheHeader from "@/views/layout/TheHeader";
import TheNavbarHor from "@/views/layout/TheNavbarHor";
import TheSidebar from "@/views/layout/TheSidebar";
import TheMain from "@/views/layout/TheMain";
import { findRouteFromWhole, findRouteFromCurTab } from "@/utils/path";
export default {
  data() {
    return {
      nodes: asyncRouterMap[0].children,
      //   nodes: this.$router.options.routes,
      curtab: 1,
    };
  },
  created() {},
  methods: {
    onSubmit() {
      console.log("submit!");
    },

    showAlarm() {
      console.log("show alarm");
    },
    childByValue: function(childValue) {
      this.nodes = childValue.nodes;
      this.curtab = childValue.curtab;
    },

    syncSidebar() {
      // let ret = findRouteFromCurTab(this.$route.name, this.curtab);
      let ret = findRouteFromWhole(
        asyncRouterMap,
        this.$route.name,
        this.curtab
      );
      if (ret >= 0) {
        this.nodes = asyncRouterMap[ret].children;
      } else {
        this.nodes = this.$refs.thenavbarhor.nodes;
      }
    },
  },
  mounted() {
    console.log("Main mounted...");

    if (window.performance.navigation.type == 1) {
      console.log("页面被刷新");
      this.$router.push({ path: "/" });
    } else {
      console.log("首次被加载");
    }

    console.log("Navbar mounted...");
    getOpticalModulesThenStore(this);
    // //getSlotsThenStore(this);
    // getSlotsByOpenRoadmDeviceThenStore(this);
    getVoaListThenStore(this);
    // setInterval(() => {
    //   getSlotsThenStore(this);
    // }, 10000);
    getSlotsByOpenRoadmDeviceAndOpticalModulesThenStore(this).then(() => {
      this.syncSidebar();
    });
  },
  beforeRouteUpdat(to, from, next) {
    next();
  },

  watch: {
    $route: function() {
      console.log("current router is", this.$route.name);
      this.syncSidebar();
    },
  },
  components: {
    TheHeader,
    TheNavbarHor,
    TheSidebar,
    TheMain,
  },
};
</script>

<style scoped lang="scss">
@import "src/styles/mixin.scss";
.app-wrapper {
  // @include clearfix;
  position: relative;
  height: 100%;
  overflow: hidden;
}
</style>
