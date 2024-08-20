<template>
  <div class="mainitem-container">
    <el-col :span="24" class="historypane-wrapper">
      <history-pane></history-pane>
    </el-col>
    <el-col :span="24" class="breadcrumb-wrapper">
      <!-- <strong class="title">{{ $route.name }}</strong> -->
      <el-breadcrumb
        separator-class="el-icon-arrow-right"
        class="breadcrumb-inner"
      >
        <el-breadcrumb-item
          v-for="(item, index) in $route.matched"
          :key="item.name"
          :to="item.path"
          >{{ item.name }}</el-breadcrumb-item
        >
      </el-breadcrumb>
    </el-col>
    <el-col :span="24" class="content-wrapper">
      <transition name="fade-transform" mode="out-in">
        <router-view v-if="isRouterAlive"></router-view>
      </transition>
    </el-col>
  </div>
</template>

<script>
import HistoryPane from "@/views/layout/components/HistoryPane";
export default {
  components: {
    HistoryPane,
  },
  data() {
    return {
      isRouterAlive: true,
    };
  },
  computed: {},
  methods: {
    toPath(item) {
      let name = item.name;
      let path = "{ path: " + name + "}";
      console.log("path=", path);
      return;
    },
  },
};
</script>

<style lang="scss" scoped>
.historypane-wrapper {
  background-color: #ffe;
  margin-top: 3px;
  margin-left: 8px;
  width: 98.5%;
  // border: 1px solid goldenrod;
}
.breadcrumb-wrapper {
  margin-bottom: 12px;
  margin-left: 16px;
  margin-top: 15px;
  .title {
    width: 200px;
    float: left;
    color: #475669;
  }
  .breadcrumb-inner {
    float: left;
  }
}
.content-wrapper {
  background-color: #fff;
  box-sizing: border-box;
  margin-left: 16px;
}
</style>
