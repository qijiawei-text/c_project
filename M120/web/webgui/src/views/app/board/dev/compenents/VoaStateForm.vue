<template>
  <div class="form">
    <!-- <h2>VoaForm.vue</h2> -->
    <el-card>
      <!-- <span>State</span> -->
      <div class="index-content">State</div>
      <el-form
        ref="form"
        :model="form"
        label-width="240px"
        label-position="left"
      >
        <!-- <el-card>
        <div class="index-content">Config</div>
        <el-form-item label="name" prop="attenuator.name">
          <el-input
            v-model="form.attenuator.name"
            placeholder=""
            readonly
          ></el-input>
        </el-form-item>
        <el-form-item
          label="attenuation-mode"
          prop="attenuator.config.attenuation-mode"
        >
          <el-select
            v-model="form.attenuator.config['attenuation-mode']"
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
        <el-form-item label="attenuation" prop="attenuator.config.attenuation">
          <el-input
            v-model="form.attenuator.config['attenuation']"
            placeholder=""
          ></el-input>
        </el-form-item>

        <el-form-item label="enabled">
          <el-switch v-model="form.attenuator.config.enabled"></el-switch>
        </el-form-item>
        <el-form-item
          label="target-output-power"
          prop="attenuator.config.target-output-power"
        >
          <el-input
            v-model="form.attenuator.config['target-output-power']"
            placeholder=""
          ></el-input>
        </el-form-item>
      </el-card> -->

        <template v-if="form.attenuator.state">
          <template
            v-for="(node, index) in form.attenuator.state['actual-attenuation']"
          >
            <el-form-item
              :label="`${index}`"
              v-if="
                typeof form.attenuator.state['actual-attenuation'][index] ==
                  'string' ||
                  typeof form.attenuator.state['actual-attenuation'][index] ==
                    'number'
              "
            >
              <el-input
                v-model="form.attenuator.state['actual-attenuation'][index]"
                placeholder
                :disabled="edit"
              ></el-input>
            </el-form-item>

            <el-form-item
              :label="`${index}`"
              v-if="
                typeof form.attenuator.state['actual-attenuation'][index] ==
                  'boolean'
              "
            >
              <el-switch
                v-model="form.attenuator.state['actual-attenuation'][index]"
                :disabled="edit"
              ></el-switch>
            </el-form-item>

            <template v-if="typeof form.attenuator.state[index] == 'object'">
              <template v-for="(node1, index1) in form.attenuator.state[index]">
                <el-form-item
                  :label="`${index}:${index1}`"
                  v-if="
                    typeof form.attenuator.state[index][index1] == 'string' ||
                      typeof form.attenuator.state[index][index1] == 'number'
                  "
                >
                  <el-input
                    v-model="form.attenuator.state[index][index1]"
                    placeholder
                    :disabled="edit"
                  ></el-input>
                </el-form-item>
              </template>
            </template>
          </template>
        </template>
      </el-form>
    </el-card>
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
import { getVoa, setVoa, setVoaConfig } from "@/api/voa";

export default {
  props: ["name"],
  data() {
    return {
      edit: true,
      form: {
        attenuator: {
          name: "string",
          state: {
            name: "string",
            "attenuation-mode": "string",
            attenuation: "string",
            "target-output-power": "string",
            enabled: false,
            "actual-attenuation": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
            "optical-return-loss": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
            component: "string",
            "output-power-total": {
              "max-time": 0,
              "min-time": 0,
              interval: 0,
              min: "string",
              avg: "string",
              max: "string",
              instant: "string",
            },
            "egress-port": "string",
            "ingress-port": "string",
          },
          config: {
            name: "string",
            "attenuation-mode": "string",
            attenuation: "string",
            "target-output-power": "string",
            enabled: false,
          },
        },
      },

      formRules: this.getRule(),
      options: ["CONSTANT_POWER", "CONSTANT_ATTENUATION"],
    };
  },
  methods: {
    getRule() {
      return {
        "form.attenuator.attenuation": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.hostname"),
          trigger: "blur",
        },
      };
    },
    submitQueryForm() {
      this.doQuery(getVoa, this.name);
    },
    submitApplyForm() {
      // 获取到组件中的form
      console.log("refs=", this.$refs);
      const form = this.$refs.form;
      console.log(form);
      this.doApply(form, setVoaConfig, this.name);
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
        //this.$refs.form.resetFields();
        this.form = results.data;
        console.log("attenuator=", this.form.attenuator);
        console.log("attenuator.state=", this.form.attenuator.state);
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
              let results = await func(this.form.attenuator.config, name);
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
    console.log("VoaStateForm mounted, route=", this.$route);
    console.log("name=", this.name);
    this.doQuery(getVoa, this.name);
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
