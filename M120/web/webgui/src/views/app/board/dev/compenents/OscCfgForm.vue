<template>
  <div class="form">
    <!-- <h1>osc cfg</h1> -->
    <template v-if="form['osc-tx-config']">
      <el-form
        ref="form"
        :model="form"
        label-width="240px"
        label-position="left"
      >
        <el-card>
          <div class="index-content">Config</div>
          <el-form-item label="osc-sfp-enable">
            <el-switch
              v-model="form['osc-tx-config']['osc-sfp-enable']"
            ></el-switch>
          </el-form-item>
        </el-card>
      </el-form>
    </template>
    <div class="query">
      <el-button
        type="primary"
        @click="submitQueryForm"
        style=" background-color:#79CB79"
        >Query</el-button
      >

      <el-button @click="submitApplyForm" style=" background-color:#FBAE42"
        >Apply</el-button
      >
    </div>
  </div>
</template>

<script>
import { getOscCfg, setOscCfg } from "@/api/osc";
export default {
  props: {
    config: {
      required: true,
    },
  },
  data() {
    return {
      edit: true,
      // form: this.config,
    };
  },
  computed: {
    form() {
      return this.config;
    },
  },
  methods: {
    submitQueryForm() {
      this.doQuery(getOscCfg);
    },
    submitApplyForm() {
      this.doApply(this.form["osc-tx-config"], setOscCfg);
    },
    doQuery(func) {
      (async () => {
        let results = await func();
        console.log("results=", results);
        // console.log("this.form=",this.form);
        // this.$refs.form.resetFields();
        this.config["osc-tx-config"] = results.data["osc-tx-config"];
        console.log("response config=", results.data);
        console.log("this.config=", this.config);
      })();
    },
    doApply(form, func) {
      alert("submit!");
      console.log("表单校验通过");
      (async () => {
        let results = await func(form);
        console.log("submit OcmConfig Setting results=", results);
        console.log("will OcmConfig get ...");
        this.submitQueryForm();
      })();
    },
  },
  mounted() {
    console.log("OscconfigForm mounted,config=", this.config);
    console.log("OscconfigForm mounted,form=", this.form);
  },
};
</script>

<style lang="scss" scoped>
.form {
  text-align: left;
  .el-form-item {
    margin-top: 4px;
    margin-bottom: 1px;
    padding: 0;
    margin: 1px;
  }
}
.query {
  margin-top: 20px;
  text-align: center;
}
</style>
