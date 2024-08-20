<template>
  <div class="main">
    <!-- <h1>Inventory</h1> -->

    <el-tabs v-model="activeName" @tab-click="handleClick" class="tab">
      <el-tab-pane label="Inventory Infomation" name="first">
        <Module></Module>
      </el-tab-pane>
      <!-- <el-tab-pane label="Active Alarms" name="second">
        <active-alarm></active-alarm>
      </el-tab-pane>-->
    </el-tabs>
  </div>
</template>


<script>
import Module from "./ModuleItem";
// import ActiveAlarm from '@/views/app/ActiveAlarm'
export default {
  components: {
    Module: Module
    // ActiveAlarm
  },
  data() {
    return {
      activeName: "first",
      SystemInfomation: "Inventory Infomation"
    };
  },
  methods: {
    handleClick(tab, event) {
      ////console.log(tab, event);
    }
  },
  mounted() {
    ////console.log("mount Inventory");
  }
};
</script>

<style lang="scss" scoped>
.main {
  margin-left: 30px;
}
</style>