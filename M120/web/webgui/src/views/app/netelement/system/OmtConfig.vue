<template>
  <div class="main">
    <!-- <h1>System Setting</h1> -->
    <el-tabs v-model="activeName" @tab-click="handleClick" class="tab">
      <el-tab-pane label="System" name="first">
        <Systeminfo></Systeminfo>
      </el-tab-pane>
      <!-- <el-tab-pane label="Active Alarms" name="second">
        <active-alarm></active-alarm>
      </el-tab-pane>-->
    </el-tabs>
  </div>
</template>

<script>
import Systeminfo from "./Info";
// import ActiveAlarm from '@/views/app/ActiveAlarm'
export default {
  components: {
    Systeminfo: Systeminfo,
    // ActiveAlarm
  },
  data() {
    return {
      activeName: "first",
    };
  },
  methods: {
    handleClick(tab, event) {
      //console.log(tab, event);
    },
  },
  mounted() {
    //console.log("mount System Setting");
  },
};
</script>

<style lang="scss" scoped>
.main {
  // background-color: green;
  margin-left: 30px;
}
</style>
