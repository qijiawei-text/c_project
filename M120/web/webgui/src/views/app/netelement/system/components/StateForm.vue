<template>
  <div class="state-form">
    <!-- <h2>StateForm.vue</h2> -->
    <el-card>
      <div class="index-content">State</div>
      <el-form
        ref="form"
        :model="form"
        :rules="formRules"
        label-width="160px"
        label-position="left"
      >
        <template v-if="form['system-state'].platform">
          <!-- <el-form-item label="os-version"> -->
          <!-- <el-form-item :label="$t( 'form.os-version' )"> -->
          <!-- <el-form-item :label="getlabel('os-version')"> -->
          <el-form-item :label="generateForm('os-version')">
            <el-input
              v-model="form['system-state'].platform['os-version']"
              placeholder=""
              readonly
              :disabled="edit"
            ></el-input>
          </el-form-item>
          <!-- <el-form-item label="os-name"> -->
          <el-form-item :label="generateForm('os-name')">
            <el-input
              v-model="form['system-state'].platform['os-name']"
              placeholder=""
              readonly
              :disabled="edit"
            ></el-input>
          </el-form-item>
          <!-- <el-form-item label="os-release"> -->
          <el-form-item :label="generateForm('os-release')">
            <el-input
              v-model="form['system-state'].platform['os-release']"
              placeholder=""
              readonly
              :disabled="edit"
            ></el-input>
          </el-form-item>
          <!-- <el-form-item label="machine"> -->
          <el-form-item :label="generateForm('machine')">
            <el-input
              v-model="form['system-state'].platform.machine"
              placeholder=""
              readonly
              :disabled="edit"
            ></el-input>
          </el-form-item>
        </template>
        <!-- <el-form-item label="boot-datetime"> -->
        <el-form-item :label="generateForm('boot-datetime')">
          <el-input
            v-model="form['system-state'].clock['boot-datetime']"
            placeholder=""
            readonly
            :disabled="edit"
          ></el-input>
        </el-form-item>
        <!-- <el-form-item label="current-datetime"> -->
        <el-form-item :label="generateForm('current-datetime')">
          <el-input
            v-model="form['system-state'].clock['current-datetime']"
            placeholder=""
            readonly
            :disabled="edit"
          ></el-input>
        </el-form-item>
      </el-form>
    </el-card>
  </div>
</template>

<script>
import { notEmpty, validateName } from "@/utils/validateform";
import { generateForm } from "@/utils/i18n";
class StateForm {
  constructor() {}

  static getRule() {
    return {};
  }
}

export default {
  data() {
    return {
      edit: true,
      form: {
        "system-state": {
          platform: {
            "os-version": "string",
            "os-name": "string",
            "os-release": "string",
            machine: "string",
          },
          clock: {
            "boot-datetime": "string",
            "current-datetime": "string",
          },
        },
      },

      formRules: this.getRule(),
    };
  },
  methods: {
    generateForm,
    getRule() {
      return {
        name: [{ validator: validateName, trigger: "blur" }],

        "os-version": {
          validator: validateName,
          required: true,
          message: this.$t("valid.port"),
          trigger: "blur",
        },
        "os-name": {
          validator: validateName,
          required: true,
          message: this.$t("valid.port"),
          trigger: "blur",
        },
        "os-release": {
          validator: validateName,
          required: true,
          message: this.$t("valid.port"),
          trigger: "blur",
        },
        machine: {
          validator: validateName,
          required: true,
          message: this.$t("valid.port"),
          trigger: "blur",
        },
        "boot-datetime": {
          validator: validateName,
          required: true,
          message: this.$t("valid.port"),
          trigger: "blur",
        },
        "current-datetime": {
          validator: validateName,
          required: true,
          message: this.$t("valid.port"),
          trigger: "blur",
        },
      };
    },
    getlabel(param) {
      // return this.$t('form.'+param )
      return this.generateForm(param);
    },
  },
  computed: {},
  created() {},
  components: {},
};
</script>

<style lang="scss" scoped>
@import "src/styles/form.scss";
.state-form {
  text-align: left;
}
</style>
