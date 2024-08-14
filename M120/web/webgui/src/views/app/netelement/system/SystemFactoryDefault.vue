<template>
  <div class="form">
    <!-- <h2>FactoryDefaultForm.vue</h2> -->
    <el-card style="margin:10px 0 10px 0">
      <div class="index-content">State</div>
      <el-alert
        title="Warning!"
        description="System Configuation will Reset or Restore..."
        type="warning"
        effect="dark"
        :closable="false"
        show-icon
        style="margin:16px 0 16px 0"
      >
      </el-alert>
      <div class="setdefault">
        <el-col :span="20">
          <el-form
            ref="form"
            :model="form"
            :rules="formRules"
            label-width="160px"
            label-position="left"
          >
            <el-form-item label="Set Default">
              <el-select
                v-model="form.input['restore-type']"
                placeholder="请选择..."
                filterable
                style="width:80%"
              >
                <template v-for="(node, index) in types">
                  <el-option
                    :label="node"
                    :value="node"
                    :key="index + ''"
                  ></el-option>
                </template>
              </el-select>
            </el-form-item>
          </el-form>
        </el-col>
        <el-col :span="4">
          <div>
            <el-button
              @click="submitSetDefault"
              style=" background-color:#FBAE42"
              >Apply</el-button
            >
          </div>
        </el-col>
      </div>
    </el-card>
    <el-card>
      <div class="SetConf">
        <el-col :span="20">
          <!-- <el-form-item label="Set Conf"> </el-form-item> -->
          <span style="margin-left:10px">SetConf</span>
        </el-col>

        <el-col :span="4">
          <div class="confbutton">
            <el-button @click="submitSetConf" style=" background-color:#FBAE42"
              >Apply</el-button
            >
          </div>
        </el-col>
      </div>
    </el-card>
  </div>
</template>

<script>
import { setDefault, setSetConf } from "@/api/rpc";
export default {
  data() {
    return {
      types: ["all", "optical"],
      form: {
        input: {
          "restore-type": "all",
        },
      },
    };
  },
  methods: {
    submitSetDefault() {
      this.doApply(setDefault, this.form);
    },
    submitSetConf() {},
    getFormPromise(form) {
      return new Promise((resolve) => {
        form.validate((res) => {
          resolve(res);
        });
      });
    },
    doApply(form, func) {
      this.getFormPromise(form).then(
        (res) => {
          console.log("res=", res);
          const validateResult = res;
          if (validateResult) {
            alert("submit!");
            console.log("表单校验通过");
            (async () => {
              let results = await func(this.$refs.formComp.form);
              console.log("submit SysConfig Setting results=", results);
              this.showprogress();
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
.setdefault {
  text-align: left;
}
.SetConf {
  text-align: left;
  height: 48px;
  line-height: 48px;
}
</style>
