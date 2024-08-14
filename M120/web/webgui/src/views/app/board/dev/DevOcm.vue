<template>
  <div>
    <!-- <h1> DEV_OCM</h1> -->
    <!-- <el-button type="primary" @click="submitQueryForm">ocmlist get</el-button> -->
    <el-tabs v-model="activeName" @tab-click="handleClick" class="tab">
      <el-tab-pane label="config" name="first">
        <ocm-cfg-form :config="config"></ocm-cfg-form>
      </el-tab-pane>
      <template
        v-for="(node, index) in form['channel-monitors']['channel-monitor']"
      >
        <!-- <el-tab-pane :label="''+index" :name="''+index" :key=node.name> -->
        <el-tab-pane :label="node.name" :name="'' + index" :key="node.name">
          <ocm-state-form :channelmonitor="node"></ocm-state-form>
        </el-tab-pane>
      </template>
    </el-tabs>
  </div>
</template>

<script>
import { getOcmList, getOcmChannels } from "@/api/ocm";
import OcmCfgForm from "@/views/app/board/dev/compenents/OcmCfgForm";
import OcmStateForm from "@/views/app/board/dev/compenents/OcmStateForm";
export default {
  components: {
    OcmCfgForm,
    OcmStateForm,
  },
  data() {
    return {
      activeName: "first",
      form: {
        "channel-monitors": {
          "channel-monitor": [
            // {
            //     "channels": {
            //     "channel": [
            //         {
            //         "state": {
            //             "power": "string",
            //             "upper-frequency": 0,
            //             "lower-frequency": 0
            //         },
            //         "upper-frequency": 0,
            //         "lower-frequency": 0
            //         }
            //     ]
            //     },
            //     "name": "string",
            //     "state": {
            //     "monitor-port": "string",
            //     "name": "string"
            //     },
            //     "config": {
            //     "monitor-port": "string",
            //     "name": "string"
            //     }
            // }
          ],
        },
      },
      config: [],
    };
  },
  methods: {
    submitQueryForm() {
      this.doQuery(getOcmList);
    },
    doQuery(func) {
      (async () => {
        let results = await func();
        console.log("results=", results);
        console.log("this.form=", this.form);
        // this.$refs.form.resetFields();
        this.form["channel-monitors"] = results.data;
        console.log(
          "ocmall=",
          this.form["channel-monitors"]["channel-monitor"]
        );
        this.updateOcmConfig();
      })();
    },
    doQueryChannel(index, func, name) {
      (async () => {
        let results = await func(name);
        console.log("index=", index);
        console.log("results=", results);
        this.form["channel-monitors"]["channel-monitor"][index].channels =
          results.data;
        console.log(
          "ocmall=",
          this.form["channel-monitors"]["channel-monitor"]
        );
      })();
    },
    updateOcmConfig() {
      console.log("this.config0=", this.config);
      this.config.length = 0;
      console.log("this.config1=", this.config);
      console.log(this.form["channel-monitors"]["channel-monitor"]);
      this.form["channel-monitors"]["channel-monitor"].forEach((item) => {
        console.log("item.config", item.config);
        let configItem = {
          config: {
            "monitor-port": "string",
            name: "string",
          },
        };
        configItem.config = item.config;
        this.config.push(configItem);
        // this.config.push(item.config)
      });
      console.log("this.config2=", this.config);
    },
    handleClick(tab, event) {
      // console.log(tab, event);
      console.log("tab.name=", tab.name);
      console.log("tab.label=", tab.label);
      if (tab.label != "config") {
        console.log("parseInt(tab.name)=", parseInt(tab.name));
        this.doQueryChannel(parseInt(tab.name), getOcmChannels, tab.label);
      }
      // console.log("event=",event);
    },
  },
  mounted() {
    this.submitQueryForm();
  },
};
</script>

<style lang="scss" scoped></style>
