<template>
  <div class="form">
    <!-- <h2>ConfigForm.vue</h2> -->
    <!-- <h1>{{name}}</h1> -->
    <el-form ref="form" :model="form" label-width="240px" label-position="left">
      <el-card>
        <!-- <span>Config</span> -->
        <div class="index-content">Config</div>
        <el-form-item label="target-gain" prop="amplifier.config.target-gain">
          <el-input
            v-model="form.amplifier.config['target-gain']"
            placeholder=""
          ></el-input>
        </el-form-item>
        <el-form-item
          label="target-output-power"
          prop="amplifier.config.target-output-power"
        >
          <el-input
            v-model="form.amplifier.config['target-output-power']"
            placeholder=""
          ></el-input>
        </el-form-item>
        <el-form-item label="amp-mode" prop="amplifier.config.amp-mode">
          <el-input
            v-model="form.amplifier.config['amp-mode']"
            placeholder=""
          ></el-input>
        </el-form-item>
        <el-form-item
          label="target-gain-tilt"
          prop="amplifier.config.target-gain-tilt"
        >
          <el-input
            v-model="form.amplifier.config['target-gain-tilt']"
            placeholder=""
          ></el-input>
        </el-form-item>
        <el-form-item label="type" prop="amplifier.config.type">
          <el-input
            v-model="form.amplifier.config.type"
            placeholder=""
          ></el-input>
        </el-form-item>
        <el-form-item label="plim-enabled">
          <el-switch
            v-model="form.amplifier.config['plim-enabled']"
          ></el-switch>
        </el-form-item>
        <el-form-item label="enabled">
          <el-switch v-model="form.amplifier.config.enabled"></el-switch>
        </el-form-item>
        <el-form-item label="tygain-rangepe" prop="amplifier.config.gain-range">
          <el-input
            v-model="form.amplifier.config['gain-range']"
            placeholder=""
          ></el-input>
        </el-form-item>
        <el-form-item label="apr-enabled">
          <el-switch v-model="form.amplifier.config['apr-enabled']"></el-switch>
        </el-form-item>
        <el-form-item label="plim-power" prop="amplifier.config.plim-power">
          <el-input
            v-model="form.amplifier.config['plim-power']"
            placeholder=""
          ></el-input>
        </el-form-item>
        <el-form-item label="name" prop="amplifier.config.name">
          <el-input
            v-model="form.amplifier.config.name"
            placeholder=""
          ></el-input>
        </el-form-item>
        <el-form-item label="apr-power" prop="amplifier.config.apr-power">
          <el-input
            v-model="form.amplifier.config['apr-power']"
            placeholder=""
          ></el-input>
        </el-form-item>
        <el-form-item label="autolos">
          <el-switch v-model="form.amplifier.config.autolos"></el-switch>
        </el-form-item>
      </el-card>

      <!-- 
        <el-form-item label="actual-gain" prop="amplifier.state.actual-gain">
          <el-input
            v-model="form.amplifier.state['actual-gain']"
            placeholder=""
          ></el-input>
        </el-form-item> -->
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
import { notEmpty, validateName } from "@/utils/validateform";
import { getOAList, setOAList, getOA, setOA } from "@/api/oa";

export default {
  props: ["name"],
  data() {
    return {
      edit: true,
      form: {
        amplifier: {
          name: "string",
          state: {
            "target-gain": "12wwww",
            "target-output-power": "string",
            "amp-mode": "string",
            "target-gain-tilt": "string",
            type: "string",
            "plim-enabled": false,
            enabled: false,
            "gain-range": "string",
            "apr-enabled": false,
            "plim-power": "string",
            name: "string",
            "apr-power": "string",
            autolos: false,
            "optical-return-loss": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
            "input-power-c-band": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
            "msa-input-power-c-band": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
            "laser-bias-current": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
            "input-power-l-band": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
            "actual-gain-tilt": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
            "pump-temperature": "string",
            "output-power-c-band": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
            "actual-gain": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
            "output-power-total": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
            "output-power-l-band": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
            "msa-output-power-c-band": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
            "input-power-total": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
          },
          config: {
            "target-gain": "string",
            "target-output-power": "string",
            "amp-mode": "string",
            "target-gain-tilt": "string",
            type: "string",
            "plim-enabled": false,
            enabled: false,
            "gain-range": "string",
            "apr-enabled": false,
            "plim-power": "string",
            name: "string",
            "apr-power": "string",
            autolos: false,
          },
        },
      },

      formRules: this.getRule(),
    };
  },
  methods: {
    getRule() {
      return {
        "system.hostname": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.hostname"),
          trigger: "blur",
        },
        "system.contact": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.contact"),
          trigger: "blur",
        },
        "system.location": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.location"),
          trigger: "blur",
        },
      };
    },
    submitQueryForm() {
      this.doQuery(getOA, this.name);
    },
    submitApplyForm() {
      // 获取到组件中的form
      console.log("refs=", this.$refs);
      const form = this.$refs.form;
      console.log(form);
      this.doApply(form, setOA, this.name);
    },
    getFormPromise(form) {
      return new Promise((resolve) => {
        form.validate((res) => {
          resolve(res);
        });
      });
    },
    doQuery(func, name) {
      (async () => {
        let results = await func(name);
        console.log("results=", results);
        console.log(this.form);
        this.$refs.form.resetFields();
        this.form = results.data;
        console.log("amplifier=", this.form.amplifier);
        console.log(
          "amplifier.state=",
          this.form.amplifier.state
        );
      })();
    },
    doApply(form, func, name) {
      this.getFormPromise(form).then(
        (res) => {
          console.log("res=", res);
          const validateResult = res;
          if (validateResult) {
            alert("submit!");
            console.log("表单校验通过");
            (async () => {
              let results = await func(this.form, name);
              console.log("submit SysConfig Setting results=", results);
            })();
          }
        },
        () => {
          console.log("error submit!!,表单未校验通过");
        }
      );
    },
  },
  computed: {},
  created() {},
  mounted() {
    console.log("OaCfgForm mounted, route=", this.$route);
    console.log("name=", this.name);
    // this.doQuery(getOA,this.name);
    this.submitQueryForm();
  },
};
</script>

<style lang="scss" scoped>
.form {
  text-align: left;
  .el-form-item {
    margin-top: 1px;
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
