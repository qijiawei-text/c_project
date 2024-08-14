<template>
  <div class="ntp-form">
    <!-- <h2>NtpForm.vue</h2> -->
    <el-card>
      <div class="index-content">NTP</div>
      <el-form
        :model="NtpForm"
        ref="NtpForm"
        :rules="NtpFormRules"
        label-width="160px"
        label-position="left"
      >
        <div class="ntp">
          <el-form-item label="NTPEnable">
            <el-switch v-model="NtpForm.ntp.enabled"></el-switch>
          </el-form-item>

          <div v-if="ntpflag">
            <el-form-item label="NTPServer">
              <el-col :span="2">
                <input
                  placeholder="name"
                  label="name"
                  readonly
                  style="border:0px"
                />
              </el-col>
              <el-col :span="12">
                <input
                  placeholder="address"
                  label="address"
                  readonly
                  style="border:0px"
                />
              </el-col>
              <el-col :span="2">
                <input
                  placeholder="port"
                  label="port"
                  readonly
                  style="border:0px"
                />
              </el-col>
              <el-col :span="4">
                <input
                  placeholder="association-type"
                  label="association-type"
                  readonly
                  style="border:0px"
                />
              </el-col>
              <el-col :span="2">
                <input
                  placeholder="iburst"
                  label="iburst"
                  readonly
                  style="border:0px"
                />
              </el-col>
              <el-col :span="2">
                <input
                  placeholder="prefer"
                  label="prefer"
                  readonly
                  style="border:0px"
                />
              </el-col>
            </el-form-item>

            <template v-for="(node, index) in NtpForm.ntp.server">
              <el-form-item
                :label="index + 1 + ''"
                :key="index"
                :prop="'ntp.server.' + index + '.name'"
              >
                <el-row>
                  <el-col :span="2">
                    <el-input
                      v-model="NtpForm.ntp.server[index].name"
                      placeholder="name"
                      label="name"
                    ></el-input>
                  </el-col>
                  <el-col :span="12">
                    <el-form-item
                      :prop="'ntp.server.' + index + '.udp.address'"
                      :rules="NtpFormRules.address"
                    >
                      <el-input
                        v-model="NtpForm.ntp.server[index].udp.address"
                        placeholder="input ip address"
                      ></el-input>
                    </el-form-item>
                  </el-col>

                  <el-col :span="2">
                    <el-form-item
                      :prop="'ntp.server.' + index + '.udp.port'"
                      :rules="NtpFormRules.port"
                      :key="index"
                    >
                      <el-input
                        v-model="NtpForm.ntp.server[index].udp.port"
                        placeholder=""
                      ></el-input>
                    </el-form-item>
                  </el-col>

                  <el-col :span="4">
                    <el-select
                      v-model="NtpForm.ntp.server[index]['association-type']"
                      placeholder=""
                      filterable
                    >
                      <el-option
                        label="server"
                        value="server"
                        default
                      ></el-option>
                      <el-option label="pool" value="pool"></el-option>
                      <el-option label="peer" value="peer"></el-option>
                    </el-select>
                  </el-col>

                  <el-col :span="2">
                    <el-switch
                      v-model="NtpForm.ntp.server[index].prefer"
                    ></el-switch>
                  </el-col>
                  <el-col :span="2">
                    <el-switch
                      v-model="NtpForm.ntp.server[index].iburst"
                    ></el-switch>
                  </el-col>
                </el-row>
              </el-form-item>
            </template>
          </div>
        </div>
      </el-form>
    </el-card>
  </div>
</template>

<script>
import {
  validateName,
  validateAge,
  validateSex,
  validateIp,
  validatePort,
} from "@/utils/validateform";
import { generateTitle, generateInvalid } from "@/utils/i18n";
// NtpForm的类
class NtpForm {
  constructor() {}

  static getRule() {
    return {};
  }
}

export default {
  data() {
    return {
      //  NtpForm: new NtpForm(),
      NtpForm: {
        ntp: {
          server: [
            {
              name: "111",
              udp: {
                port: 123,
                address: "127.0.0.1",
              },
              prefer: false,
              "association-type": "server",
              iburst: false,
            },
            {
              name: "222",
              udp: {
                port: 123,
                address: "192.168.1.123",
              },
              prefer: false,
              "association-type": "peer",
              iburst: false,
            },
          ],
          enabled: true,
        },
      },
      NtpFormRules: this.getRule(),
    };
  },
  computed: {
    ntpflag() {
      return this.NtpForm.ntp.enabled;
    },
  },
  methods: {
    generateTitle,
    generateInvalid,
    getRule() {
      return {
        name: [{ validator: validateName, trigger: "blur" }],

        // rules: {
        port: {
          validator: validatePort,
          required: true,
          message: this.$t("invalid.port"),
          trigger: "blur",
        },
        //address:{ validator: validateIp, required: true, message: this.$t( 'valid.ip' ), trigger: "blur"},
        address: {
          validator: validateIp,
          required: true,
          message: this.generateInvalid("ip"),
          trigger: "blur",
        },
        "ntp.enabled": {
          type: "string",
          required: true,
          message: "请填写",
          trigger: "blur",
        },
        "ntp.server.0.name": {
          type: "string",
          required: true,
          message: "请填写名字",
          trigger: "blur",
        },
        "ntp.server.1.name": {
          type: "string",
          required: true,
          message: "请填写名字",
          trigger: "blur",
        },
        // "ntp.server.0.udp.address": [
        //   {

        //     type: "string",
        //     required: true,
        //     message: "请填写ip地址",
        //     trigger: "blur",
        //   },
        //   { min: 3, max: 5, message: "长度在 3 到 5 个字符", trigger: "blur" },
        // ],
        // "ntp.server.0.udp.port": {
        //   type: "string",
        //   required: true,
        //   message: "请填写port地址",
        //   trigger: "blur"
        // },
        // "ntp.server.1.udp.address": [{
        //   validator: validateIp,
        //   message: "ip地址格式错误",
        //   trigger: "blur",
        // }]
        // "ntp.server.1.udp.port": {
        //   type: "string",
        //   required: true,
        //   message: "请填写port地址",
        //   trigger: "blur"
        // }
      };
    },
  },
};
</script>

<style lang="scss" scoped>
.ntp-form {
  text-align: left;
  margin-top: 0px;

  .el-form-item {
    margin-top: 0px;
    margin-bottom: 0px;
    padding: 0;
    margin: 0;
  }
  .ntp {
    margin-right: 4px;
    margin-bottom: 20px;
  }
}
</style>
