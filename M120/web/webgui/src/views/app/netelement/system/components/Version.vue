<template>
  <div>
    <div class="index-content">Version</div>
    <el-form ref="form" :model="form" label-width="150px" label-position="left" size="mini">
      <el-form-item label="Active Release">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="Active Release:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>

      <div class="index-content">{{lable1}}</div>
      <el-form-item label="EDFA SW:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="EDFA FPGA:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="APS SW:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>

      <div class="index-content">{{lable2}}</div>
      <el-form-item label="EDFA SW:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="EDFA FPGA:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="APS SW:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>

      <div class="index-content">{{lable3}}</div>
      <el-form-item label="APP:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="FPGA:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="CPLD">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>

      <el-button type="primary" @click="onSubmit" style=" background-color:#79CB79">Query</el-button>
      <!-- <el-button style=" background-color:#FBAE42">Apply</el-button> -->
    </el-form>
  </div>
</template>

<script>
export default {
  data() {
    return {
      lable1: "OCML Card1",
      lable2: "OCML Card2",
      lable3: "Control Module Card",

      form: {
        name: "",
        region: "",
        date1: "",
        date2: "",
        delivery: false,
        type: [],
        resource: "",
        desc: ""
      }
    };
  },
  methods: {
    onSubmit() {
      //console.log("submit!");
    }
  }
};
</script>

<style lang="scss" scoped>
button {
  background-color: "#79CB79";
}

.el-form-item {
  margin-top: 0px;
  margin-bottom: -2px;
  padding: 0;
  margin: 0;
}
</style>