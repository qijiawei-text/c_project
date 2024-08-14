<template>
  <div class="form">
    <!-- <h1>ocm cfg</h1> -->
    <el-card>
      <div class="index-content">Config</div>
      <el-table
        stripe
        :data="config"
        highlight-current-row
        @selection-change="selsChange"
        style="width: 100%;"
      >
        <!-- <el-table-column type="selection" width="55"> </el-table-column> -->
        <el-table-column type="index" label="Port ID" width="80">
        </el-table-column>

        <el-table-column prop="config.name" label="Name" width="180">
        </el-table-column>

        <!-- <el-table-column prop="config.monitor-port" label="monitor-port" width="160"> </el-table-column> -->
        <el-table-column
          prop="config.monitor-port"
          label="Monitor-Port"
          width="340"
        >
          <template slot-scope="scope">
            <el-input
              v-model="scope.row.config['monitor-port']"
              placeholder="monitor-port"
              @focus="$event.target.select()"
            ></el-input>
          </template>
        </el-table-column>

        <el-table-column label="Remark" width="180"> </el-table-column>

        <el-table-column label="Action" width="120">
          <template slot-scope="scope">
            <el-button
              size="small"
              @click="submitApplyForm(scope.$index, scope.row)"
              style=" background-color:#FBAE42"
              >Apply</el-button
            >
          </template>
        </el-table-column>
      </el-table>
    </el-card>
  </div>
</template>

<script>
import { getOcmConfig, setOcmConfig } from "@/api/ocm";
export default {
  props: {
    config: {
      required: true,
    },
  },
  data() {
    return {
      current: "",
    };
  },
  methods: {
    submitQueryForm(index) {
      this.doQuery(index, getOcmConfig, this.current);
    },
    submitApplyForm(index, data) {
      console.log("setValue,data=", data);
      this.current = data.config.name;
      this.doApply(index, data, setOcmConfig, this.current);
    },
    doQuery(index, func, name) {
      (async () => {
        let results = await func(name);
        console.log("results=", results);
        // console.log("this.form=",this.form);
        // this.$refs.form.resetFields();
        this.config[index].config = results.data.config;
        console.log("response config=", results.data);
        console.log("this.config=", this.config);
      })();
    },
    doApply(index, form, func, name) {
      alert("submit!");
      console.log("表单校验通过");
      (async () => {
        console.log("form=", form, "name=", name);
        let results = await func(form, name);
        console.log("submit OcmConfig Setting results=", results);
        console.log("will OcmConfig get ...");
        this.submitQueryForm(index);
      })();
    },

    selsChange() {},
  },
  mounted() {
    console.log("OcmCfgForm mounted,config=", this.config);
  },
};
</script>

<style lang="scss" scoped></style>
