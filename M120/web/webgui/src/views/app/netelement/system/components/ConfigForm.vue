<template>
  <div class="form">
    <!-- <h2>ConfigForm.vue</h2> -->
    <el-card>
       <div class="index-content">Config</div>
      <el-form
        ref="form"
        :model="form"
        :rules="formRules"
        label-width="160px"
        label-position="left"
      >
         
        <!-- <el-form-item label="hostname" prop="system.hostname"> -->
        <el-form-item :label="generateForm('hostname')" prop="system.hostname">     
          <el-input v-model="form.system.hostname" placeholder=""></el-input>
        </el-form-item>
        <!-- <el-form-item label="contact" prop="system.contact"> -->
        <el-form-item :label="generateForm('contact')" prop="system.contact">   
          <el-input v-model="form.system.contact" placeholder=""></el-input>
        </el-form-item>
        <!-- <el-form-item label="location" prop="system.location"> -->
        <el-form-item :label="generateForm('location')" prop="system.location">  
          <el-input v-model="form.system.location" placeholder=""></el-input>
        </el-form-item>
      </el-form>
    </el-card>
  </div>
</template>

<script>
import { notEmpty, validateName } from "@/utils/validateform";
import { generateForm } from "@/utils/i18n";
class ConfigForm {
  constructor() {}

  static getRule() {
    return {};
  }
}

export default {
  data() {
    return {
      form: {
        system: {
          tacacs: {
            options: {
              timeout: 0,
            },
            server: [
              {
                statistics: {
                  "connection-failures": 0,
                  "messages-sent": 0,
                  "connection-timeouts": 0,
                  "connection-opens": 0,
                  "connection-aborts": 0,
                  "errors-received": 0,
                  "connection-closes": 0,
                  "messages-received": 0,
                },
                address: "string",
                "shared-secret": "string",
                port: 0,
                "single-connection": false,
                name: "string",
                "source-ip": "string",
                "server-type": "authentication",
              },
            ],
          },
          hostname: "string",
          contact: "string",
          location: "string",
          ntp: {
            server: [
              {
                name: "string",
                udp: {
                  port: 0,
                  address: "string",
                },
                prefer: false,
                "association-type": "server",
                iburst: false,
              },
            ],
            enabled: true,
          },
          clock: {
            "timezone-utc-offset": 0,
            "timezone-name": "string",
          },
          radius: {
            options: {
              timeout: 0,
              attempts: 0,
            },
            server: [
              {
                "authentication-type": "string",
                name: "string",
                udp: {
                  address: "string",
                  "shared-secret": "string",
                  "authentication-port": 0,
                },
              },
            ],
          },
          "dns-resolver": {
            options: {
              timeout: 0,
              attempts: 0,
            },
            server: [
              {
                name: "string",
                "udp-and-tcp": {
                  port: 0,
                  address: "string",
                },
              },
            ],
            search: ["string"],
          },
          authentication: {
            user: [
              {
                "user-role": 0,
                name: "string",
                password: "string",
                "authorized-key": [
                  {
                    name: "string",
                    "key-data": "string",
                    algorithm: "string",
                  },
                ],
              },
            ],
            "user-authentication-order": ["string"],
          },
        },
      },

      formRules: this.getRule(),
    };
  },
  methods: {
    generateForm,
    getRule() {
      return {
        "system.hostname": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.hostname"),
          trigger: "blur",
        },
        "system.contact": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.contact"),
          trigger: "blur",
        },
        "system.location": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.location"),
          trigger: "blur",
        },
      };
    },
  },
  computed: {},
  created() {},
};
</script>

<style lang="scss" scoped>
.state-form {
  text-align: left;
}
</style>
