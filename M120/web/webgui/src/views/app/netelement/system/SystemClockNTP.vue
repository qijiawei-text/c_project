<template>
  <div class="app">
    <!-- <h1>App.vue</h1> -->
    <div class="forms-container">
      <!-- ClockFormComp.vue -->
      <clock-form ref="ClockFormComp" />
      <!-- NtpFormComp.vue -->
      <ntp-form ref="NtpFormComp" />
    </div>

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
import ClockForm from "@/views/app/netelement/system/components/ClockForm";
import NtpForm from "@/views/app/netelement/system/components/NtpForm";
import uri from "@/const/uri";
import {
  getClockNtpInfo,
  setClockNtpInfo,
  setCurrentDateTime,
} from "@/api/sysClockNtp";
export default {
  components: {
    "clock-form": ClockForm,
    "ntp-form": NtpForm,
  },
  methods: {
    submitQueryForm() {
      (async () => {
        let results = await getClockNtpInfo();
        let [data1, data2] = results;

        console.log("submitQueryForm data1=", data1); // 打印data1结果
        console.log("submitQueryForm data2=", data2); // 打印data2结果

        this.$refs.ClockFormComp.ClockForm = data1.data;
        this.$refs.NtpFormComp.NtpForm = data2.data;

        console.log(
          "clockForm=",
          this.$refs.ClockFormComp.ClockForm
        );
        console.log(
          "ntpform=",
          this.$refs.NtpFormComp.NtpForm
        );
      })();
    },

    submitApplyForm() {
      // 获取到组件中的form
      console.log("refs=", this.$refs);
      const ClockForm = this.$refs.ClockFormComp.$refs.ClockForm;
      const NtpForm = this.$refs.NtpFormComp.$refs.NtpForm;
      console.log(ClockForm);
      console.log(NtpForm);
      // 使用Promise.all去校验结果
      Promise.all([ClockForm, NtpForm].map(this.getFormPromise)).then((res) => {
        const validateResult = res.every((item) => !!item);
        if (validateResult) {
          console.log("两个表单都校验通过");
          (async () => {
            console.log(
              "this.$refs.ClockFormComp.CurrentDatetime=",
              this.$refs.ClockFormComp.CurrentDatetime
            );
            let results = await setClockNtpInfo(
              this.$refs.ClockFormComp.ClockForm,
              this.$refs.NtpFormComp.NtpForm,
              this.$refs.ClockFormComp.CurrentDatetime
            );
            let [data1, data2, data3] = results; // 结构赋值 ES6 特性

            console.log("submitQueryForm data1=", data1); // 打印data1结果
            console.log("submitQueryForm data2=", data2); // 打印data2结果
            console.log("submitQueryForm data3=", data3); // 打印data2结果
          })();
        } else {
          console.log("两个表单未校验通过");
        }
      });
    },

    getFormPromise(form) {
      return new Promise((resolve) => {
        form.validate((res) => {
          resolve(res);
        });
      });
    },
  },
};
</script>

<style lang="scss" scoped>
.query {
  margin-top: 20px;
}
</style>
