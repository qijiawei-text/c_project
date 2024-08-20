<template>
  <div class="form">
    <!-- <h2>IPConfigForm.vue</h2> -->
    <el-form
      ref="form"
      :model="form"
      :rules="formRules"
      label-width="180px"
      label-position="left"
    >
      <div
        v-for="(node, index) in form['ip-interfaces'].interface"
        :key="node.name"
      >
        <!-- <el-card> -->
        <!-- <span>{{node.name}}</span>   -->
        <el-card>
          <div class="index-content">{{ node.name }}</div>
          <el-form-item label="enabled">
            <el-select
              v-model="node.enabled"
              placeholder="..."
              filterable
              style="width:100%"
            >
              <template v-for="(node1, index1) in enabled">
                <el-option
                  :label="node1.label"
                  :value="node1.value"
                  :key="index1 + ''"
                ></el-option>
              </template>
            </el-select>
          </el-form-item>
          <el-form-item
            label="name"
            :prop="'ip-interfaces.interface.' + index + '.name'"
          >
            <el-input v-model="node.name" placeholder=""></el-input>
          </el-form-item>
          <el-form-item
            label="type"
            :prop="'ip-interfaces.interface.' + index + '.type'"
          >
            <el-input v-model="node.type" placeholder=""></el-input>
          </el-form-item>
          <el-form-item
            label="macAddress"
            :prop="'ip-interfaces.interface.' + index + '.macAddress'"
          >
            <el-input
              v-model="node.macAddress"
              placeholder=""
              readonly
              :disabled="edit"
            ></el-input>
          </el-form-item>
        </el-card>

        <template v-if="typeflag(index)">
          <!-- ipv4 -->
          <el-card>
            <!-- <div slot="header"><span>卡片名称</span></div> -->
            <span>IPV4</span>
            <el-form-item
              label="current-ipAddress"
              :prop="
                'ip-interfaces.interface.' + index + '.ipv4.current-ipAddress'
              "
            >
              <el-input
                v-model="node.ipv4['current-ipAddress']"
                placeholder=""
                :disabled="edit"
              ></el-input>
            </el-form-item>
            <el-form-item
              label="current-netmask"
              :prop="
                'ip-interfaces.interface.' + index + '.ipv4.current-netmask'
              "
            >
              <el-input
                v-model="node.ipv4['current-netmask']"
                placeholder=""
                readonly
                :disabled="edit"
              ></el-input>
            </el-form-item>
            <el-form-item
              label="current-defaultGateway"
              :prop="
                'ip-interfaces.interface.' +
                  index +
                  '.ipv4.current-defaultGateway'
              "
            >
              <el-input
                v-model="node.ipv4['current-defaultGateway']"
                placeholder=""
                readonly
                :disabled="edit"
              ></el-input>
            </el-form-item>

            <el-form-item label="origin">
              <el-select
                v-model="node.ipv4.origin"
                placeholder="..."
                filterable
                style="width:100%"
              >
                <template v-for="(node2, index2) in origin">
                  <el-option
                    :label="node2"
                    :value="node2"
                    :key="index2 + ''"
                  ></el-option>
                </template>
              </el-select>
            </el-form-item>

            <template v-if="originstaticflag(index)">
              <el-form-item
                label="ipAddress"
                :prop="'ip-interfaces.interface.' + index + '.ipv4.ipAddress'"
                :rules="formRules.ipAddress"
                :inline-message="true"
              >
                <el-input
                  v-model="node.ipv4.ipAddress"
                  placeholder=""
                ></el-input>
              </el-form-item>

              <el-form-item
                label="netmask"
                :prop="'ip-interfaces.interface.' + index + '.ipv4.netmask'"
                :rules="formRules.netmask"
                :inline-message="true"
              >
                <el-input v-model="node.ipv4.netmask" placeholder=""></el-input>
              </el-form-item>

              <el-form-item
                label="defaultGateway"
                :prop="
                  'ip-interfaces.interface.' + index + '.ipv4.defaultGateway'
                "
                :rules="formRules.defaultGateway"
                :inline-message="true"
              >
                <el-input
                  v-model="node.ipv4.defaultGateway"
                  placeholder=""
                ></el-input>
              </el-form-item>

              <template v-if="node.ipv4['dhcp-server']">
                <el-form-item
                  label="lease-time"
                  :prop="
                    'ip-interfaces.interface.' +
                      index +
                      '.ipv4.dhcp-server.lease-time'
                  "
                >
                  <el-input
                    v-model="node.ipv4['dhcp-server']['lease-time']"
                    placeholder=""
                  ></el-input>
                </el-form-item>

                <el-form-item
                  label="pool-start-ip"
                  :prop="
                    'ip-interfaces.interface.' +
                      index +
                      '.ipv4.dhcp-server.pool-start-ip'
                  "
                >
                  <el-input
                    v-model="node.ipv4['dhcp-server']['pool-start-ip']"
                    placeholder=""
                  ></el-input>
                </el-form-item>

                <el-form-item
                  label="pool-end-ip"
                  :prop="
                    'ip-interfaces.interface.' +
                      index +
                      '.ipv4.dhcp-server.pool-end-ip'
                  "
                >
                  <el-input
                    v-model="node.ipv4['dhcp-server']['pool-end-ip']"
                    placeholder=""
                  ></el-input>
                </el-form-item>
              </template>
            </template>

            <template v-if="origindhcpflag(index)">
              <template v-if="node.ipv4['dhcp-client']">
                <el-form-item
                  label="client-id"
                  :prop="
                    'ip-interfaces.interface.' +
                      index +
                      '.ipv4.dhcp-client.client-id'
                  "
                >
                  <el-input
                    v-model="node.ipv4['dhcp-client']['client-id']"
                    placeholder=""
                  ></el-input>
                </el-form-item>

                <el-form-item
                  label="timeout"
                  :prop="
                    'ip-interfaces.interface.' +
                      index +
                      '.ipv4.dhcp-client.timeout'
                  "
                >
                  <el-input
                    v-model="node.ipv4['dhcp-client'].timeout"
                    placeholder=""
                  ></el-input>
                </el-form-item>

                <el-form-item
                  label="lease"
                  :prop="
                    'ip-interfaces.interface.' +
                      index +
                      '.ipv4.dhcp-client.lease'
                  "
                >
                  <el-input
                    v-model="node.ipv4['dhcp-client'].lease"
                    placeholder=""
                  ></el-input>
                </el-form-item>
              </template>
            </template>
          </el-card>

          <!-- ipv6 -->
          <el-card>
            <!-- <div slot="header"><span>卡片名称</span></div> -->
            <span>IPV6</span>
            <el-form-item
              label="current-ipAddressV6"
              :prop="
                'ip-interfaces.interface.' + index + '.ipv6.current-ipAddressV6'
              "
            >
              <el-input
                v-model="node.ipv6['current-ipAddressV6']"
                placeholder=""
                :disabled="edit"
              ></el-input>
            </el-form-item>
            <el-form-item
              label="current-prefix-length"
              :prop="
                'ip-interfaces.interface.' +
                  index +
                  '.ipv6.current-prefix-length'
              "
            >
              <el-input
                v-model="node.ipv6['current-prefix-length']"
                placeholder=""
                readonly
                :disabled="edit"
              ></el-input>
            </el-form-item>
            <el-form-item
              label="current-defaultGatewayV6"
              :prop="
                'ip-interfaces.interface.' +
                  index +
                  '.ipv6.current-defaultGatewayV6'
              "
            >
              <el-input
                v-model="node.ipv6['current-defaultGatewayV6']"
                placeholder=""
                readonly
                :disabled="edit"
              ></el-input>
            </el-form-item>

            <el-form-item label="originV6">
              <el-select
                v-model="node.ipv6.originV6"
                placeholder="..."
                filterable
                style="width:100%"
              >
                <template v-for="(node3, index3) in origin">
                  <el-option
                    :label="node3"
                    :value="node3"
                    :key="index3 + ''"
                  ></el-option>
                </template>
              </el-select>
            </el-form-item>

            <template v-if="originv6staticflag(index)">
              <el-form-item
                label="ipAddressV6"
                :prop="'ip-interfaces.interface.' + index + '.ipv6.ipAddressV6'"
              >
                <el-input
                  v-model="node.ipv6.ipAddressV6"
                  placeholder=""
                ></el-input>
              </el-form-item>

              <el-form-item
                label="prefix-length"
                :prop="
                  'ip-interfaces.interface.' + index + '.ipv6.prefix-length'
                "
              >
                <el-input
                  v-model="node.ipv6['prefix-length']"
                  placeholder=""
                ></el-input>
              </el-form-item>

              <el-form-item
                label="defaultGatewayV6"
                :prop="
                  'ip-interfaces.interface.' + index + '.ipv6.defaultGatewayV6'
                "
              >
                <el-input
                  v-model="node.ipv6.defaultGatewayV6"
                  placeholder=""
                ></el-input>
              </el-form-item>

              <!-- <el-form-item label="lease-time" :prop="'ip-interfaces.interface.'+ index + '.ipv6.dhcp-serverV6.lease-time'" >
                                <el-input v-model="node.ipv6['dhcp-serverV6']['lease-time']" placeholder=""></el-input>
                            </el-form-item>

                            <el-form-item label="pool-start-ip" :prop="'ip-interfaces.interface.'+ index + '.ipv6.dhcp-serverV6.pool-start-ip'" >
                                <el-input v-model="node.ipv6['dhcp-serverV6']['pool-start-ip']" placeholder=""></el-input>
                            </el-form-item>

                            <el-form-item label="pool-end-ip" :prop="'ip-interfaces.interface.'+ index + '.ipv6.dhcp-serverV6.pool-end-ip'" >
                                <el-input v-model="node.ipv6['dhcp-serverV6']['pool-end-ip']" placeholder=""></el-input>
                            </el-form-item>    -->
            </template>

            <template v-if="originv6dhcpflag(index)">
              <!-- <el-form-item label="client-id" :prop="'ip-interfaces.interface.'+ index + '.ipv6.dhcp-clientV6.client-id'" >
                                <el-input v-model="node.ipv6['dhcp-clientV6']['client-id']" placeholder=""></el-input>
                            </el-form-item>

                            <el-form-item label="timeout" :prop="'ip-interfaces.interface.'+ index + '.ipv6.dhcp-clientV6.timeout'" >
                                <el-input v-model="node.ipv6['dhcp-clientV6'].timeout" placeholder=""></el-input>
                            </el-form-item>

                            <el-form-item label="lease" :prop="'ip-interfaces.interface.'+ index + '.ipv6.dhcp-clientV6.lease'" >
                                <el-input v-model="node.ipv6['dhcp-clientV6'].lease" placeholder=""></el-input>
                            </el-form-item>       -->
            </template>
          </el-card>
        </template>
        <!-- </el-card> -->
      </div>
    </el-form>
  </div>
</template>

<script>
import { notEmpty, validateName, validateIp } from "@/utils/validateform";

class IPConfigForm {
  constructor() {}

  static getRule() {
    return {};
  }
}

export default {
  data() {
    return {
      edit: true,
      enabled: [
        {
          label: "false",
          value: false,
        },
        {
          label: "true",
          value: true,
        },
      ],
      origin: ["other", "static", "dhcp", "link-layer", "random"],
      form: {
        "ip-interfaces": {
          interface: [
            {
              macAddress: "string",
              ipv4: {
                "current-defaultGateway": "string",
                "current-ipAddress": "string",
                netmask: "string",
                "dhcp-client": {
                  lease: 0,
                  "client-id": "string",
                  timeout: 0,
                },
                origin: "other",
                ipAddress: "string",
                "current-netmask": "string",
                defaultGateway: "string",
                "dhcp-server": {
                  "pool-start-ip": "string",
                  "lease-time": 0,
                  "pool-end-ip": "string",
                },
              },
              ipv6: {
                "prefix-length": 0,
                "current-prefix-length": 0,
                originV6: "other",
                defaultGatewayV6: "string",
                "current-ipAddressV6": "string",
                "dhcp-serverV6": {
                  "pool-start-ip": "string",
                  "lease-time": 0,
                  "pool-end-ip": "string",
                },
                "current-defaultGatewayV6": "string",
                ipAddressV6: "string",
                "dhcp-clientV6": {
                  lease: 0,
                  "client-id": "string",
                  timeout: 0,
                },
              },
              name: "string",
              type: "ip",
              enabled: false,
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
        ipAddress: {
          validator: validateIp,
          required: true,
          message: this.$t("invalid.ip"),
          trigger: "blur",
        },
        netmask: {
          validator: validateIp,
          required: true,
          message: this.$t("invalid.netmask"),
          trigger: "blur",
        },
        defaultGateway: {
          validator: validateIp,
          required: true,
          message: this.$t("invalid.defaultgw"),
          trigger: "blur",
        },
      };
    },
  },
  computed: {
    originstaticflag() {
      return function(index) {
        return this.form["ip-interfaces"].interface[index].ipv4.origin ==
          "static"
          ? true
          : false;
      };
    },
    origindhcpflag() {
      return function(index) {
        return this.form["ip-interfaces"].interface[index].ipv4.origin == "dhcp"
          ? true
          : false;
      };
    },
    originv6staticflag() {
      return function(index) {
        return this.form["ip-interfaces"].interface[index].ipv6.originV6 ==
          "static"
          ? true
          : false;
      };
    },
    originv6dhcpflag() {
      return function(index) {
        return this.form["ip-interfaces"].interface[index].ipv6.originV6 ==
          "dhcp"
          ? true
          : false;
      };
    },
    typeflag() {
      return function(index) {
        return this.form["ip-interfaces"].interface[index].type == "ip"
          ? true
          : false;
      };
    },
  },

  created() {},
};
</script>

<style lang="scss" scoped>
.form {
  text-align: left;
  .el-form-item {
    margin-top: 8px;
    margin-bottom: 8px;
    padding: 0;
    margin: 2px;
  }
}
</style>
