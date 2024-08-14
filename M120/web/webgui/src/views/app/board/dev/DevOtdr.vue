<template>
  <div>
    <!-- <h1> DEV_OCM</h1> -->
    <!-- <el-button type="primary" @click="submitQueryForm">otdr get</el-button> -->
    <el-tabs v-model="activeName" @tab-click="handleClick" class="tab">
      <el-tab-pane label="config" name="first">
        <otdr-cfg-form :config="form['optical-otdr']"></otdr-cfg-form>
      </el-tab-pane>
      <el-tab-pane label="state" name="second">
        <otdr-state-form
          :config="form['optical-otdr']"
          @ParentsubmitQueryForm="submitQueryForm"
        ></otdr-state-form>
      </el-tab-pane>
      <template v-for="(node, index) in form['optical-otdr']['otdr-port']">
        <el-tab-pane
          :label="node['port-name']"
          :name="'' + index"
          :key="node['port-name']"
        >
          <otdr-port-form
            :otdrport="node"
            @ParentsubmitQueryForm="submitQueryForm"
          >
          </otdr-port-form>
        </el-tab-pane>
      </template>
    </el-tabs>
  </div>
</template>

<script>
import { getOtdr, getOtdrPort } from "@/api/otdr";
import OtdrCfgForm from "@/views/app/board/dev/compenents/OtdrCfgForm";
import OtdrStateForm from "@/views/app/board/dev/compenents/OtdrStateForm";
import OtdrPortForm from "@/views/app/board/dev/compenents/OtdrPortForm";
export default {
  components: {
    OtdrCfgForm,
    OtdrStateForm,
    OtdrPortForm,
  },
  data() {
    return {
      activeName: "first",
      form: {
        "optical-otdr": {
          "otdr-port": [
            {
              state: {
                loss: "string",
                "fiber-length": 0,
                reflectance: "string",
                "completion-time": "string",
                "event-count": 0,
                "otdr-event": [
                  {
                    loss: "string",
                    "fiber-length": 0,
                    reflectance: "string",
                    "total-loss": "string",
                    "event-id": 0,
                    "event-type": "non-reflective",
                  },
                ],
                "port-name": "string",
                "otdr-mode": "measuring",
              },
              config: {
                "launch-cable-length": 0,
                "port-name": "string",
              },
              "port-name": "string",
              name: "dfdfd",
            },
          ],
          state: {
            "otdr-mode": "measuring",
          },
          "pre-config": [
            {
              "pulse-width": 0,
              "sample-resolution": "string",
              "measuring-time": 0,
              "user-ior": "string",
              "distance-range": 0,
              "scan-type": "customized",
              "reflectance-thr": "string",
              "fiber-end-thr": "string",
              "splice-loss-thr": "string",
            },
          ],
          config: {
            "pulse-width": 0,
            "sample-resolution": "string",
            "measuring-time": 0,
            "user-ior": "string",
            "distance-range": 0,
            "scan-type": "customized",
            "reflectance-thr": "string",
            "fiber-end-thr": "string",
            "splice-loss-thr": "string",
            "event-trigger": false,
            interval: 0,
          },
        },
      },
      config: [],
    };
  },
  methods: {
    submitQueryForm() {
      this.doQuery(getOtdr);
    },
    doQuery(func) {
      (async () => {
        let results = await func();
        console.log("results=", results);
        console.log("this.form=", this.form);
        // this.$refs.form.resetFields();
        this.form["optical-otdr"] = results.data["optical-otdr"];
        console.log(
          "ddddddddddddddddddddd, otdrall=",
          this.form["optical-otdr"]["otdr-port"]
        );
      })();
    },
    doQueryPort(index, func, name) {
      (async () => {
        let results = await func(name);
        console.log("index=", index);
        console.log("results=", results);
        this.form["optical-otdr"]["otdr-port"][index] = results.data;
        console.log("comall=", this.form["optical-otdr"]["otdr-port"]);
      })();
    },

    handleClick(tab, event) {
      // console.log(tab, event);
      console.log("tab.name=", tab.name);
      console.log("tab.label=", tab.label);
      if (tab.label != "config") {
        console.log("parseInt(tab.name)=", parseInt(tab.name));
        // this.doQueryPort(parseInt(tab.name),getOtdrPort,tab.label)
      }
      // console.log("event=",event);
    },
  },
  created() {
    console.log("otdr created....");
    // this.submitQueryForm();
  },
  mounted() {
    this.submitQueryForm();
  },
};
</script>

<style lang="scss" scoped></style>
