<template>
  <div class="app">
    <el-card>
      <div class="index-content">Config</div>
      <el-form
        ref="form"
        :model="form"
        label-width="160px"
        label-position="left"
      >
        <el-form-item label="Syslog Service">
          <el-switch v-model="enabled"></el-switch>
        </el-form-item>
      </el-form>

      <template v-if="enabled">
        <div class="index-content">Server List</div>
        <!--列表-->
        <el-table
          stripe
          :data="form.syslog['log-actions'].remote.destination"
          highlight-current-row
          @selection-change="selsChange"
          style="width: 100%;"
        >
          <!-- <el-table-column type="selection" width="55"> </el-table-column> -->
          <el-table-column type="index" width="55"> </el-table-column>
          <el-table-column prop="name" label="Name" width="230">
          </el-table-column>
          <el-table-column prop="udp.address" label="Address" width="230">
          </el-table-column>
          <el-table-column prop="udp.port" label="Port" width="200">
          </el-table-column>

          <el-table-column label="Action" width="200">
            <template slot-scope="scope">
              <el-button
                size="small"
                @click="handleEdit(scope.$index, scope.row)"
                style="width:60px"
                >Edit</el-button
              >
              <el-button
                type="danger"
                size="small"
                @click="handleDel(scope.$index, scope.row)"
                style="width:60px"
                >Delete</el-button
              >
            </template>
          </el-table-column>
        </el-table>

        <el-col
          :span="24"
          class="toolbar"
          style="margin:28px; text-align:center;"
        >
          <!-- <el-form-item>           -->
          <el-button type="primary" v-on:click="getSyslogs">{{
            generateForm("Query")
          }}</el-button>

          <el-button type="primary" @click="handleAdd">{{
            generateForm("Add")
          }}</el-button>
          <!-- </el-form-item> -->
        </el-col>
      </template>

      <!--编辑界面-->
      <el-dialog
        :title="textMap[dialogStatus]"
        :visible.sync="dialogFormVisible"
        :close-on-click-modal="false"
      >
        <el-form
          :model="editForm"
          label-width="80px"
          :rules="editFormRules"
          ref="editForm"
        >
          <el-form-item
            label="name"
            prop="destination.name"
            :inline-message="true"
          >
            <el-input
              v-model="editForm.destination.name"
              auto-complete="off"
              placeholder="input name please..."
            ></el-input>
          </el-form-item>

          <el-form-item
            label="address"
            prop="destination.udp.address"
            :inline-message="true"
          >
            <el-input
              v-model="editForm.destination.udp.address"
              auto-complete="off"
              placeholder="input ip please..."
            ></el-input>
          </el-form-item>

          <el-form-item
            label="port"
            prop="destination.udp.port"
            :inline-message="true"
          >
            <el-input
              v-model="editForm.destination.udp.port"
              auto-complete="off"
              placeholder="input port please..."
            ></el-input>
          </el-form-item>
        </el-form>
        <div slot="footer" class="dialog-footer">
          <el-button @click.native="dialogFormVisible = false">取消</el-button>
          <el-button
            v-if="dialogStatus == 'create'"
            type="primary"
            @click="createData"
            >添加</el-button
          >
          <el-button v-else type="primary" @click="updateData">修改</el-button>
        </div>
      </el-dialog>
    </el-card>
  </div>
</template>

<script>
import {
  getSyslogListPage,
  removeDestination,
  editDestination,
  addDestination,
} from "@/api/syslog";
import {
  validateName,
  validateAge,
  validateSex,
  validateIp,
  validatePort,
} from "@/utils/validateform";
import { generateTitle, generateInvalid } from "@/utils/i18n";
import { generateForm } from "@/utils/i18n";
export default {
  data() {
    return {
      oldname: "",
      enabled: true,
      dialogStatus: "",
      textMap: {
        update: "Edit",
        create: "Create",
      },
      dialogFormVisible: false,
      editFormRules: {
        "destination.name": [
          { required: true, message: "请输入Name", trigger: "blur" },
        ],
        "destination.udp.port": {
          validator: validatePort,
          required: true,
          message: this.generateInvalid("port"),
          trigger: "blur",
        },
        // //address:{ validator: validateIp, required: true, message: this.$t( 'valid.ip' ), trigger: "blur"},
        "destination.udp.address": {
          validator: validateIp,
          required: true,
          message: this.generateInvalid("ip"),
          trigger: "blur",
        },
      },
      editForm: {
        destination: {
          "log-selector": {
            "log-facility": [
              {
                severity: "emergency",
                "severity-operator": "equals-or-higher",
                facility: "string",
              },
            ],
            "no-facilities": "string",
          },
          name: "string",
          udp: {
            port: 0,
            address: "string",
          },
          tcp: {
            port: 0,
            address: "string",
          },
        },
      },
      form: {
        syslog: {
          "log-actions": {
            remote: {
              destination: [
                {
                  "log-selector": {
                    "log-facility": [
                      {
                        severity: "emergency",
                        "severity-operator": "equals-or-higher",
                        facility: "string",
                      },
                    ],
                    "no-facilities": "string",
                  },
                  name: "string",
                  udp: {
                    port: 0,
                    address: "string",
                  },
                  tcp: {
                    port: 0,
                    address: "string",
                  },
                },
              ],
            },
          },
          "local-syslog-filename": "string",
        },
      },
    };
  },
  methods: {
    generateForm,
    generateTitle,
    generateInvalid,
    getSyslogs() {
      const para = {
        page: this.page,
        // name: this.filters.name,
      };
      getSyslogListPage(para).then((res) => {
        // this.total = res.data.total;
        // this.users = res.data.authentication.user;
        this.form.syslog = res.data.syslog;
        console.log("data-- ", res.data);
        // this.users.forEach((element, index) => {
        //   console.log(element);
        // });
      });
    },
    // 删除
    handleDel(index, row) {
      this.$confirm("确认删除该记录吗?", "提示", {
        type: "warning",
      })
        .then(() => {
          const para = { name: row.name };
          console.log("row=", row);
          console.log("para=", para);
          removeDestination(row).then((res) => {
            this.$message({
              message: "删除成功",
              type: "success",
            });
            this.getSyslogs();
          });
        })
        .catch(() => {});
    },
    // 显示编辑界面
    handleEdit(index, row) {
      this.dialogStatus = "update";
      this.dialogFormVisible = true;
      this.oldname = row.name;
      console.log("will eidt name=", row.name);
      this.editForm.destination = Object.assign({}, row);
    },
    // 显示新增界面
    handleAdd() {
      this.dialogStatus = "create";
      this.dialogFormVisible = true;
      // this.editForm = {
      //   destination:{
      //     name: "string",
      //     udp: {
      //       port: 0,
      //       address: "string",
      //     }
      //   }
      // };
    },
    // 编辑
    updateData() {
      this.$refs.editForm.validate((valid) => {
        if (valid) {
          this.$confirm("确认提交吗？", "提示", {})
            .then(() => {
              const para = Object.assign({}, this.editForm.destination);
              editDestination(para, this.oldname).then((res) => {
                this.$message({
                  message: "提交成功",
                  type: "success",
                });
                this.$refs["editForm"].resetFields();
                this.dialogFormVisible = false;
                this.getSyslogs();
              });
            })
            .catch((e) => {
              // 打印一下错误
              //console.log(e)
            });
        }
      });
    },
    // 新增
    createData: function() {
      this.$refs.editForm.validate((valid) => {
        if (valid) {
          this.$confirm("确认提交吗？", "提示", {})
            .then(() => {
              // this.editForm.id = parseInt(Math.random() * 100).toString(); // mock a id
              const para = Object.assign({}, this.editForm.destination);
              //console.log(para)

              addDestination(para).then((res) => {
                this.$message({
                  message: "提交成功",
                  type: "success",
                });
                this.$refs["editForm"].resetFields();
                this.dialogFormVisible = false;
                this.getSyslogs();
              });
            })
            .catch((e) => {
              // 打印一下错误
              //console.log(e)
            });
        }
      });
    },
    // 全选单选多选
    selsChange(sels) {
      this.sels = sels;
    },
  },
};
</script>

<style lang="scss" scoped>
.app {
  text-align: left;
}
</style>
