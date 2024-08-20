<template>
  <div>
    <el-card>
      <div class="index-content">LED</div>
      <ul>
        <template v-for="(node, index) in form.leds">
          <!-- <span>{{node.led['led-name']}}</span>
            <span>{{node.led.status}}</span> -->
          <li>
            <div>{{ node.led["led-name"] }}</div>
            <!-- <span>{{node.led.status}}</span> -->
            <!-- <span class="status orange flash">{{node.led.status}}</span> -->
            <div
              :class="{
                greenled: node.led.status == 'green',
                redled: node.led.status == 'red',
                greenblinkled: node.led.status == 'green-blink',
                redblinkled: node.led.status == 'red-blink',
                offled: node.led.status == 'off',
              }"
            ></div>
          </li>
        </template>
      </ul>

      <div class="index-content">Version</div>
      <el-form
        ref="form"
        :model="nodes['org-openroadm-device'].info"
        :rules="formRules"
        label-width="160px"
        label-position="left"
      >
        <template v-for="(node, index) in form['circuit-pack-components']">
          <el-form-item :label="node.component.name" :key="index">
            <el-input
              v-model="node.component['current-version']"
              placeholder
              :disabled="edit"
              :key="index"
            ></el-input>
          </el-form-item>
        </template>
      </el-form>
    </el-card>
  </div>
</template>

<script>
import { mapState, mapMutations, mapActions, mapGetters } from "vuex";
import { generateForm } from "@/utils/i18n";
export default {
  components: {},
  data() {
    return {
      edit: true,
      name: this.$route.name,
      form: {},
      formRules: this.getRule(),
    };
  },
  computed: {
    ...mapGetters({
      nodes: "nodes",
    }),
  },
  mounted() {
    console.log("CpMainBoard mouted, route=", this.$route.name);
    this.checkNum();
  },
  methods: {
    checkNum() {
      console.log("checkNum ,this.name=", this.name);
      this.nodes["org-openroadm-device"]["circuit-packs"].some((item) => {
        console.log("this.name=", this.name);
        // console.log(item['circuit-pack-name']);
        // console.log("bbbbbb",this.name.indexOf(item['circuit-pack-name']));
        // console.log("cccccc",this.fanNum.indexOf(item['circuit-pack-name']));
        if (this.name == item["circuit-pack-name"]) {
          console.log("-------finded ", item["circuit-pack-name"]);
          this.form = item;
          console.log("form=", this.form);
          return;
        }
      });
    },
    generateForm,
    getRule() {
      return {};
    },
    class() {},
    getledstatus(status) {
      console.log("status=", status);
      if (status == "green") {
        return "greenled";
      } else if (status == "green-blink") {      
        return "greenlinkled";
      } else if (status == "red") {
        return "redled";
      } else if (status == "red-blink") {      
        return "redlinkled";
      }
    },
  },
  watch: {
    $route: function() {
      console.log("current router is", this.$route.name);
      this.name = this.$route.name;
      this.checkNum();
    },
  },
};
</script>

<style lang="scss" scoped>
@import "src/styles/form.scss";
// * {
//     background: #f3f3f3;
//     font-family: sans-serif;
//     margin-top: 2%;
// }
ul {
  padding: 0;
  margin: 0 auto;
  background: white;
  border-radius: 0.25rem;
  width: 500px;
  list-style: none;
  border: 1px solid #dcdcdc;
  line-height: 50px;
  font-size: 1rem;
}
ul li {
  border-bottom: 1px solid #dcdcdc;
  text-indent: 10px;
  display: inline-block;
  // float: left;
  padding: 1px 3px 3px 3px;
}
ul li:last-child {
  border: none;
}
// ul .status {
//     font-family: monospace;
//     display: inline-block;
//     float: left;
//     height: 1em;
//     line-height: 1.2em;
//     text-indent: 0;
//     margin: 15px 10px;
//     font-weight: bold;
//     text-transform: uppercase;
//     font-size: 1rem;
//     display: inline-block;
//     border: 1px solid;
//     border-radius: 0.25rem;
//     padding: 1px 3px 3px 3px;
// }
// .red {
//     color: red;
// }
// .orange {
//     color: orange;
// }
// .green {
//     color: green;
// }
// .flash {
//     animation: flash 2s linear infinite;
//     -webkit-animation: flash 2s infinite linear;
//     -ms-animation: flash 2s infinite linear;
//     -moz-animation: flash 2s infinite linear;
// }
// @keyframes flash {
//     0% {
//         opacity: 0;
//     }
//     20% {
//         opacity: 1;
//     }
//     100% {
//         opacity: 1;
//     }
// }
// @-webkit-keyframes flash {
//     0% {
//         opacity: 0;
//     }
//     20% {
//         opacity: 1;
//     }
//     100% {
//         opacity: 1;
//     }
// }
// @-ms-keyframes flash {
//     0% {
//         opacity: 0;
//     }
//     20% {
//         opacity: 1;
//     }
//     100% {
//         opacity: 1;
//     }
// }
// @-moz-keyframes flash {
//     0% {
//         opacity: 0;
//     }
//     20% {
//         opacity: 1;
//     }
//     100% {
//         opacity: 1;
//     }
// }

.greenled {
  @include breath_light(48px, 48px, green, 0s);
}

.redled {
  @include breath_light(48px, 48px, rgb(255, 67, 34), 0s);
}
.greenblinkled {
  @include breath_light(48px, 48px, green, 2s);
}

.redblinkled {
  @include breath_light(48px, 48px, rgb(255, 67, 34), 2s);
}

.offled {
  @include breath_light(48px, 48px, gray, 0s);
}
</style>
