<template>
  <div class="clock-form">
    <!-- <h2>ClockForm.vue</h2> -->

    <el-card>
      <div class="index-content">C</div>
      <el-form
        ref="ClockForm"
        :model="ClockForm"
        :rules="ClockFormRules"
        label-width="160px"
        label-position="left"
      >
        <!-- <el-form-item label="Timezone" class="Timezone">
          <el-radio-group v-model="resource">
            <el-radio label="timezone-name"></el-radio>
            <el-radio label="timezone-utc-offset"></el-radio>
          </el-radio-group>
        </el-form-item>

        <el-form-item
          label="TimezoneName"
          v-if="tzflag == 'timezone-name'"
          class="TimezoneName"
        >
          <el-select
            v-model="ClockForm.clock['timezone-name']"
            placeholder="请选择时区"
            filterable
          >
            <template v-for="(node, index) in timezone">
              <el-option
                :label="node"
                :value="node"
                :key="index + ''"
              ></el-option>
            </template>
          </el-select>
        </el-form-item> -->

        <el-form-item
          label="timezone utc offset"
          v-if="tzflag == 'timezone-utc-offset'"
          class="TimezoneOffset"
        >
          <el-select
            v-model="ClockForm.clock['timezone-utc-offset']"
            placeholder="请选择时区偏移"
            filterable
          >
            <template v-for="(node, index) in utc_offset">
              <el-option
                :label="node"
                :value="node"
                :key="index + ''"
              ></el-option>
            </template>
          </el-select>
        </el-form-item>
        <!-- </div> -->

        <el-form-item label="Date&Time" v-if="showtime">
          <el-date-picker
            type="date"
            placeholder="选择日期"
            v-model="date1"
            style="width: 100%;"
            value-format="yyyy-MM-dd"
          ></el-date-picker>

          <el-time-picker
            placeholder="选择时间"
            v-model="time1"
            style="width: 100%;"
            value-format="HH:mm:ss"
          ></el-time-picker>
        </el-form-item>
      </el-form>
    </el-card>
  </div>
</template>

<script>
import { notEmpty, validateName } from "@/utils/validateform";
var moment = require("moment-timezone");
class ClockForm {
  constructor() {
    //this.name = '';
    //this.position = null;
  }

  static getRule() {
    return {};
  }
}

export default {
  data() {
    return {
      showtime: true,
      time1: "",
      date1: "",
      timezone: [],
      utc_offset: [],
      //ClockForm: new ClockForm(),
      resource: "timezone-utc-offset",
      ClockForm: {
        clock: {
          "timezone-utc-offset": 0,
          "timezone-name": "string",
        },
      },

      ClockFormRules: ClockForm.getRule(),
    };
  },
  methods: {
    utc_init() {
      for (let i = 0; i <= 24 * 2; i++) {
        this.utc_offset.push((i - 24) * 60);
      }
      console.log("this.utc_offset=", this.utc_offset);
    },
  },
  computed: {
    tzflag() {
      return this.resource;
    },
    CurrentDatetime() {
      return {
        input: {
          "current-datetime": this.date1 + " " + this.time1,
        },
      };
    },
  },
  created() {
    console.log("created @@@@@@@@@@@@12");
    this.timezone = moment.tz.names();
    console.log("timezone=", this.timezone);

    this.utc_init();
  },
};
</script>

<style lang="scss" scoped>
.clock-form {
  text-align: left;
  .el-form-item {
    margin-top: 0px;
    margin-bottom: 0px;
    padding: 0;
    margin: 0;
  }
  .Timezone {
  }
  .TimezoneOffset {
  }
  .TimezoneName {
  }
}
</style>
