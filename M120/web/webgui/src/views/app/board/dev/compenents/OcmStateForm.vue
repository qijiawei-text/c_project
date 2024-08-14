<template>
    <div class="form">
        <!-- <h1>ocm state</h1> -->
        <!-- {{channelmonitor.name}} -->
        <el-card>
        <div class="index-content">State</div>
        <el-table
            :data="channelmonitor.channels.channel"
            style="width: 100%"
            stripe
            >

            <el-table-column type="index" label="index" width="60"> </el-table-column>
            <el-table-column :prop="channelmonitor.name" label="channel No" width="160"> </el-table-column>
            <el-table-column prop="state.upper-frequency" label="upper-frequency" width="180"> </el-table-column>
            <el-table-column prop="state.lower-frequency" label="lower-frequency" width="180"> </el-table-column>
            <el-table-column prop="channelmonitor.name" label="port description" width="180">{{channelmonitor.name}}</el-table-column>
            <el-table-column prop="state.psd" label="power" width="240"> </el-table-column>

        </el-table>
        </el-card>
    </div>
</template>

<script>
    export default {
        props: {
            channelmonitor: {
                required: true
            },
        },            
    }
</script>

<style lang="scss" scoped>
    .form{
        margin: 8px;
    }
</style>