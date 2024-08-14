<template>
  <!-- <div class="thesidebar"> -->
  <!-- <aside> -->
  <el-scrollbar wrapClass="scrollbar-wrapper">
    <el-menu
      mode="vertical"
      :show-timeout="800"
      :default-active="$route.name"
      :collapse="false"
      class="el-menu-vertical-demo"
      @open="handleopen"
      @close="handleclose"
      @select="handleselect_v"
      theme="dark"
      router
      unique-opened
    >
      <!-- <menu-tree :nodes="$router.options.routes[0].children"></menu-tree> -->
      <menu-tree :nodes="nodes" :curtab="curtab"></menu-tree>

      <!-- <sidebar-item :nodes="nodes"></sidebar-item> -->
    </el-menu>
  </el-scrollbar>
  <!-- </aside> -->
  <!-- </div> -->
</template>

<script>
import MenuTree from "@/views/layout/components/MenuTree";
import { mapMutations, mapState, mapGetters } from "vuex";
import { asyncRouterMap } from "@/router";
export default {
  props: ["nodes", "curtab"],
  data() {
    return {
      //nodes:asyncRouterMap[0].children
    };
  },

  methods: {
    handleopen() {
      //console.log('handleopen');
    },
    handleclose() {
      //console.log('handleclose');
    },
    handleselect_v: function(a, b) {},
  },
  computed: {
    // ...mapGetters(["nodes"])
    //    nodes(){
    //        return this.$store.getters.nodes
    //    }
  },
  components: {
    MenuTree,
  },
  //need define nodes in data
  //   watch: {
  //   $route: function() {
  //     console.log("store=",this.$store)
  //     console.log("this.$store.getters.nodes=", this.$store.getters.nodes);
  //     this.nodes = this.$store.getters.nodes;
  //   }
  // },
};
</script>

<style lang="scss" scoped></style>
