<template>
  <div>
    <el-form ref="form" :model="form" label-width="150px" label-position="left" size="mini">
      <div class="index-content">{{lable1}}</div>
      <el-form-item label="Description:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="Manufactory:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>

      <div class="index-content">{{lable2}}</div>
      <el-form-item label="EDFA-DS:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="EDFA-US:">
        <el-input v-model="form.name" placeholder width="100px"></el-input>
      </el-form-item>
      <el-form-item label="EDFA-DS Status:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="EDFA-US Status:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>

      <el-button type="primary" @click="onSubmit" style=" background-color:#79CB79">Query</el-button>
      <el-button style=" background-color:#FBAE42">Apply</el-button>
    </el-form>
  </div>
</template>

<script>
export default {
  data() {
    return {
      lable1: "Module Information",
      lable2: "EDFA Information",
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

.el-button {
  margin-top: 10px;
}
</style>