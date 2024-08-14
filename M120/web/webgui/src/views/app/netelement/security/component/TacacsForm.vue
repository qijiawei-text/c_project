<template>
  <div class="form">
    <!-- <h2>TacacsForm.vue</h2> -->
    <el-card>
      <div class="index-content">Config</div>
      <el-form
        ref="form"
        :model="form"
        :rules="formRules"
        label-width="160px"
        label-position="left"
      >
        <el-form-item label="timeout">
          <el-input
            v-model="form.tacacs.options.timeout"
            placeholder=""       
          ></el-input>
        </el-form-item>

        <template v-for="(node, index) in form.tacacs.server">

         <el-form-item
            label="address"
            :prop="'tacacs.server.' + index + '.address'"
            :rules="formRules.address"
            :key="index + 0"
          >
            <el-input
              v-model="form.tacacs.server[index].address"
              placeholder="input ip address"
            ></el-input>
          </el-form-item>

          <el-form-item
            label="shared-secret"
            :prop="'tacacs.server.' + index + '.shared-secret'"
            :rules="formRules['shared-secret']"
            :key="index + 1"
          >
            <el-input
              v-model="form.tacacs.server[index]['shared-secret']"
              placeholder="input shared-secret"
            ></el-input>
          </el-form-item>

          <el-form-item
            label="port"
            :prop="'tacacs.server.' + index + '.port'"
            :rules="formRules['port']"
            :key="index + 2"
          >
            <el-input
              v-model="form.tacacs.server[index]['port']"
              placeholder="input port"
            ></el-input>
          </el-form-item>

          <el-form-item
            label="single-connection"
            :prop="'tacacs.server.' + index + '.single-connection'"
            :rules="formRules['single-connection']"
            :key="index + 3"
          >
           <el-switch v-model="form.tacacs.server[index]['single-connection']"></el-switch>
          </el-form-item>

          <el-form-item
            label="name"
            :prop="'tacacs.server.' + index + '.name'"
            :rules="formRules.name"
            :key="index + 4"
          >
            <el-input
              v-model="form.tacacs.server[index].name"
              placeholder=""
            ></el-input>
          </el-form-item>

          <el-form-item
            label="source-ip"
            :prop="'tacacs.server.' + index + '.source-ip'"
            :rules="formRules['source-ip']"
            :key="index + 5"
          >
            <el-input
              v-model="form.tacacs.server[index]['source-ip']"
              placeholder=""
            ></el-input>
          </el-form-item>          

          <el-form-item
            label="server-type"
            :prop="'tacacs.server.' + index + '.server-type'"
            :rules="formRules['server-type']"
            :key="index + 6"
          >
            <el-input
              v-model="form.tacacs.server[index]['server-type']"
              placeholder=""
            ></el-input>
          </el-form-item>   
 
        </template>
      </el-form>
    </el-card>
  </div>
</template>

<script>
import { notEmpty, validateName } from "@/utils/validateform";

class TacacsForm {
  constructor() {}

  static getRule() {
    return {};
  }
}

export default {
  data() {
    return {
      form: {
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
      },

      formRules: this.getRule(),
    };
  },
  methods: {
    getRule() {
      return {
        address: {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.address"),
          trigger: "blur",
        },
        "shared-secret": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.shared-secret"),
          trigger: "blur",
        },
        port: {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.port"),
          trigger: "blur",
        },
        "single-connection": {
          type:'boolean',// validator: notEmpty,
          required: true,
          message: this.$t("invalid.single-connection"),
          trigger: "blur",
        },
        name: {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.name"),
          trigger: "blur",
        },
        "source-ip": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.source-ip"),
          trigger: "blur",
        },
        "server-type": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.server-type"),
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
.form {
  text-align: left;
}
</style>
