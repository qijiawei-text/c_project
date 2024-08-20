<template>
  <div class="main1">
    <!-- <h1>Slots Information</h1> -->
   <el-card>
    <div class="index-content">Inventory</div>
    <el-form
      ref="form"
      :model="nodes['org-openroadm-device'].info"
      :rules="formRules"
      label-width="160px"
      label-position="left"
    >
      <!-- <el-form-item :label="$t( 'form.node-type' )"> -->
      <!-- <el-form-item :label="getlabel('node-type')"> -->
      <!-- <el-form-item :label="generateForm('node-type')">         -->
      <el-form-item label="node-type">
        <el-input
          v-model="nodes['org-openroadm-device'].info['node-id']"
          placeholder=""
          readonly
          :disabled="edit"
        ></el-input>
      </el-form-item>
      <el-form-item label="node-type">
        <el-input
          v-model="nodes['org-openroadm-device'].info['node-type']"
          placeholder=""
          readonly
          :disabled="edit"
        ></el-input>
      </el-form-item>
      <el-form-item label="clli">
        <el-input
          v-model="nodes['org-openroadm-device'].info['clli']"
          placeholder=""
          readonly
          :disabled="edit"
        ></el-input>
      </el-form-item>
      <el-form-item label="vendor">
        <el-input
          v-model="nodes['org-openroadm-device'].info['vendor']"
          placeholder=""
          readonly
          :disabled="edit"
        ></el-input>
      </el-form-item>

      <el-form-item label="model">
        <el-input
          v-model="nodes['org-openroadm-device'].info['model']"
          placeholder=""
          readonly
          :disabled="edit"
        ></el-input>
      </el-form-item>
      <el-form-item label="softwareVersion">
        <el-input
          v-model="nodes['org-openroadm-device'].info['softwareVersion']"
          placeholder=""
          readonly
          :disabled="edit"
        ></el-input>
      </el-form-item>
      <el-form-item label="software-build">
        <el-input
          v-model="nodes['org-openroadm-device'].info['software-build']"
          placeholder=""
          readonly
          :disabled="edit"
        ></el-input>
      </el-form-item>
      <el-form-item label="hardware-version">
        <el-input
          v-model="nodes['org-openroadm-device'].shelves[0]['hardware-version']"
          placeholder=""
          readonly
          :disabled="edit"
        ></el-input>
      </el-form-item>
      <el-form-item label="serial-id">
        <el-input
          v-model="nodes['org-openroadm-device'].shelves[0]['serial-id']"
          placeholder=""
          readonly
          :disabled="edit"
        ></el-input>
      </el-form-item>
      <el-form-item label="product-code">
        <el-input
          v-model="nodes['org-openroadm-device'].shelves[0]['product-code']"
          placeholder=""
          readonly
          :disabled="edit"
        ></el-input>
      </el-form-item>
      <el-form-item label="manufacture-date">
        <el-input
          v-model="nodes['org-openroadm-device'].shelves[0]['manufacture-date']"
          placeholder=""
          readonly
          :disabled="edit"
        ></el-input>
      </el-form-item>
      <el-form-item label="temperature">
        <el-input
          v-model="nodes['org-openroadm-device'].shelves[0]['temperature']"
          placeholder=""
          readonly
          :disabled="edit"
        ><i slot="suffix" style="font-style:normal;margin-right: 10px;">℃</i></el-input>
      </el-form-item>

      <!-- <div class="index-content">State</div>
      <template v-for="(node, index) in nodes['org-openroadm-device'].info">
        <el-form-item
          :label="`${index}`"
          v-if="
            typeof nodes['org-openroadm-device'].info[index] == 'string' ||
              typeof nodes['org-openroadm-device'].info[index] == 'number'
          "
          :key="index"
        >
          <el-input
            v-model="nodes['org-openroadm-device'].info[index]"
            placeholder
            :disabled="edit"
          ></el-input>
        </el-form-item>
        <el-form-item
          :label="`${index}`"
          v-if="typeof nodes['org-openroadm-device'].info[index] == 'boolean'"
          :key="index"
        >
          <el-switch
            v-model="nodes['org-openroadm-device'].info[index]"
            :disabled="edit"
          ></el-switch>
        </el-form-item>
      </template>
      <template
        v-for="(node, index) in nodes['org-openroadm-device'].shelves[0]"
      >
        <el-form-item
          :label="`${index}`"
          v-if="
            typeof nodes['org-openroadm-device'].shelves[0][index] ==
              'string' ||
              typeof nodes['org-openroadm-device'].shelves[0][index] == 'number'
          "
          :key="index"
        >
          <el-input
            v-model="nodes['org-openroadm-device'].shelves[0][index]"
            placeholder
            :disabled="edit"
          ></el-input>
        </el-form-item>
        <el-form-item
          :label="`${index}`"
          v-if="
            typeof nodes['org-openroadm-device'].shelves[0][index] == 'boolean'
          "
          :key="index"
        >
          <el-switch
            v-model="nodes['org-openroadm-device'].shelves[0][index]"
            :disabled="edit"
          ></el-switch>
        </el-form-item>
      </template> -->
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
  },
  computed: {
    ...mapGetters({
      nodes: "nodes",
    }),
  },
  mounted() {
    //console.log("mount SlotsConfiguration");
  },
};
</script>

<style lang="scss" scoped>
.main1 {
  text-align: left;
}

</style>
