<template>
  <div class="app">
    <!-- <h1>SystemState.vue</h1> -->

    <div class="forms-container">
      <!-- StateFormComp.vue -->
      <state-form ref="formComp" />
    </div>

    <div class="query">
      <el-button
        type="primary"
        @click="submitQueryForm"
        style=" background-color:#79CB79"
        >Query</el-button
      >
    </div>
  </div>
</template>

<script>
import StateForm from "@/views/app/netelement/system/components/StateForm";

import { getSystemState } from "@/api/sysState";
export default {
  components: {
    "state-form": StateForm,
  },
  methods: {
    submitQueryForm() {
      this.doQuery(getSystemState);
    },
    doQuery(func) {
      (async () => {
        let results = await func();
        console.log("results=", results);
        console.log(this.$refs.formComp.form);
        this.$refs.formComp.$refs.form.resetFields();
        this.$refs.formComp.form = results.data;
        // this.$refs.formComp.form['system-state'].clock = results.data['system-state'].clock;
      })();
    },
  },
  mounted() {
    this.doQuery(getSystemState);
  },

  updated() {},
};
</script>

<style lang="scss" scoped>
.query {
  margin-top: 20px;
}
</style>
