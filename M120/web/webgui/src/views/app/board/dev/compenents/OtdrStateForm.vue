<template>
  <div class="form">
    <!-- <h1>otdr state</h1> -->
    <el-form ref="form" :model="form" label-width="240px" label-position="left">
      <el-card>
        <div class="index-content">State</div>
        <el-form-item label="otdr-mode" prop="state.otdr-mode">
          <el-input v-model="form.state['otdr-mode']" placeholder=""></el-input>
        </el-form-item>
      </el-card>

      <el-card>
        <div class="index-content">pre-config</div>

        <el-table border style="margin-top: 10px;" :data="form['pre-config']">
          <el-table-column
            v-for="(item, key, index) in form['pre-config'][0]"
            :label="key"
            :key="index"
            align="center"
          >
            <template slot-scope="scope">
              {{ scope.row[key] }}
            </template>
          </el-table-column>
        </el-table>

        <!-- <template v-if="form['pre-config']">
          <template
            v-for="(node, index) in form['pre-config']"
          >
            <el-form-item
              :label="`${index}`"
              v-if="
                typeof form['pre-config'][index] ==
                  'string' ||
                  typeof form['pre-config'][index] ==
                    'number'
              "
            >
              <el-input
                v-model="form['pre-config'][index]"
                placeholder
                :disabled="edit"
              ></el-input>
            </el-form-item>

            <el-form-item
              :label="`${index}`"
              v-if="
                typeof form['pre-config'][index] ==
                  'boolean'
              "
            >
              <el-switch
                v-model="form['pre-config'][index]"
                :disabled="edit"
              ></el-switch>
            </el-form-item>

            <template v-if="typeof form['pre-config'][index] == 'object'">
              <template v-for="(node1, index1) in form['pre-config'][index]">
                <el-form-item
                  :label="`${index}:${index1}`"
                  v-if="
                    typeof form['pre-config'][index][index1] == 'string' ||
                      typeof form['pre-config'][index][index1] == 'number'
                  "
                >
                  <el-input
                    v-model="form['pre-config'][index][index1]"
                    placeholder
                    :disabled="edit"
                  ></el-input>
                </el-form-item>
              </template>
            </template>
          </template>
        </template> -->
      </el-card>
    </el-form>
    <div class="query">
      <el-button
        type="primary"
        @click="submitQueryForm"
        style=" background-color:#79CB79"
        >Query</el-button
      >

      <!-- <el-button @click="submitApplyForm" style=" background-color:#FBAE42"
        >Apply</el-button
      > -->
    </div>
  </div>
</template>

<script>
import { getOtdrCfg, setOtdrCfg, getOtdr } from "@/api/otdr";
export default {
  props: {
    config: {
      required: true,
    },
  },
  data() {
    return {
      edit: true,
      //   form: this.config,
      options: ["customized", "short", "medium", "long", "all"],
    };
  },
  computed: {
    form() {
      return this.config;
    },
  },
  methods: {
    submitQueryForm() {
      this.$emit("ParentsubmitQueryForm");
    },
  },
  mounted() {
    console.log("OtdrStategForm mounted,state=", this.config);
  },
};
</script>

<style lang="scss" scoped>
.form {
  text-align: left;

  .el-form-item {
    margin-top: 4px;
    margin-bottom: 1px;
    padding: 0;
    margin: 1px;
  }
}
.query {
  margin-top: 20px;
  text-align: center;
}

</style>
