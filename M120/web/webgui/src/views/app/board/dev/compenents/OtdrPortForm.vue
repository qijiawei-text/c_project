<template>
  <div class="form">
    <!-- <h1>otdr port</h1> -->
    <!-- <h1>{{ otdrport }}</h1> -->
    <el-form
      ref="form"
      :model="form"
      :rules="formRules"
      label-width="240px"
      label-position="left"
    >
      <el-card>
        <div class="index-content">Config</div>
        <el-form-item
          label="launch-cable-length"
          prop="config.launch-cable-length"
          :rules="formRules['launch-cable-length']"
        >
          <el-input
            v-model="form.config['launch-cable-length']"
            placeholder="30"
          ></el-input>
        </el-form-item>
        <el-form-item
          label="port-name"
          prop="config.port-name"
          :rules="formRules['port-name']"
        >
          <el-input
            v-model="form.config['port-name']"
            placeholder=""
          ></el-input>
        </el-form-item>
      </el-card>

      <el-card>
        <div class="index-content">state</div>

        <template v-if="form['state']">
          <template v-for="(node, index) in form['state']">
            <el-form-item
              :label="`${index}`"
              v-if="
                typeof form['state'][index] == 'string' ||
                  typeof form['state'][index] == 'number'
              "
              :key="index+''"
            >
              <el-input
                v-model="form['state'][index]"
                placeholder
                :disabled="edit"
              ></el-input>
            </el-form-item>

            <el-form-item
              :label="`${index}`"
              v-if="typeof form['state'][index] == 'boolean'"
              :key="index+''"
            >
              <el-switch
                v-model="form['state'][index]"
                :disabled="edit"
              ></el-switch>
            </el-form-item>

            <template v-if="typeof form['state'][index] == 'object'">
              <template v-for="(node1, index1) in form['state'][index]">
                <el-form-item
                  :label="`${index}:${index1}`"
                  v-if="
                    typeof form['state'][index][index1] == 'string' ||
                      typeof form['state'][index][index1] == 'number'
                  "
                  :key="index1+''"
                >
                  <el-input
                    v-model="form['state'][index][index1]"
                    placeholder
                    :disabled="edit"
                  ></el-input>
                </el-form-item>
              </template>
            </template>

            <!-- <template v-if="form['state'][index] instanceof Array">
            <el-table border style="margin-top: 10px;" :data="form.state['otdr-event']" :key=index>
                <el-table-column v-for="(item,key,index) in form.state['otdr-event'][0]" :label="key" :key="index" align="center">
                    <template slot-scope="scope">
                        {{scope.row[key]}}
                    </template>
                </el-table-column>
            </el-table>
            </template> -->
          </template>
          <!-- <template v-if="form['state']['otdr-event'] instanceof Array"> -->
          <el-table
            border
            style="margin-top: 10px;"
            :data="form.state['otdr-event']"
            key=""
          >
            <el-table-column
              v-for="(item, key, index) in form.state['otdr-event'][0]"
              :label="key"
              :key="index"
              align="center"
            >
              <template slot-scope="scope">
                {{ scope.row[key] }}
              </template>
            </el-table-column>
          </el-table>
          <!-- </template> -->
        </template>
      </el-card>
    </el-form>
    <div class="query">
      <el-button
        type="primary"
        @click="submitQueryForm"
        style=" background-color:#79CB79"
        >Query</el-button
      >

      <el-button @click="submitApplyForm" style=" background-color:#FBAE42"
        >Apply</el-button
      >
    </div>
  </div>
</template>

<script>
import { notEmpty, validateName } from "@/utils/validateform";
import { getOtdrPort, setOtdrPortCfg } from "@/api/otdr";
export default {
  props: {
    otdrport: {
      required: true,
    },
  },
  data() {
    return {
      edit: true,
      form: this.otdrport,
      formRules:  this.getRule(),
    };
  },
  computed: {
    // form() {
    //   return this.otdrport;
    // },
  },
  methods: {
    getRule() {
      return {
        "port-name": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid['port-name']"),
          trigger: "blur",
        },
        "launch-cable-length": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid['launch-cable-length']"),
          trigger: "blur",
        },
      };
    },
    submitQueryForm() {
      console.log("this.otdrport", this.otdrport);
      console.log("this.form", this.form);
      this.doQuery(getOtdrPort,this.otdrport['port-name']);
    //   this.$emit("ParentsubmitQueryForm");
    },
    submitApplyForm() {
      this.doApply(this.form, setOtdrPortCfg, this.form["port-name"]);
    },
    getFormPromise(form) {
      return new Promise((resolve) => {
        form.validate((res) => {
          resolve(res);
        });
      });
    },    
    doQuery(func, name) {
      (async () => {
        let results = await func(name);
        console.log("results=", results);
        // console.log("this.form=",this.form);
        // this.$refs.form.resetFields();
        // this.otdrport = results.data;
        this.form = results.data["otdr-port"];
        console.log("response =", results.data);
        console.log("this.form1=", this.form);
      })();
    },
    doApply(form, func, name) {
      this.getFormPromise(this.$refs.form).then(
        (res) => {
          console.log("res=", res);
          const validateResult = res;
          if (validateResult) {
            alert("submit!");
            console.log("表单校验通过");
            (async () => {
              let results = await func(form.config, name);
              console.log("submit OtdrPortConfig Setting results=", results);
              console.log("will OtdrPortConfig get ...");
              this.submitQueryForm();
            })();
          }
        },
        () => {
          console.log("error submit!!,表单未校验通过");
        }
      );
    },
  },
};
</script>

<style lang="scss" scoped>
.form {
  margin: 8px;
}
</style>
