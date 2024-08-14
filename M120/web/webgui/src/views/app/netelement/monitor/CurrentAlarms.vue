<template>
    <div>
        <!-- <h1>CurrentAlarms</h1> -->
        <!-- <div class="index-content">State</div> -->
        <active-alarm></active-alarm>
    </div>
</template>

<script>
import ActiveAlarm from '@/views/app/alarm/ActiveAlarm'
    export default {
        components: {
            ActiveAlarm,
        },
    }
</script>

<style lang="scss" scoped>

</style>