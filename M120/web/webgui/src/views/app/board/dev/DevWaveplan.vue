<template>
  <div>
    <!-- <h1> DEV_waveplan</h1> -->
    <waveplan-form ref="formComp"></waveplan-form>
  </div>
</template>

<script>
import { getOpticalModules } from "@/api/opticalModule";
import WaveplanForm from "@/views/app/board/dev/compenents/WaveplanForm";
export default {
  components: {
    WaveplanForm,
  },
  data() {
    return {};
  },
  mounted() {
    console.log("DevWaveplan mouted, route=", this.$route.meta);
  },

  methods: {},
  watch: {
    $route: function() {
      console.log("current router is", this.$route.name);
    },
  },
};
</script>

<style lang="scss" scoped></style>
