<template>
  <div class="form">
    <!-- <h1>osc state</h1> -->
    <template v-if="form">
      <el-form
        ref="form"
        :model="form"
        label-width="240px"
        label-position="left"
      >
        <el-card>
          <div class="index-content">State</div>
          <el-form-item label="tx-power" prop="tx-power">
            <el-input
              v-model="form['tx-power']"
              placeholder=""
              readonly
            ></el-input>
          </el-form-item>

          <el-form-item label="rx-power" prop="rx-power">
            <el-input
              v-model="form['rx-power']"
              placeholder=""
              readonly
            ></el-input>
          </el-form-item>

          <el-form-item label="drop-power" prop="drop-power">
            <el-input
              v-model="form['drop-power']"
              placeholder=""
              readonly
            ></el-input>
          </el-form-item>
          <el-form-item label="add-power" prop="add-power">
            <el-input
              v-model="form['add-power']"
              placeholder=""
              readonly
            ></el-input>
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

      <!-- <el-button @click="submitApplyForm" style=" background-color:#FBAE42"
        >Apply</el-button
      > -->
    </div>
  </div>
</template>

<script>
import { getOsc } from "@/api/otdr";
export default {
  props: {
    config: {
      required: true,
    },
  },
  data() {
    return {
      edit: true,
      //  form: this.config,
    };
  },
  computed: {
    form() {
      return this.config;
    },
  },
  methods: {
    submitQueryForm() {
      this.$emit("ParentsubmitQueryForm");
    },
  },
  mounted() {
    console.log("OtdrStategForm mounted,state=", this.config);
    console.log("OtdrStategForm mounted,form=", this.form);
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
