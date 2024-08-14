<template>
  <div>
    <!-- <h1> DEV_VOA</h1> -->
    <!-- <voa-form :name="name" ref="formComp"></voa-form> -->
    <el-tabs v-model="activeName" @tab-click="handleClick" class="tab">
      <el-tab-pane label="Config" name="first">
        <voa-cfg-form :name="name" ref="formComp"></voa-cfg-form>
      </el-tab-pane>
      <el-tab-pane label="State" name="second">
        <voa-state-form :name="name" ref="formComp1"></voa-state-form>
      </el-tab-pane>
    </el-tabs>
  </div>
</template>

<script>
import { getVoa } from "@/api/voa";
import VoaForm from "@/views/app/board/dev/compenents/VoaForm";
import VoaCfgForm from "@/views/app/board/dev/compenents/VoaCfgForm";
import VoaStateForm from "@/views/app/board/dev/compenents/VoaStateForm";
export default {
  components: {
    VoaForm,
    VoaCfgForm,
    VoaStateForm,
  },
  data() {
    return {
      activeName: "first",
      name: this.$route.meta,
    };
  },
  mounted() {
    console.log("DevVoa mouted, route=", this.$route.meta);
  },

  methods: {
    checkVoaPort() {
      this.name = this.$route.meta.toLowerCase();
      console.log("voa name=", this.name);
      // if( 0 < (this.$route.meta).toLowerCase().indexOf("padding-voa")){
      //     this.name  = "padding-voa"
      // }
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
      this.checkVoaPort();
      this.$refs.formComp.doQuery(getVoa, this.name);
      this.$refs.formComp1.doQuery(getVoa, this.name);
    },
  },
};
</script>

<style lang="scss" scoped></style>
