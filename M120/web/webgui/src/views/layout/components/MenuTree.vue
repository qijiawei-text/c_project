<template>
  <div class="menu-wrapper">
    <template v-for="(node, index) in nodes">
      <div v-if="!node.hidden" :key="index">
        <el-submenu
          :index="index + ''"
          v-if="!node.leaf"
          class="parent"
          @contextmenu.prevent.native="openMenu(node, $event)"
        >
          <template slot="title">
            <!-- <i class="el-icon-menu"></i> -->
            <i :class="node.iconCls"></i>
            <!-- {{node.name}} -->
            {{ generateTitle(node.meta.title) }}
          </template>
          <menu-tree :nodes="node.children" :curtab="curtab"></menu-tree>
        </el-submenu>
        <!-- 这里是用:route="{name:node.name}，如果不使用name跳转的话，可以在数据源那里把path写成全路径即可 -->
        <el-menu-item
          v-if="node.leaf"
          :index="node.path"
          :route="{ name: node.name }"
          class="sub"
          @contextmenu.prevent.native="openMenu(node, $event)"
        >
          <!-- <el-menu-item v-if="node.leaf" :index="node.path" :route="{name:node.name}"  :class="$route.path==node.path?'is-active':''"> -->
          <!-- <i class="el-icon-menu"></i> -->
          <i :class="node.iconCls"></i>
          <!-- {{node.name}} -->
          {{ generateTitle(node.meta.title) }}
          <!-- {{curtab}} -->
        </el-menu-item>
      </div>
    </template>
    <ul
      class="contextmenu"
      v-show="visible"
      :style="{ left: left + 'px', top: top + 'px' }"
    >
      <li v-if="nextaction" @click="activate()">
        {{ generateTitle("Activate") }}
      </li>
      <li v-if="!nextaction" @click="deactivate()">
        {{ generateTitle("Deactivate") }}
      </li>
    </ul>
  </div>
</template>

<script type="text/ecmascript-6">
//import Fragment from "vue-fragment";
import { generateTitle } from "@/utils/i18n";
export default {
  name: "MenuTree",
  components: {
    //fragment: Fragment
  },
  data() {
    return {
      visible: false,
      top: 0,
      left: 0,
      selectedNode: {},
      nextaction:true
    };
  },
  methods: {
    generateTitle,
    activate() {
      this.$message({
        message: "Activate...",
        type: "success",
      });
    },
    deactivate() {
      this.$message({
        message: "Deactivate...",
        type: "success",
      });
    },
    openMenu(node, e) {
      e.stopPropagation();
      console.log("open0...",this.curtab);
      console.log("node.meta.status=",node.meta.status);
      console.log(node.meta.status==null);
       console.log(node.meta.status=='');
       console.log("node.meta.level=",node.meta.level);
      if(this.curtab == 1 && node.meta.level==2 && node.meta.status!=''){
        console.log("open1...",node);
        this.visible = true
        this.selectedNode = node
        this.left = e.clientX
        // this.top = e.clientY
        this.top = e.clientY -120 >0? e.clientY -120:0
        console.log( e.clientX, e.clientY);
        if(node.meta.status == "activate"){
          this.nextaction = false;
        }else if(node.meta.status == "deactivate"){
          this.nextaction = true;
        }
      }
    },
    closeMenu() {
      console.log("closeMenu...");
      this.visible = false
    }
  },
  props: ['nodes','curtab'],
  created() {
    console.log("submenu created....", this.nodes);
  },
  mounted() {
    console.log("submenu mounted,this=", this);
    console.log("submenu mounted....", this.nodes);
    console.log("submenu mounted,curtab1=", this.curtab1);
  },
  updated() {
    console.log("submenu updated....", this.nodes);
  },
  destroyed() {
    console.log("submenu destroyed....", this.nodes);
  },
  watch: {
    nodes(val, oldVal) {
      //普通的watch监听
      console.log("#########nodes: " + val, oldVal);
    },
    visible(value) {
      if (value) {
        document.body.addEventListener('click', this.closeMenu)
      } else {
        document.body.removeEventListener('click', this.closeMenu)
      }
    }
  }
};
</script>

<style lang="scss" scoped>
.menu-wrapper {
  // align-items: left;
  text-align: left;
}

.contextmenu {
  margin: 0;
  background: #fff;
  z-index: 10002;
  position: absolute;
  list-style-type: none;
  padding: 5px 0;
  border-radius: 4px;
  font-size: 12px;
  font-weight: 400;
  color: #333;
  text-align: left;
  box-shadow: 3px 3px 4px 0 rgba(0, 0, 0, 0.3);
  li {
    margin: 0;
    padding: 7px 16px;
    cursor: pointer;
    &:hover {
      background: #eee;
    }
  }
}
</style>
