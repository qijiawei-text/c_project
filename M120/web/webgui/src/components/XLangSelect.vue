<template>
<el-select v-model="language" placeholder="请选择" size="mini" @change="handleSetLanguage">
    <el-option
      v-for="item in options"      
      :key="item.value"
      :label="item.label"
      :value="item.value">
    </el-option>
  </el-select>
</template>

<script>
export default {
  name: 'x-lang-select',
  data() {
    return {
     options: [{
          value: 'en',
          label: 'English'
        }, {
          value: 'zh',
          label: '中文'
        }],
      value: 'en'
    }
  },
  components: { },
  computed: {
    language() {
      return this.$store.getters.language
    }
  },
  methods: {
    handleSetLanguage(lang) {
      console.log("lang=",lang)
      console.log("value=",this.value)
      this.$i18n.locale = lang
      this.$store.dispatch('setLanguage', lang)
      this.$message({
        message: 'switch language success',
        type: 'success'
      })
    }
  }
}
</script>

<style scoped>
.international-icon {
  font-size: 22px;
  cursor: pointer;
  vertical-align: -5px!important;
}
.el-dropdown{
  color:#5a5e66;
}
</style>


