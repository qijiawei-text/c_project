<template>
  <div class="form">
    <!-- <h2>RadiusForm.vue</h2> -->
    <el-card>
      <div class="index-content">Config</div>
      <el-form
        ref="form"
        :model="form"
        :rules="formRules"
        label-width="160px"
        label-position="left"
      >
        <el-form-item label="timeout">
          <el-input
            v-model="form.radius.options.timeout"
            placeholder=""
          ></el-input>
        </el-form-item>
        <el-form-item label="attempts">
          <el-input
            v-model="form.radius.options.attempts"
            placeholder=""
          ></el-input>
        </el-form-item>

        <el-card>
         
        <div style="text-align:left">         
           <el-button type="primary" @click="handleAdd" >Add Server</el-button>         
        </div>
        <el-table
          stripe
          :data="form.radius.server"
          highlight-current-row
          @selection-change="selsChange"
          style="width: 100%;"
        >
          <el-table-column type="selection" width="55"> </el-table-column>
          <el-table-column type="index" width="60"> </el-table-column>
          <el-table-column prop="name" label="name" width="80">
          </el-table-column>
          <el-table-column
            prop="address"
            label="address"
            width="120"
          ></el-table-column>
          <el-table-column
            prop="authentication-type"
            label="authentication-type"
            width="150"
          ></el-table-column>
          <el-table-column
            prop="shared-secret"
            label="shared-secret"
            width="120"
          ></el-table-column>
          <el-table-column
            prop="authentication-port"
            label="authentication-port"
            width="150"
          ></el-table-column>

          <el-table-column label="操作" width="150">
            <template slot-scope="scope">
              <el-button
                size="small"
                @click="handleEdit(scope.$index, scope.row)"
                >编辑</el-button
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
        </el-card>
      </el-form>
    </el-card>


    <el-dialog
      :title="textMap[dialogStatus]"
      :visible.sync="dialogFormVisible"
      :close-on-click-modal="false"
    >
      <el-form
      ref="server"
      :model="server"
      :rules="formRules"
      label-width="160px"
      label-position="left"
      >
        <el-form-item
          label="authentication-type"
          :prop="'authentication-type'"
          :rules="formRules['authentication-type']"
        >
          <el-input
            v-model="server['authentication-type']"
            placeholder=""
          ></el-input>
        </el-form-item>

        <el-form-item
          label="name"
          :prop="'name'"
          :rules="formRules.name"
        >
          <el-input v-model="server.name" placeholder=""></el-input>
        </el-form-item>

       <el-form-item
          label="address"
          :prop="'udp.address'"
          :rules="formRules.address"
        >
          <el-input
            v-model="server.udp.address"
            placeholder="input ip address"
          ></el-input>
        </el-form-item>
        
        <el-form-item
          label="shared-secret"
          :prop="'udp.shared-secret'"
          :rules="formRules['shared-secret']"
        >
          <el-input
            v-model="server.udp['shared-secret']"
            placeholder="input shared-secret"
          ></el-input>
        </el-form-item>

        <el-form-item
          label="authentication-port"
          :prop="'udp.authentication-port'"
          :rules="formRules['authentication-port']"
        >
          <el-input
            v-model="server.udp['authentication-port']"
            placeholder="input authentication-port"
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
   
  </div>
</template>

<script>
import { notEmpty, validateName, } from "@/utils/validateform";
import {addRadiusServer,getRadius,editRadiusServer,removeRadiusServer } from "@/api/radius"

class RadiusForm {
  constructor() {}

  static getRule() {
    return {};
  }
}

export default {
  data() {
    return {
      dialogStatus: "",
      textMap: {
        update: "Edit",
        create: "Create",
      },
      dialogFormVisible:false,
      server: {
        "authentication-type": "string",
        name: "string",
        udp: {
          address: "string",
          "shared-secret": "string",
          "authentication-port": 0,
        },
      },
      form: {
        radius: {
          options: {
            timeout: 0,
            attempts: 0,
          },
          server: [
            {
                "authentication-type": "string",
                name: "string",
                udp: {
                  address: "string",
                  "shared-secret": "string",
                  "authentication-port": 0,
                },
            },
          ]
        },
      },

      formRules: this.getRule(),
    };
  },
  methods: {
    getRule() {
      return {
        "authentication-type": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.authentication-type"),
          trigger: "blur",
        },
        name: {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.name"),
          trigger: "blur",
        },
        address: {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.address"),
          trigger: "blur",
        },
        "shared-secret": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.shared-secret"),
          trigger: "blur",
        },
        "authentication-port": {
          validator: notEmpty,
          required: true,
          message: this.$t("invalid.authentication-port"),
          trigger: "blur",
        },
      };
    },

    getServer(){     
        (async () => {
          let results = await getRadius();
          console.log("results=", results);
          console.log(this.$refs.formComp.form);
          //this.$refs.form.resetFields();
          this.form = results.data;
          // this.$refs.formComp.form.radius.options = results.data.radius.options;
        })();
    },
    handleDel(index, row) {
      this.$confirm("确认删除该记录吗?", "提示", {
        type: "warning",
      })
        .then(() => {
          const para = { name: row.name };
           console.log("###############row=",row);
          console.log("###############para=",para);
          removeRadiusServer(row).then((res) => {
            this.$message({
              message: "删除成功",
              type: "success",
            });
            this.getServer();
          });
        })
        .catch(() => {});
    },
    handleEdit(index, row) {
      this.dialogStatus = "update";
      this.dialogFormVisible = true;
      this.oldname = row.name
      console.log("will eidt name=",row.name);
      this.editForm = Object.assign({}, row);
    },
    handleAdd() {
      this.dialogStatus = "create";
      this.dialogFormVisible = true;
      // this.editForm = {
      //   name: "",
      //   password: "",
      //   "userrole": 0,
      // };
    },
        // 编辑
    updateData() {
      this.$refs.form.validate((valid) => {
        if (valid) {
          this.$confirm("确认提交吗？", "提示", {})
            .then(() => {
              const para = Object.assign({}, this.form);
              editRadiusServer(para,this.oldname).then((res) => {
                this.$message({
                  message: "提交成功",
                  type: "success",
                });
                this.$refs["server"].resetFields();
                this.dialogFormVisible = false;
                this.getServer();
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
      this.$refs.server.validate((valid) => {
        if (valid) {
          this.$confirm("确认提交吗？", "提示", {})
            .then(() => {
              // this.editForm.id = parseInt(Math.random() * 100).toString(); // mock a id
              const para = Object.assign({}, this.server);
              console.log(para)

              addRadiusServer(para).then((res) => {
                this.$message({
                  message: "提交成功",
                  type: "success",
                });
                this.$refs.server.resetFields();
                this.dialogFormVisible = false;
                // this.getServer();
              }).catch((e) => {
                console.log("eeeeeeeeeeeeeeeeeeeeeeeeeeee",e)
                // this.dialogFormVisible = false;
                });
            })
            .catch((e) => {
              // 打印一下错误
              console.log("fffffffffffffffffff")
              console.log(e)
              this.dialogFormVisible = false;
            });
        }
      });
    },
        // 全选单选多选
    selsChange(sels) {
      this.sels = sels;
    },
  },
  computed: {},
  created() {},
};
</script>

<style lang="scss" scoped>
.form {
  text-align: left;
  //  display: flex;
  //  width: auto;
  /* width: 400px; */
  /* height: 350px;
  padding: 20px;
  border: 1px solid #ccc;
  margin-left: 30px; */
  // .el-form-item {
  //   margin-top: 28px;
  //   margin-bottom: 18px;
  //   padding: 0;
  //   margin: 10px;
  // }
}

</style>
