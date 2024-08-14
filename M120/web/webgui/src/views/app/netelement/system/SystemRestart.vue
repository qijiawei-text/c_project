<template>
  <div class="app">
    <!-- <h1>SystemRestart.vue</h1> -->

    <div class="forms-container">
      <!-- RestartFormComp.vue -->
      <restart-form ref="formComp" />
    </div>

    <!-- <div class="query">
      <el-button
        type="primary"
        @click="submitQueryForm"
        style=" background-color:#79CB79"
        >Query</el-button
      > -->
    <el-dialog title="Restart System..." :visible.sync="dialogTableVisible" :close-on-click-modal=false :close-on-press-escape=false :before-close="handleClose">
          <el-progress :percentage="percentage" :color="customColorMethod"></el-progress>
    </el-dialog>

      <el-button @click="submitApplyForm" style=" background-color:#FBAE42"
        >Apply</el-button
      >
    </div>
  </div>
</template>

<script>
import RestartForm from '@/views/app/netelement/system/components/RestartForm'
import { setRestart, setShutdown } from "@/api/sysRestart";
export default {
  components: {
    "restart-form": RestartForm,
  },
  data() {
    return {
      timer:null,
      percentage: 0,
      dialogTableVisible: false
    }
  },
  methods: {
    submitApplyForm() {
      // 获取到组件中的form
      console.log("refs=", this.$refs);
      const form = this.$refs.formComp.$refs.form;
      console.log(form);
      console.log(this.$refs.formComp.form);
      
      let type = this.$refs.formComp.form.type.toLowerCase()
      switch(type){
        case 'restart':
            console.log('restart');
            this.doApply(form,setRestart)
            break
        case 'shutdown':
            console.log('shutdown');
            this.doApply(form,setShutdown)
            break
        default:
            break
      }

      //this.doApply(form,setSystemConfig)
    },
    getFormPromise(form) {
      return new Promise((resolve) => {
        form.validate((res) => {
          resolve(res);
        });
      });
    }, 
    doApply(form,func) {
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
              this.showprogress()
            })();
          }
        },
        () => {
          console.log("error submit!!,表单未校验通过");
        }
      );
    },
    customColorMethod(percentage) {
      if (percentage < 30) {
        return '#909399';
      } else if (percentage < 70) {
        return '#e6a23c';
      } else {
        return '#67c23a';
      }
    },
    showprogress(){
      this.percentage = 0
      this.dialogTableVisible = true
       this.timer = setInterval(()=>{
        this.percentage += 10;
        console.log("percentage=",this.percentage);
        if (this.percentage > 99) {
          console.log("percentage = 100..............");
          this.percentage = 0
          clearInterval(this.timer);   
          this.dialogTableVisible = false;
        }
      },1000)
    },
    handleClose(done) {
      this.$confirm('确认关闭？')
      .then(_ => {
        done();
      })
      .catch(_ => {});
    }
  },
  mounted() {
   
  },
  updated() {},
};
</script>

<style lang="scss" scoped>
.query {
  margin-top: 20px;
}
</style>
