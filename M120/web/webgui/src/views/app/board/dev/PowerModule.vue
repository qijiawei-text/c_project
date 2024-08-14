<template>
  <div class="mainpower">
    <!-- <h1>mainpower</h1> -->
    <el-card>
      <div class="index-content">Version</div>
      <el-form
        ref="form"
        :model="form"
        :rules="formRules"
        label-width="160px"
        label-position="left"
      >
        <el-form-item label="vendor">
          <!-- <el-form-item :label="$t( 'vendor' )"> -->
          <!-- <el-form-item :label="getlabel('node-type')"> -->
          <!-- <el-form-item :label="generateForm('node-type')">         -->
          <el-input
            v-model="form.vendor"
            placeholder=""
            readonly
            :disabled="edit"
          ></el-input>
        </el-form-item>
        <el-form-item label="model">
          <el-input
            v-model="form.model"
            placeholder=""
            readonly
            :disabled="edit"
          ></el-input>
        </el-form-item>
        <el-form-item label="serial-id">
          <el-input
            v-model="form['serial-id']"
            placeholder=""
            readonly
            :disabled="edit"
          ></el-input>
        </el-form-item>
        <el-form-item label="product-code">
          <el-input
            v-model="form['product-code']"
            placeholder=""
            readonly
            :disabled="edit"
          ></el-input>
        </el-form-item>

        <el-form-item label="manufacture-date">
          <el-input
            v-model="form['manufacture-date']"
            placeholder=""
            readonly
            :disabled="edit"
          ></el-input>
        </el-form-item>
        <el-form-item label="hardware-version">
          <el-input
            v-model="form['hardware-version']"
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
      name: this.$route.name,
      activeName: "first",
      SystemInfomation: " Infomation",
      form: {},
      formRules: this.getRule(),
    };
  },
  methods: {
    handleClick(tab, event) {
      //console.log(tab, event);
    },
    checkNum() {
      console.log("checkNum!!!!!!!!!!!!!!!!!!,this.name=", this.name);
      this.nodes["org-openroadm-device"]["circuit-packs"].some((item) => {
        if (this.name == item["circuit-pack-name"]) {
          console.log("-------finded ", item["circuit-pack-name"]);
          this.form = item;
          console.log("form=", this.form);
          return;
        }
      });
    },
    generateForm,
    getRule() {
      return {};
    },
  },
  computed: {
    ...mapGetters({
      nodes: "nodes",
    }),
  },
  watch: {
    $route: function() {
      console.log("current router is", this.$route.name);
      this.name = this.$route.name;
      this.checkNum();
    },
  },
  mounted() {
    //console.log("mount PowerModule");
  },
};
</script>

<style lang="scss" scoped>
.mainpower {

}

</style>
