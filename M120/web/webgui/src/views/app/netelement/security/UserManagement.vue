<template>
  <el-card class="user-container"
    ><!-- <section class="user-container"> -->
    <!--工具条-->
    <div class="index-content">Config</div>
    <el-col
      :span="24"
      class="toolbar"
      style="padding-bottom: 0px; text-align:left"
    >
      <el-form :inline="true" :model="filters" @submit.native.prevent>
        <!-- <el-form-item>
          <el-input v-model="filters.name" placeholder="Name"></el-input>
        </el-form-item> -->
        <el-form-item>
          <el-button type="primary" v-on:click="getUsers" style="width:80px">{{
            generateForm("Query")
          }}</el-button>
        </el-form-item>
        <el-form-item>
          <el-button type="primary" @click="handleAdd" style="width:80px">{{
            generateForm("Add")
          }}</el-button>
        </el-form-item>
      </el-form>
    </el-col>

    <!--列表-->
    <el-table
      stripe
      :data="users.slice((page - 1) * size, page * size)"
      highlight-current-row
      @selection-change="selsChange"
      style="width: 100%;"
    >
      <el-table-column type="selection" width="55"> </el-table-column>
      <el-table-column type="index" width="55"> </el-table-column>
      <el-table-column prop="name" label="name" width="100"> </el-table-column>

      <el-table-column
        prop="password"
        label="password"
        width="190"
      ></el-table-column>
      <el-table-column
        prop="user-role"
        label="user-role"
        width="60"
      ></el-table-column>

      <el-table-column prop="create-time" label="create-time" width="160">
      </el-table-column>
      <el-table-column prop="last-operation" label="last-operation" width="160">
      </el-table-column>

      <el-table-column label="操作" width="160">
        <template slot-scope="scope">
          <el-button size="small" @click="handleEdit(scope.$index, scope.row)"
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

    <!--工具条-->
    <el-col :span="24" class="toolbar">
      <!-- <el-button
        type="danger"
        @click="batchRemove"
        :disabled="this.sels.length === 0"
        >批量删除</el-button
      > -->
      <!-- <el-pagination
        layout="prev, pager, next"
        @current-change="handleCurrentChange"
        :page-size="10"
        :total="total"
      >
      </el-pagination> -->
      <el-pagination
        @size-change="handleSizeChange"
        @current-change="handleCurrentChange"
        :current-page="page"
        :page-sizes="[5, 10, 15, 20]"
        :page-size="size"
        layout="total, sizes, prev, pager, next, jumper"
        :total="total"
        style="float:right;"
      >
      </el-pagination>
    </el-col>

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
        <el-form-item label="name" prop="name">
          <el-input v-model="editForm.name" auto-complete="off"></el-input>
        </el-form-item>

        <el-form-item label="password" prop="password">
          <el-input
            v-model="editForm.password"
            auto-complete="off"
            :type="passwordType"
          ></el-input>
          <i
            class="el-icon-view el-input__icon"
            slot="suffix"
            @click="showPassword"
          ></i>
          <i slot="prefix" class="icon-mima"></i>
        </el-form-item>

        <el-form-item label="user-role">
          <!-- <el-radio-group v-model="editForm['user-role']">
            <el-radio class="radio" :label="2">admin</el-radio>
            <el-radio class="radio" :label="1">operator</el-radio>
            <el-radio class="radio" :label="0">query</el-radio>
          </el-radio-group> -->
          <el-select
            v-model="editForm['userrole']"
            placeholder="请选择..."
            filterable
          >
            <template v-for="(node, index) in types">
              <el-option
                :label="node.label"
                :value="node.value"
                :key="index + ''"
              ></el-option>
            </template>
          </el-select>
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
    </el-dialog> </el-card
  ><!-- </section> -->
</template>

<script>
import util from "@/utils/table.js";
import {
  getUserListPage,
  removeUser,
  batchRemoveUser,
  editUser,
  addUser,
} from "@/api/userTable";
import { generateForm } from "@/utils/i18n";

export default {
  data() {
    return {
      oldname: "",
      // types:['admin','operator','query'],
      types: [
        {
          value: "4",
          label: "admin",
        },
        {
          value: "2",
          label: "operator",
        },
        {
          value: "1",
          label: "query",
        },
      ],
      passwordType: "password",
      dialogStatus: "",
      textMap: {
        update: "Edit",
        create: "Create",
      },
      dialogFormVisible: false,
      filters: {
        name: "",
      },
      users: [],
      total: 0,
      page: 1,
      size: 5,
      sels: [], // 列表选中列
      editFormRules: {
        name: [{ required: true, message: "请输入姓名", trigger: "blur" }],
      },

      editForm: {
        name: "",
        password: "",
        userrole: "4",
      },

      addFormVisible: false, // 新增界面是否显示
      addFormRules: {
        name: [{ required: true, message: "请输入姓名", trigger: "blur" }],
      },
    };
  },
  methods: {
    generateForm,
    showPassword() {
      this.passwordType === ""
        ? (this.passwordType = "password")
        : (this.passwordType = "");
    },
    handleSizeChange(val) {
      console.log(`每页 ${val} 条`);
      this.size = val;
    },
    handleCurrentChange(val) {
      this.page = val;
      console.log(`当前页: ${val}`);
      this.getUsers();
    },
    // 获取用户列表
    getUsers() {
      const para = {
        page: this.page,
        name: this.filters.name,
      };
      getUserListPage(para).then((res) => {
        this.total = res.data.authentication.user.length;
        console.log("^^^^^^^this.total =", this.total);
        this.users = res.data.authentication.user;
        console.log("data-- ", res.data);
        this.users.forEach((element, index) => {
          console.log(element);
          // if(index % 2)
          // element.level = "Minor"
          // else
          // element.level = "Critical"
        });
      });
    },
    // 删除
    handleDel(index, row) {
      this.$confirm("确认删除该记录吗?", "提示", {
        type: "warning",
      })
        .then(() => {
          const para = { name: row.name };
          console.log("###############row=", row);
          console.log("###############para=", para);
          removeUser(row).then((res) => {
            this.$message({
              message: "删除成功",
              type: "success",
            });
            this.getUsers();
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
      this.editForm = Object.assign({}, row);
    },
    // 显示新增界面
    handleAdd() {
      this.dialogStatus = "create";
      this.dialogFormVisible = true;
      this.editForm = {
        name: "",
        password: "",
        userrole: 0,
      };
    },
    // 编辑
    updateData() {
      this.$refs.editForm.validate((valid) => {
        if (valid) {
          this.$confirm("确认提交吗？", "提示", {})
            .then(() => {
              const para = Object.assign({}, this.editForm);
              editUser(para, this.oldname).then((res) => {
                this.$message({
                  message: "提交成功",
                  type: "success",
                });
                this.$refs["editForm"].resetFields();
                this.dialogFormVisible = false;
                this.getUsers();
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
              const para = Object.assign({}, this.editForm);
              //console.log(para)

              addUser(para).then((res) => {
                this.$message({
                  message: "提交成功",
                  type: "success",
                });
                this.$refs["editForm"].resetFields();
                this.dialogFormVisible = false;
                this.getUsers();
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
    // 批量删除
    batchRemove() {
      var ids = this.sels.map((item) => item.id).toString();
      this.$confirm("确认删除选中记录吗？", "提示", {
        type: "warning",
      })
        .then(() => {
          const para = { ids: ids };
          batchRemoveUser(para).then((res) => {
            this.$message({
              message: "删除成功",
              type: "success",
            });
            this.getUsers();
          });
        })
        .catch(() => {});
    },
  },
  mounted() {
    this.getUsers();
  },
};
</script>

<style scoped>
.user-container {
  text-align: left;
  border: 1px solid greenyellow;
}
.toolbar {
  margin: 6px, 0px;
}
</style>
