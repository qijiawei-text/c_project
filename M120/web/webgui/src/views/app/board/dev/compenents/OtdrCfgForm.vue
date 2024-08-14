<template>
  <div class="form">
    <!-- <h1>otdr cfg</h1> -->
    <el-form ref="form" :model="form" label-width="240px" label-position="left">
      <el-card>
        <div class="index-content">Config</div>
        <el-form-item label="name" prop="config.interval">
          <el-input v-model="form.config.interval" placeholder=""></el-input>
        </el-form-item>
        <el-form-item label="scan-type" prop="config.scan-type">
          <el-select
            v-model="form.config['scan-type']"
            placeholder="请选择"
            style="width:100%"
          >
            <el-option
              v-for="item in options"
              :key="item"
              :label="item"
              :value="item"
            >
            </el-option>
          </el-select>
        </el-form-item>

        <el-form-item label="measuring-time" prop="config.measuring-time">
          <el-input
            v-model="form.config['measuring-time']"
            placeholder=""
          ></el-input>
        </el-form-item>

        <el-form-item label="event-trigger">
          <el-switch v-model="form.config['event-trigger']"></el-switch>
        </el-form-item>
        <el-form-item label="user-ior" prop="config.user-ior">
          <el-input v-model="form.config['user-ior']" placeholder=""></el-input>
        </el-form-item>

        <el-form-item label="distance-range" prop="config.distance-range">
          <el-input
            v-model="form.config['distance-range']"
            placeholder=""
          ></el-input>
        </el-form-item>

        <el-form-item label="pulse-width" prop="config.pulse-width">
          <el-input
            v-model="form.config['pulse-width']"
            placeholder=""
          ></el-input>
        </el-form-item>

        <el-form-item label="sample-resolution" prop="config.sample-resolution">
          <el-input
            v-model="form.config['sample-resolution']"
            placeholder=""
          ></el-input>
        </el-form-item>

        <el-form-item label="reflectance-thr" prop="config.reflectance-thr">
          <el-input
            v-model="form.config['reflectance-thr']"
            placeholder=""
          ></el-input>
        </el-form-item>

        <el-form-item label="splice-loss-thr" prop="config.splice-loss-thr">
          <el-input
            v-model="form.config['splice-loss-thr']"
            placeholder=""
          ></el-input>
        </el-form-item>

        <el-form-item label="fiber-end-thr" prop="config.fiber-end-thr">
          <el-input
            v-model="form.config['fiber-end-thr']"
            placeholder=""
          ></el-input>
        </el-form-item>
      </el-card>
    </el-form>
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
import { getOtdrCfg, setOtdrCfg } from "@/api/otdr";
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
      options: ["customized", "short", "medium", "long", "all"],
    };
  },
  computed: {
    form() {
      return this.config;
    },
  },
  methods: {
    submitQueryForm() {
      this.doQuery(getOtdrCfg);
    },
    submitApplyForm() {
      this.doApply(this.form.config, setOtdrCfg);
    },
    doQuery(func) {
      (async () => {
        let results = await func();
        console.log("results=", results);
        // console.log("this.form=",this.form);
        // this.$refs.form.resetFields();
        this.config.config = results.data.config;
        console.log("nnnnnnnnnnn, response config=", results.data);
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
    console.log("OtdrconfigForm mounted,config=", this.config);
  },
};
</script>

<style lang="scss" scoped>
.form {
  text-align: left;
  //  display: flex;
  //  width: auto;
  /* width: 400px; */
  /* height: 350px;
  padding: 20px;
  border: 1px solid #ccc;
  margin-left: 30px; */
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
// .index-content {
//   background-color: #d9edf7;
//   text-align: left;
//   margin-top: 8px;
// }
</style>
