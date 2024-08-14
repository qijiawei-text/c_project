<template>
  <div class="form">
    <!-- <h1>waveplan</h1> -->
    <el-form ref="form" :model="form" label-width="240px" label-position="left">
      <el-card>
        <div class="index-content">wavelengths</div>

        <el-table
          stripe
          border
          style="margin-top: 10px;"
          :data="form['wavelengths']"
        >
          <el-table-column
            v-for="(item, key, index) in form['wavelengths'][0]"
            :label="key"
            :key="index"
            align="center"
          >
            <template slot-scope="scope">
              {{ scope.row[key] }}
            </template>
          </el-table-column>
        </el-table>
      </el-card>
    </el-form>
    <div class="query">
      <el-button
        type="primary"
        @click="submitQueryForm"
        style=" background-color:#79CB79"
        >Query</el-button
      >
      <!-- <el-button @click="submitApplyForm" style=" background-color:#FBAE42"
        >Apply</el-button
      > -->
    </div>
  </div>
</template>

<script>
import { getOpticalModules } from "@/api/opticalModule";
export default {
  props: {},
  data() {
    return {
      edit: true,
      form: {},
      options: ["customized", "short", "medium", "long", "all"],
    };
  },
  computed: {},
  methods: {
    submitQueryForm() {
      this.doQuery(getOpticalModules);
    },
    doQuery(func) {
      (async () => {
        let results = await func();
        console.log("results=", results);
        // console.log("this.form=",this.form);
        // this.$refs.form.resetFields();
        this.form = results.data["optical-modules"]["default-waveplan"];
        console.log("response =", results.data);
        console.log("this.form=", this.form);
      })();
    },
  },
  mounted() {
    console.log("WaveplanForm mounted,state=", this.form);
    this.submitQueryForm();
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
