<template>
  <div>
    <el-col :span="24" class="header">
      <el-col :span="4" class="logo">
        <span>OPLINK</span>
      </el-col>
      <el-col :span="12" style="min-height:1px">
        <span></span>
      </el-col>
      <el-col :span="3" style="min-height:1px">
        <i class="el-icon-message-solid" @click="showActiveAlarm"></i>
        <span></span>
      </el-col>
      <el-col :span="1" style="min-height:1px">
        <x-lang-select style="width:80px"></x-lang-select>
      </el-col>
      <el-col :span="4" style="min-height:1px">
        <el-dropdown trigger="click">
          <i class="el-icon-s-custom" style="color:#ffffff"></i>

          <el-dropdown-menu slot="dropdown">
            <el-dropdown-item>
              <span @click="logout" style="display:block;">{{
                generateTitle("LogOut")
              }}</span>
            </el-dropdown-item>
          </el-dropdown-menu>
        </el-dropdown>
        <i>{{ name }}</i>
        <span></span>
      </el-col>
    </el-col>

    <el-dialog
      title="Active Alarm"
      :visible.sync="dialogTableVisible"
      width="80%"
    >
      <active-alarm></active-alarm>
    </el-dialog>
  </div>
</template>

<script>
import XLangSelect from "@/components/XLangSelect";
import { generateTitle } from "@/utils/i18n";
import ActiveAlarm from "@/views/app/alarm/ActiveAlarm";
export default {
  data() {
    return {
      total: "",
      name: "admin",
      dialogTableVisible: false,
    };
  },
  methods: {
    logout() {
      this.$store.dispatch("LogOut").then(() => {
        location.reload(); 
        this.$router.push("/");
      });
    },
    generateTitle,
    selsChange() {},
    handleCurrentChange() {},
    showActiveAlarm() {
      // this.dialogTableVisible= !this.dialogTableVisible
      this.dialogTableVisible = true;
    },
  },
  components: {
    XLangSelect,
    ActiveAlarm,
  },
};
</script>

<style lang="scss" scoped>
.header {
  display: flex;
  align-items: center;
  height: 48px;
  line-height: 48px;
  background: #20a0ff;
  color: #ffffff;
  // border: 2px solid orange ;
  .userinfo {
    text-align: right;
    padding-right: 0px;
    .userinfo-inner {
      color: #ffffff;
      cursor: pointer;
      img {
        width: 40px;
        height: 40px;
        border-radius: 20px;
        margin: 10px 0px 10px 10px;
        float: right;
      }
    }
  }
  .logo {
    font-size: 22px;
    img {
      width: 40px;
      float: left;
      // margin: 10px 10px 10px 18px;
    }
    .txt {
      color: #20a0ff;
    }
  }
}
</style>
