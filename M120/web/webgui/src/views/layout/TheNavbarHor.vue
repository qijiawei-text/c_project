<template>
  <div>
    <el-col :span="24" class="navbarhor">
      <el-col :span="8" style="min-height:1px"></el-col>
      <el-col :span="16">
        <el-menu
          :default-active="activeIndex"
          class="el-menu-demo"
          mode="horizontal"
          @select="handleSelect_h"
        >
          <el-menu-item index="1">{{ generateTitle("Home") }}</el-menu-item>
          <el-menu-item index="2">{{
            generateTitle("Board Management")
          }}</el-menu-item>
          <el-menu-item index="3">{{
            generateTitle("NE Management")
          }}</el-menu-item>
          <el-menu-item index="4">{{ generateTitle("Help") }}</el-menu-item>
        </el-menu>
      </el-col>
    </el-col>
  </div>
</template>

<script>
import { asyncRouterMap } from "@/router";
import { apiGet, getSlotsInfo, getSlotsThenStore } from "@/api/restGet";
import { generateTitle } from "@/utils/i18n";
export default {
  data() {
    return {
      activeIndex: "1",
      nodes: asyncRouterMap[0].children,
    };
  },
  methods: {
    handleSelect_h(key, keyPath) {
      this.nodes = asyncRouterMap[key - 1].children;
      let tmp = {
        nodes: this.nodes,
        curtab: key - 1,
      };
      this.$emit("childByValue", tmp);
      this.$router.push({
        path: "/" + asyncRouterMap[key - 1].children[0].name,
      });
    },
    generateTitle,
  },
  mounted() {},
};
</script>

<style lang="scss" scoped>
.navbarhor {
  //  border: 6px solid yellow;
  border-bottom: 1px solid #d8dce5;
}
</style>
