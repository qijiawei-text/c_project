<template>
  <div class="form">
    <!-- <h2>ConfigForm.vue</h2> -->
    <el-card style="margin:10px 0 10px 0">
      <div class="index-content">Restart</div>
      <el-alert
        title="Warning!"
        description="System will Restart/Shutdown..."
        type="warning"
        effect="dark"
        :closable="false"
        show-icon
        style="margin:16px 0 16px 0"
      >
      </el-alert>
      <el-form
        ref="form"
        :model="form"
        :rules="formRules"
        label-width="160px"
        label-position="left"
        style="margin:10px 0 10px 0"
      >
        <el-select
          v-model="form.type"
          placeholder="请选择..."
          filterable
          style="width:100%"
        >
          <template v-for="(node, index) in types">
            <el-option
              :label="node"
              :value="node"
              :key="index + ''"
            ></el-option>
          </template>
        </el-select>
      </el-form>
    </el-card>
  </div>
</template>

<script>
import { notEmpty, validateName } from "@/utils/validateform";

class RestartForm {
  constructor() {}

  static getRule() {
    return {};
  }
}

export default {
  data() {
    return {
      types: ["restart", "shutdown"],
      form: {
        type: "restart",
      },

      formRules: this.getRule(),
    };
  },
  methods: {
    getRule() {
      return {};
    },
  },
  computed: {},
  created() {},
};
</script>

<style lang="scss" scoped>
.form {
  text-align: left;
}
</style>
