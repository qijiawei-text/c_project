<template>
  <div class="app">
    <!-- <h1>SystemIPConfig.vue</h1> -->
    <div class="forms-container">
      <ipconfig-form ref="formComp" />
    </div>

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
import IPConfigForm from "@/views/app/netelement/system/components/IPConfigForm";
import { getIPConfig, setIPConfig } from "@/api/ipConfig";
export default {
  components: {
    "ipconfig-form": IPConfigForm,
  },
  methods: {
    submitQueryForm() {
      this.doQuery(getIPConfig);
    },
    submitApplyForm() {
      // 获取到组件中的form
      console.log("refs=", this.$refs);
      const form = this.$refs.formComp.$refs.form;
      console.log(form);
      this.doApply(form, setIPConfig);
    },
    getFormPromise(form) {
      return new Promise((resolve) => {
        form.validate((res) => {
          resolve(res);
        });
      });
    },
    doQuery(func) {
      (async () => {
        let results = await func();
        console.log("results=", results);
        console.log(this.$refs.formComp.form);
        this.$refs.formComp.$refs.form.resetFields();
        this.$refs.formComp.form = results.data;
      })();
    },
    doApply(form, func) {
      this.getFormPromise(form).then(
        (res) => {
          console.log("res=", res);
          const validateResult = res;
          if (validateResult) {
            alert("submit!");
            console.log("表单校验通过");
            (async () => {
              let results = await func(this.$refs.formComp.form);
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
  mounted() {
    this.doQuery(getIPConfig);
  },
  updated() {},
};
</script>

<style lang="scss" scoped>
.query {
  margin-top: 20px;
}
</style>
