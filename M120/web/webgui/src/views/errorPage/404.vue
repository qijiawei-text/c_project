<template>
  <div class="errPage-container">
    <el-row>
      <el-col :span="24">
        <h1 class="text-jumbo">Oops!</h1>
      </el-col>
      <el-col :span="24">
        <el-button @click="back" icon="arrow-left">Back</el-button>
        <el-button @click="gohome" icon="arrow-left">Home</el-button>
      </el-col>
    </el-row>
  </div>
</template>

<script>
export default {
  name: "page401",
  data() {
    return {};
  },
  methods: {
    back() {
      if (this.$route.query.noGoBack) {
        this.$router.push({ path: "/" });
      } else {
        this.$router.go(-1);
      }
    },
    gohome() {
      this.$router.push({ path: "/" });
    },
  },
};
</script>

<style rel="stylesheet/scss" lang="scss" scoped>
.errPage-container {
  width: 800px;
  margin: 100px auto;
  .text-jumbo {
    font-size: 60px;
    font-weight: 700;
    color: #484848;
    margin-bottom: 24px;
  }
}
</style>
