<template>
  <div>
    <!-- <h1> DEV_OA</h1> -->
    <!-- <oa-form :name="name" ref="formComp"></oa-form> -->
    <el-tabs v-model="activeName" @tab-click="handleClick" class="tab">
      <el-tab-pane label="Config" name="first">
        <oa-cfg-form></oa-cfg-form>
      </el-tab-pane>
      <el-tab-pane label="State" name="second">
        <oa-state-form></oa-state-form>
      </el-tab-pane>
    </el-tabs>
  </div>
</template>

<script>
import { getOAList, setOAList, getOA, setOA } from "@/api/oa";
import OaCfgForm from "@/views/app/board/dev/compenents/OaCfgForm";
import OaStateForm from "@/views/app/board/dev/compenents/OaStateForm";
export default {
  components: {
    OaCfgForm,
    OaStateForm,
  },
  data() {
    return {
      activeName: "first",
      name: this.$route.meta,
    };
  },
  mounted() {
    console.log("DevOa mouted, route=", this.$route.meta);
  },

  methods: {
    checkOAType() {
      console.log("is pa?=", this.$route.meta.toLowerCase().indexOf("pa"));
      console.log("is ba?=", this.$route.meta.toLowerCase().indexOf("ba"));
      if (0 < this.$route.meta.toLowerCase().indexOf("pa")) {
        this.name = "PA";
      }
      if (0 < this.$route.meta.toLowerCase().indexOf("ba")) {
        this.name = "BA";
      }
    },
    handleClick(tab, event) {
      // console.log(tab, event);
      console.log("tab.name=", tab.name);
      console.log("tab.label=", tab.label);
      if (tab.label != "config") {
        console.log("parseInt(tab.name)=", parseInt(tab.name));
        // this.doQueryPort(parseInt(tab.name),getOtdrPort,tab.label)
      }
      // console.log("event=",event);
    },
  },
  watch: {
    $route: function() {
      console.log("current router is", this.$route.name);
      this.checkOAType();
      this.$refs.formComp.doQuery(getOA, this.name);
    },
  },
};
</script>

<style lang="scss" scoped></style>
