<template>
  <div>
    <div class="index-content">{{ SystemInfomation }}</div>
    <br />

    <el-form
      ref="form"
      :model="system"
      label-width="150px"
      label-position="left"
    >
      <!-- <el-form-item label="System nodename:">
        <el-input v-model="system.nodename" placeholder="This field must be 1 to 31 characters"></el-input>
      </el-form-item>
      <el-form-item label="netmask:">
        <el-input v-model="system.netmask" placeholder="This field must be 1 to 31 characters"></el-input>
      </el-form-item>
      <el-form-item label="gateway:">
        <el-input v-model="system.gateway" placeholder="This field must be 1 to 31 characters"></el-input>
      </el-form-item>
      <el-form-item label="fiber-type">
        <el-input
          v-model="system['fiber-type']"
          placeholder="This field must be 1 to 31 characters"
        ></el-input>
      </el-form-item>-->

      <template v-for="(node, index) in system">
        <el-form-item :label="`${index}`">
          <el-input v-model="system[index]" placeholder></el-input>
        </el-form-item>
      </template>
      <el-button
        type="primary"
        @click="onSubmit"
        style=" background-color:#79CB79"
        >Query</el-button
      >
      <el-button style=" background-color:#FBAE42">Apply</el-button>
    </el-form>
  </div>
</template>

<script>
import uri from "@/const/uri";
import { apiGet } from "@/api/restGet";
export default {
  data() {
    return {
      SystemInfomation: "System Infomation",
      form: {
        name: "",
        region: "",
        date1: "",
        date2: "",
        delivery: false,
        type: [],
        resource: "",
        desc: "",
      },
      system: {
        nodename: "",
        "ip-address": "",
        netmask: "",
        gateway: "",
        "ntp-ip-1": "",
        "ntp-ip-2": "",
        "syslog-ip": "",
        "tacacs-ip": "",
        "tacacs-server-secret": "",
        "fiber-type": "",
        "launching-power-per-channel": 0,
        "remote-ip-address": "",
      },
    };
  },

  methods: {
    async getsysinfo() {
      try {
        let res = await apiGet(uri.GETSYSINFO.type, uri.GETSYSINFO.uri);
        // this.system = JSON.parse(JSON.stringify(res.system));
        this.system = res.data.system;
      } catch (error) {
        console.log(error);
      }
    },
    onSubmit() {
      console.log("submit!");
      console.log(
        "this is rest get,type=",
        uri.GETSYSINFO.type,
        "uri=",
        uri.GETSYSINFO.uri
      );

      this.getsysinfo();
    },
  },
};
</script>

<style lang="scss" scoped>
button {
  background-color: "#79CB79";
}
</style>
