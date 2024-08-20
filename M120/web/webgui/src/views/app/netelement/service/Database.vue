<template>
  <div class="app">
    <!-- <h1>Database</h1> -->
    <el-card style="margin:10px 0 10px 0">
      <div class="index-content">Config</div>
      <el-alert
        title="Warning!"
        description=""
        type="warning"
        effect="dark"
        :closable="false"
        show-icon
        style="margin:16px 0 16px 0;text-align:left"
      >
        <template slot="title">
          <div>Warning！</div>
          <div>
            Restore: Restore all parameters to file, effective immediately,
            persist after reset
          </div>
          <div>
            Backup: Backup file nodeid+description+version+timestamp.db to disk
          </div>
        </template>
      </el-alert>
      <!-- </el-card> -->

      <!-- <el-card> -->
      <el-form
        ref="form"
        :model="form"
        :rules="formRules"
        label-width="160px"
        label-position="left"
      >
        <el-form-item label="Database Operation">
          <el-select
            v-model="operation"
            placeholder="请选择"
            filterable
            style="width:100%"
            @change="selectChange"
          >
            <template v-for="(node, index) in options">
              <el-option
                :label="node"
                :value="node"
                :key="index + ''"
              ></el-option>
            </template>
          </el-select>
        </el-form-item>

        <template v-if="operationflag == 'db-backup'">
          <el-form-item
            label="Database Name"
            prop="db-backup.input.filename"
            :rules="formRules['filename']"
          >
            <el-input
              v-model="form['db-backup'].input.filename"
              placeholder=""
            ></el-input>
          </el-form-item>
        </template>

        <template v-if="operationflag == 'db-restore'">
          <el-table
            stripe
            :data="form['show-file-output'].output.file"
            highlight-current-row
            @selection-change="selsChange"
            style="width: 100%;"
          >
            <!-- <el-table-column type="selection" width="55"> </el-table-column> -->
            <el-table-column type="index" width="60"> </el-table-column>
            <el-table-column prop="filename" label="filename" width="330">
            </el-table-column>
            <el-table-column prop="file-size" label="file-size" width="100">
            </el-table-column>
            <el-table-column
              prop="modified-date"
              label="modified-date"
              width="230"
            >
            </el-table-column>
            <el-table-column label="操作" width="230">
              <template slot-scope="scope">
                <el-button
                  size="small"
                  @click="handleRestore(scope.$index, scope.row)"
                  >Restore</el-button
                >
                <el-button
                  type="danger"
                  size="small"
                  @click="handleDel(scope.$index, scope.row)"
                  >删除</el-button
                >
              </template>
            </el-table-column>
          </el-table>
        </template>

        <template v-if="operationflag == 'db-activate'">
          <el-form-item
            label="RollBackTimer"
            prop="db-activate.input.rollBackTimer"
            :rules="formRules['rollBackTimer']"
          >
            <el-input
              v-model="form['db-activate'].input.rollBackTimer"
              placeholder="180"
            ></el-input>
          </el-form-item>
        </template>

        <template v-if="operationflag == 'cancel-rollback-timer'">
          <el-form-item label="accept" style="text-align:left">
            <el-switch
              v-model="form['cancel-rollback-timer'].input.accept"
            ></el-switch>
          </el-form-item>
        </template>
      </el-form>

      <template v-if="operationflag != 'db-restore'">
        <div class="query">
          <el-button @click="submitApplyForm" style=" background-color:#FBAE42"
            >Apply</el-button
          >
        </div>
      </template>
    </el-card>
  </div>
</template>

<script>
import { notEmpty, validateNum } from "@/utils/validateform";
import { generateTitle, generateInvalid } from "@/utils/i18n";
import {
  setDbBackup,
  setDbStore,
  setDbActivate,
  setDbInit,
  setCancelRollbackTimer,
} from "@/api/database";
import { setDeleteFile, setShowFile, setTransfer } from "@/api/fileTransfer";
export default {
  data() {
    return {
      options: [
        "db-backup",
        "db-restore",
        "db-activate",
        "database-init",
        "cancel-rollback-timer",
      ],
      form: {
        "db-backup": {
          input: {
            filename: "/db-backups",
          },
        },
        "db-restore": {
          input: {
            filename: "string",
            nodeIDCheck: true,
          },
        },
        "db-activate": {
          input: {
            rollBackTimer: "180",
          },
        },
        "cancel-rollback-timer": {
          input: {
            accept: false,
          },
        },
        "show-file-input": {
          input: {
            filename: "string",
          },
        },
        "show-file-output": {
          output: {
            file: [
              {
                "file-size": 0,
                "modified-date": "string",
                filename: "string",
              },
              {
                "file-size": 1,
                "modified-date": "string",
                filename: "string",
              },
            ],
            "status-message": "string",
            status: "Successful",
          },
        },
      },
      operation: "db-backup",
      formRules: this.getRule(),
    };
  },
  computed: {
    operationflag() {
      return this.operation;
    },
  },
  methods: {
    getRule() {
      return {
        // filename: [{ validator: validateName, trigger: "blur" }],
        filename: {
          validator: notEmpty,
          required: true,
          message: "filename is empty",
          trigger: "blur",
        },
        rollBackTimer: [
          {
            type: "number",
            validator: notEmpty,
            required: true,
            message: "rollBack timer is empty",
            trigger: "blur",
          },
          {
            validator: validateNum,
            message: " is not a number",
            trigger: "blur",
          },
        ],
      };
    },
    submitQueryForm() {
      this.doApply(this.form["show-file-input"], setShowFile);
    },
    submitApplyForm() {
      console.log("operation=", this.operation);
      if (this.operation == "db-backup") {
        console.log("db-backup");
        this.doApply(this.form["db-backup"], setDbBackup);
      } else if (this.operation == "db-activate") {
        console.log("db-activate");
        this.doApply(this.form["db-activate"], setDbActivate);
      } else if (this.operation == "database-init") {
        console.log("database-init");
        this.doApply(null, setDbInit);
      } else if (this.operation == "cancel-rollback-timer") {
        console.log("cancel-rollback-timer");
        this.doApply(
          this.form["cancel-rollback-timer"],
          setCancelRollbackTimer
        );
      }
    },
    getFormPromise(form) {
      return new Promise((resolve) => {
        form.validate((res) => {
          resolve(res);
        });
      });
    },
    doApply(form, func) {
      this.getFormPromise(this.$refs.form).then(
        (res) => {
          console.log("res=", res);
          const validateResult = res;
          if (validateResult) {
            // alert("submit!");
            console.log("表单校验通过");
            (async () => {
              let results = await func(form);
              console.log("submit DB Setting results=", results);
              this.$message({
                message: results.status + " " + results.data.output.status,
                type: "success",
              });
              if ("db-restore" == this.operation) {
                if (results.data.output.file) {
                  this.form["show-file-output"].output.file =
                    results.data.output.file;
                }
                this.form["show-file-output"].output.status =
                  results.data.status;
                this.form["show-file-output"].output["status-message"] =
                  results.data["status-message"];
              }
            })();
          }
        },
        () => {
          console.log("error submit!!,表单未校验通过");
        }
      );
    },
    selsChange() {},
    selectChange() {
      console.log("select changed,now is=", this.operation);
      if ("db-restore" == this.operation) {
        console.log("will get file list");
        this.submitQueryForm();
      }
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
          setDeleteFile(row).then((res) => {
            this.$message({
              message: "删除成功",
              type: "success",
            });
            this.submitQueryForm();
          });
        })
        .catch(() => {});
    },
    // 显示编辑界面
    handleRestore(index, row) {
      console.log("will eidt name=", row.filename);
      let fileInput = {
        input: {
          filename: row.filename,
          nodeIDCheck: true,
        },
      };
      console.log("row=", row);
      console.log("fileInput=", fileInput);
      this.doApply(fileInput, setDbStore);
    },
  },
};
</script>

<style lang="scss" scoped></style>
