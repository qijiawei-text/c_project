<template>
  <div
    class="login-container pull-height blur"
    @keyup.enter.native="handleLogin"
  >
    <div class="login-border animated fadeInRight">
      <div class="login-main">
        <!-- <h2 class="login-title">登 录</h2> -->

        <x-logo :isCollapse="true"></x-logo>

        <x-lang-select style="width:100px"></x-lang-select>

        <el-tabs v-model="activeName">
          <el-tab-pane label="账号密码" name="user">
            <UserLogin></UserLogin>
          </el-tab-pane>
          <!-- <el-tab-pane label="验证码" name="code">
            <codeLogin></codeLogin>
          </el-tab-pane> -->
          <!-- <el-tab-pane label="OAUTH2" name="oauth2">
            <oauth2Login></oauth2Login>
          </el-tab-pane> -->
        </el-tabs>
      </div>
    </div>
  </div>
</template>
<script>
import UserLogin from "./UserLogin";
// import codeLogin from "./codelogin";
// import oauth2Login from "./oauth2login";
// import { mapGetters } from "vuex";
import XLangSelect from "@/components/XLangSelect";
import XLogo from "@/components/XLogo";
export default {
  name: "login",
  components: {
    UserLogin,
    // codeLogin,
    // oauth2Login
    XLangSelect,
    XLogo,
  },
  data() {
    return {
      activeName: "user",
    };
  },
  // created() {},
  // mounted() {},
  computed: {
    // ...mapGetters(["website"])
  },
  props: [],
  methods: {},
};
</script>

<style lang="scss">
body {
  overflow: hidden;
}
.login-container {
  display: flex;
  align-items: center;
  justify-content: space-around;
  //background: rgba(0, 0, 0, 0.2);
  //position: relative;
  //height: 900px;
  //margin: 100px auto;
  background-size: cover;
}
.login-container::before {
  z-index: -999;
  content: "";
  position: absolute;
  left: 0;
  top: 0;
  width: 100%;
  height: 100%;
  background-image: url("../../assets/images/top_images/login.jpg");
  background-size: cover;
}
.login-info {
  padding-left: 60px;
}
.login-info-title {
  line-height: 90px;
}
.login-info-item {
  font-size: 14px;
}
.login-border {
  display: flex;
  justify-content: center;
  flex-direction: column;
  padding: 30px 50px 25px 50px;
  background-color: #fff;
  border-radius: 6px;
  box-shadow: 1px 1px 2px #eee;
  margin: 10% auto;
}
.login-main {
  border-radius: 3px;
  box-sizing: border-box;
  background-color: #fff;
}
.login-main > h3 {
  margin-bottom: 20px;
}
.login-main > p {
  color: #76838f;
}
.login-title {
  margin: 0 0 20px;
  text-align: center;
  color: #409eff;
  letter-spacing: 3px;
}
.login-submit {
  margin-top: 20px;
  width: 100%;
  border-radius: 28px;
}
.login-form {
  margin: 10px 0;
  .el-form-item__content {
    width: 270px;
  }
  .el-form-item {
    margin-bottom: 12px;
  }
  .el-input {
    input {
      text-indent: 5px;
      border-color: #dcdcdc;
      border-radius: 3px;
    }
    .el-input__prefix {
      i {
        padding: 0 5px;
        font-size: 16px !important;
      }
    }
  }
}
.login-code {
  display: flex;
  align-items: center;
  justify-content: space-around;
  margin: 0 0 0 10px;
}
.login-code-img {
  margin-top: 2px;
  width: 100px;
  height: 32px;
  background-color: #fdfdfd;
  border: 1px solid #f0f0f0;
  color: #333;
  font-size: 14px;
  font-weight: bold;
  letter-spacing: 5px;
  line-height: 32px;
  text-indent: 5px;
  text-align: center;
}
// .blur {
//     -webkit-filter: blur(1px); /* Chrome, Opera */
//        -moz-filter: blur(1px);
//         -ms-filter: blur(1px);
//             filter: blur(1px);
// }
</style>
