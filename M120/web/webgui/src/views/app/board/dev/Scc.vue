<template>
  <div class="mainscc">
    <!-- <h1>Slots Information</h1> -->
    <el-card>
      <div class="index-content">Version</div>
      <el-form
        ref="form"
        :model="slot"
        :rules="formRules"
        label-width="160px"
        label-position="left"
      >
        <el-form-item label="vendor">
          <!-- <el-form-item :label="$t( 'vendor' )"> -->
          <!-- <el-form-item :label="getlabel('node-type')"> -->
          <!-- <el-form-item :label="generateForm('node-type')">         -->
          <el-input
            v-model="scc.vendor"
            placeholder=""
            readonly
            :disabled="edit"
          ></el-input>
        </el-form-item>
        <el-form-item label="model">
          <el-input
            v-model="scc.model"
            placeholder=""
            readonly
            :disabled="edit"
          ></el-input>
        </el-form-item>
        <el-form-item label="serial-id">
          <el-input
            v-model="scc['serial-id']"
            placeholder=""
            readonly
            :disabled="edit"
          ></el-input>
        </el-form-item>
        <el-form-item label="product-code">
          <el-input
            v-model="scc['product-code']"
            placeholder=""
            readonly
            :disabled="edit"
          ></el-input>
        </el-form-item>

        <el-form-item label="manufacture-date">
          <el-input
            v-model="scc['manufacture-date']"
            placeholder=""
            readonly
            :disabled="edit"
          ></el-input>
        </el-form-item>
        <el-form-item label="hardware-version">
          <el-input
            v-model="scc['hardware-version']"
            placeholder=""
            readonly
            :disabled="edit"
          ></el-input>
        </el-form-item>
      </el-form>
    </el-card>
  </div>
</template>

<script>
// import Module from "./ModuleItem";
// import ActiveAlarm from '@/views/app/ActiveAlarm'
import { mapState, mapMutations, mapActions, mapGetters } from "vuex";
import { generateForm } from "@/utils/i18n";
export default {
  components: {},
  data() {
    return {
      edit: true,
      activeName: "first",
      SystemInfomation: " Infomation",
      formRules: this.getRule(),
      circuitPack:{}
    };
  },
  methods: {
    handleClick(tab, event) {
      //console.log(tab, event);
    },
    generateForm,
    getRule() {
      return {};
    },
    getSccData() {
      //return this.nodes["org-openroadm-device"]["circuit-packs"][7];
      let num=-1;
      this.nodes["org-openroadm-device"]["circuit-packs"].some(
        (item,index) => {
          if (item["circuit-pack-name"] == "cp-scc") {
            console.log("find scc");
            num =index
            return true;
          }
        }
      );
      console.log("num=",num);
      return this.nodes["org-openroadm-device"]["circuit-packs"][num];
    },
  },
  computed: {
    ...mapGetters({
      nodes: "nodes",
    }),
    getSccData0() {
      // return this.nodes.some((item)=>{
      //     if(item['circuit-pack-name'] == "cp-scc"){
      //         console.log("find scc");
      //         return item;
      //     }
      // })
      return this.nodes["org-openroadm-device"]["circuit-packs"][7];
    },
  },
  mounted() {
    //console.log("mount Scc");
    this.circuitPack = this.getSccData()
  },
};
</script>

<style lang="scss" scoped>
.mainscc {
  // background-color: green;
  margin-left: 30px;
}
// .index-content{
//     background-color: #D9EDF7;
//     // text-align: left;
// }
</style>
