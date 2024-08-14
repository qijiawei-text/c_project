<template>
  <div class="rest">
    <h1 style="color:red">{{ title }}</h1>

    <el-row>
      <el-col :span="24">
        <div class="grid-content bg-purple-dark title">
          <h4>Request</h4>
        </div>
      </el-col>
    </el-row>

    <el-row>
      <el-col :span="4">
        <div class="grid-content bg-purple">
          <el-select
            v-model="value"
            placeholder="请选择"
            @change="changemode($event)"
          >
            <el-option
              v-for="item in options"
              :key="item.value"
              :label="item.label"
              :value="item.value"
            ></el-option>
          </el-select>
        </div>
      </el-col>
      <el-col :span="18">
        <div class="grid-content bg-purple-light rest-uri">
          <el-input
            v-model="requesturi"
            placeholder="Please input request uri"
          ></el-input>
        </div>
      </el-col>
      <el-col :span="2">
        <div class="grid-content bg-purple">
          <el-button type="success" @click="test_get()">Send</el-button>
        </div>
      </el-col>
    </el-row>

    <el-row v-show="postflag">
      <el-col :span="4">
        <div class="grid-content bg-purple-dark"></div>
      </el-col>
      <el-col :span="20">
        <div class="grid-content bg-purple-dark rest-post-para">
          <el-input
            type="textarea"
            :autosize="{ minRows: 2, maxRows: 4 }"
            v-model="param"
            placeholder="Please input post JSON param"
          ></el-input>
        </div>
      </el-col>
    </el-row>

    <el-row>
      <el-col :span="24">
        <div class="grid-content bg-purple-dark title">
          <h4>Response</h4>
        </div>
      </el-col>
    </el-row>
    <el-row>
      <el-col :span="24">
        <div class="grid-content bg-purple-dark rest-res">
          <el-input
            type="textarea"
            :autosize="{ minRows: 6, maxRows: 10 }"
            v-model="databuf"
            placeholder
          ></el-input>
        </div>
      </el-col>
    </el-row>

    <!-- <h4>{{requesturi}}</h4>  -->
  </div>
</template>

<script>
import { apiGet } from "@/api/restGet";
export default {
  data() {
    return {
      title: "this page only for test",
      requesturi: "",

      options: [
        {
          value: "选项1",
          label: "GET",
        },
        {
          value: "选项2",
          label: "POST",
        },
      ],
      value: "选项1",
      label: "GET",
      databuf: "",
      param: "",
      postflag: false,
    };
  },
  methods: {
    test_get() {
      console.log("this.value=" + this.value);
      console.log(
        "this is rest get,type=" + this.label + "uri=" + this.requesturi
      );
      apiGet(this.label, this.requesturi, this.param)
        .then((res) => {
          console.log("res", res);
          this.databuf = JSON.stringify(res);
        })
        .catch((err) => {
          console.log("apiget err", err);
          this.databuf = err;
        });
    },
    changemode(e) {
      console.log("changemode", e);
      console.log("label=" + this.label);
      console.log("value=" + this.value);
      console.log("e.lable=" + e.lable);
      console.log("e.value=" + e.value);
      this.lable = e.lable;
      if ("选项2" === this.value) {
        console.log("选项2");
        this.postflag = true;
        this.label = "POST";
        console.log("postflag = " + this.postflag);
        this.requesturi = "";
      } else {
        console.log("选项1");
        this.postflag = false;
        this.label = "GET";
        console.log("postflag = " + this.postflag);
        this.requesturi = "";
      }
    },
  },
};
</script>

<style lang="scss" scoped>
.rest {
  margin: 0, 20, 20, 20;
}

.rest-type {
  float: left;
}

.rest-uri {
  float: left;
  width: 100%;
  margin-left: 2px;
}

.rest-res {
  background-color: red;
  margin-top: 1px;
}

.rest-post-para {
  margin: 5px, 0, 5px, 10px;
  background-color: blue;
}

.title {
  float: left;
  margin-bottom: 0px;
  margin-top: 20px;
}

.el-row {
  margin-bottom: 20px;
  &:last-child {
    margin-bottom: 0;
  }
}
.el-col {
  border-radius: 4px;
}

.grid-content {
  border-radius: 4px;
  min-height: 12px;
}
</style>
