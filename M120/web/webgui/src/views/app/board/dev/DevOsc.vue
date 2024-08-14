<template>
  <div>
    <!-- <h1> DEV_OSC</h1> -->
    <!-- <el-button type="primary" @click="submitQueryForm">osc get</el-button> -->
    <el-tabs v-model="activeName" @tab-click="handleClick" class="tab">
      <el-tab-pane label="osc-tx-config" name="first">
        <osc-cfg-form :config="form"></osc-cfg-form>
      </el-tab-pane>
      <el-tab-pane label="osc-monitor" name="second">
        <osc-state-form
          :config="form['osc-monitor']"
          @ParentsubmitQueryForm="submitQueryForm"
        ></osc-state-form>
      </el-tab-pane>
    </el-tabs>
  </div>
</template>

<script>
import { getOsc } from "@/api/osc";
import OscCfgForm from "@/views/app/board/dev/compenents/OscCfgForm";
import OscStateForm from "@/views/app/board/dev/compenents/OscStateForm";
export default {
  components: {
    OscCfgForm,
    OscStateForm,
  },
  data() {
    return {
      activeName: "first",
      name: this.$route.meta,
      form: {},
      config: [],
    };
  },
  methods: {
    submitQueryForm() {
      this.doQuery(getOsc, this.name);
    },
    doQuery(func, name) {
      (async () => {
        let results = await func(name);
        console.log("results=", results);
        console.log("this.form=", this.form);
        // this.$refs.form.resetFields();
        this.form = results.data["osc"];
        console.log("osc=", this.form);
      })();
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
  created() {
    console.log("otdr created....");
    // this.submitQueryForm();
  },
  mounted() {
    console.log("DevOsc mounted,name=", this.$route.meta);
    this.submitQueryForm();
  },
};
</script>

<style lang="scss" scoped></style>
