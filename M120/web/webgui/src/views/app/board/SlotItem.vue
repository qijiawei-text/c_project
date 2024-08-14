<template>
  <div>
    <!-- <h1>slot</h1> -->
    <router-view></router-view>
  </div>
</template>

<script>
export default {
  mounted() {
    console.log("slotitem mounted...");
  }
};
</script>

<style lang="scss" scoped>
</style>