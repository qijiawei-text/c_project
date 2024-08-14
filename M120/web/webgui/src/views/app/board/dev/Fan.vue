<template>
  <div>
    <!-- <h1> {{fanNum}} </h1> -->
    <!-- <oa-form :name="name" ref="formComp"></oa-form> -->
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
  </div>
</template>

<script>
import { mapState, mapMutations, mapActions, mapGetters } from "vuex";
import { generateForm } from "@/utils/i18n";
export default {
  components: {},
  data() {
    return {
      edit: true,
      name: this.$route.name,
      fanNum: "",
      form: {},
      formRules: this.getRule(),
    };
  },
  computed: {
    ...mapGetters({
      nodes: "nodes",
    }),
  },
  mounted() {
    console.log("Fan mouted, route=", this.$route.name);
    this.checkNum();
  },
  methods: {
    checkNum() {
      console.log("checkNum!!!!!!!!!!!!!!!!!!,this.name=", this.name);
      this.nodes["org-openroadm-device"].shelves[0].slots.some((item) => {
        console.log("item['slot-name']=", item["slot-name"]);
        //  if(0 < this.name.indexOf(item['slot-name']) ){
        if (this.name == item["provisioned-circuit-pack"]) {
          this.fanNum = item["provisioned-circuit-pack"];
          console.log("---------break this.fanNum =", this.fanNum);
          return;
        }
      });

      this.nodes["org-openroadm-device"]["circuit-packs"].some((item) => {
        console.log("this.fanNum=", this.fanNum);
        // console.log(item['circuit-pack-name']);
        // console.log("bbbbbb",this.name.indexOf(item['circuit-pack-name']));
        // console.log("cccccc",this.fanNum.indexOf(item['circuit-pack-name']));
        if (this.fanNum == item["circuit-pack-name"]) {
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
  watch: {
    $route: function() {
      console.log("current router is", this.$route.name);
      this.name = this.$route.name;
      this.checkNum();
    },
  },
};
</script>

<style lang="scss" scoped></style>
