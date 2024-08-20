<template>
  <div class="alarmcontent">
    <!-- <el-dialog
      title="Active Alarm"
      :visible.sync="showDialog"
       width="80%"
    > -->
    <el-table
      height="360"
      :data="activeAlarms"
      highlight-current-row
      @selection-change="selsChange"
      style="width: 100%"
      :cell-class-name="tableCellClassName"
      :row-class-name="tableRowClassName"
    >
      <!-- <el-table-column type="selection" width="55"></el-table-column> -->
      <el-table-column type="index" width="60"></el-table-column>
      <el-table-column prop="id" label="id" width="150"></el-table-column>
      <el-table-column prop="type" label="type" width="140"></el-table-column>
      <el-table-column
        prop="raiseTime"
        label="raiseTime"
        width="190"
      ></el-table-column>
      <el-table-column
        prop="probableCause.cause"
        label="cause"
        width="160"
      ></el-table-column>
      <el-table-column
        prop="resource.resource.circuit-pack-name"
        label="resource"
        width="120"
      ></el-table-column>
      <el-table-column
        prop="severity"
        label="severity"
        width="140"
      ></el-table-column>
    </el-table>

    <!--工具条-->
    <el-col :span="24" class="toolbar">
      <!-- <el-button type="danger" @click="batchRemove" :disabled="this.sels.length===0">批量删除</el-button> -->
      <el-pagination
        layout="prev, pager, next"
        @current-change="handleCurrentChange"
        :page-size="10"
        :total="total"
        style="float:right;"
      ></el-pagination>
    </el-col>
    <!-- </el-dialog> -->
  </div>
</template>

<script>
import { getActiveAlarm } from "@/api/activeAlarm";
export default {
  props: {
    dialogTableVisible: {
      type: Boolean,
      default: false,
    },
  },
  computed: {
    showDialog: {
      get: function() {
        return this.dialogTableVisible;
      },
      set: function(val) {
        this.dialogTableVisible = val;
      },
    },
  },
  data() {
    return {
      total: 10,
      name: "admin",
      // "active-alarm-list": {
      activeAlarms: [
        {
          severity: "critical",
          "circuit-id": "string",
          probableCause: {
            cause: "vendorExtension",
            extension: "string",
            location: "NEND",
            direction: "tx",
          },
          resource: {
            resource: {
              "node-id": "string",
              "connection-name": "string",
              "internal-link-name": "string",
              "interface-name": "string",
              "versioned-service-name": "string",
              "odu-sncp-pg-name": "string",
              "circuit-pack-name": "string",
              "degree-number": 0,
              "srg-number": 0,
              "shelf-name": "string",
              "service-name": "string",
              "common-id": "string",
              "physical-link-name": "string",
              port: {
                "circuit-pack-name": "string",
                "port-name": "string",
              },
              "other-resource-id": "string",
              "xpdr-number": 0,
              "version-number": 0,
              "amp-number": 0,
            },
            device: {
              "node-id": "string",
            },
            resourceType: {
              type: "other",
              extension: "string",
            },
          },
          raiseTime: "string",
          id: "string",
          type: "communication",
        },
      ],
    };
  },
  methods: {
    selsChange() {},
    handleCurrentChange() {},
    submitQueryForm() {
      this.doQuery(getActiveAlarm);
    },
    doQuery(func) {
      (async () => {
        let results = await func();
        console.log("results=", results);
        this.activeAlarms = results.data["active-alarm-list"].activeAlarms;
        console.log("activeAlarms=", this.activeAlarms);
      })();
    },
    tableCellClassName({ row, column, rowIndex, columnIndex }) {
      //第6列添加 red 类
      if (columnIndex == 6) {
        if (String(row.severity) == "critical") {
          return "redClass";
        }
      }
    },
    tableRowClassName({ row, rowIndex }) {
      if (String(row.severity) == "critical") {
        return "critical-row";
      } else if (String(row.severity) == "major") {
        return "major-row";
      } else if (String(row.severity) == "minor") {
        return "minor-row";
      }
      // return 'critical-row';
      // if (rowIndex === 1) {
      //     return 'warning-row';
      //   } else if (rowIndex === 3) {
      //     return 'success-row';
      //   }
    },
  },
  mounted() {
    this.submitQueryForm();
  },
};
</script>

<style lang="scss">
.el-table .critical-row {
  // background:#f56c6c ;
  background: oldlace;
}

.el-table .major-row {
  background: #f0f9eb;
}

.el-table .minor-row {
  background: #ebf9f9;
}
// .critical{
//   background-color: red;
// }
.redClass {
  //  background-color: red;
  color: red;
  font-weight: 1000;
}
// .greenClass{
//    background-color: green;
// }
// .orangeClass, .greenClass, .redClass {
//         position: relative;
//         font-size: 0;
//         &::before {
//             content: '!';
//             position: absolute;
//             top: 50%;
//             left: 50%;
//             transform: translate(-50%, -50%);
//             width: 20px;
//             height: 20px;
//             line-height: 16px;
//             font-size: 14px;
//             font-weight: bold;
//             text-align: center;
//             color: orange;
//             border: 2px solid orange;
//             border-radius: 50%;
//         }
//     }
//     .greenClass {
//         &::before {
//             content: '√';
//             color: green;
//             border: 2px solid green;
//         }
//     }
//     .redClass {
//         &::before {
//             content: '×';
//             line-height: 14px;
//             color: red;
//             border: 2px solid red;
//         }
//     }
//     .emptyClass {
//         background-color: #f8f8f8;
//     }
</style>
