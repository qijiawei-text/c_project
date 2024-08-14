<template>
  <div>
    <el-form ref="form" :model="form" label-width="150px" label-position="left" size="mini">
      <div class="index-content">{{lable1}}</div>
      <el-form-item label="TYPE:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="Serial Number:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="Part Number:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="Manufactured Date">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>

      <div class="index-content">{{lable2}}</div>
      <el-form-item label="TYPE:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="Serial Number:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="Part Number:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="Manufactured Date">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>

      <div class="index-content">{{lable3}}</div>
      <el-form-item label="Serial Number:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="Part Number:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="Manufactured Date">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>

      <div class="index-content">{{lable4}}</div>
      <el-form-item label="TYPE:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="Serial Number:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="Part Number:">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <el-form-item label="Manufactured Date">
        <el-input v-model="form.name" placeholder></el-input>
      </el-form-item>
      <!-- <el-button type="primary" @click="onSubmit" style=" background-color:#79CB79">Query</el-button>
      <el-button style=" background-color:#FBAE42">Apply</el-button>-->
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
      lable4: "Power Supply Card",
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