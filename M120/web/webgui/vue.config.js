module.exports = {
    // overlay: {
    //     warnings: false,
    //     errors: false
    //  },
     lintOnSave: false,

    devServer: {
        open: process.platform === "darwin",
        disableHostCheck: false,
        host: "0.0.0.0",
        port: 8999,
        https: false,
        hotOnly: false, // See https://github.com/vuejs/vue-cli/blob/dev/docs/cli-service.md#configuring-proxy
        //proxy: null, // string | Object
        
        proxy: {
            //for login
            // '/user': { //使用"/api"来代替"真实地址" 
            //     target: 'http://localhost:8082',//真实地址
            //     //secure: false,//https:
            //     changeOrigin: true,//改变源 
            //     pathRewrite: {
            //         '^/user': '/user'//路径重写 
            //     },
            // },
            // '/api': { //使用"/api"来代替"真实地址" 
            //     target: 'http://localhost:8082',//真实地址
            //     //secure: false,//https:
            //     changeOrigin: true,//改变源 
            //     pathRewrite: {
            //         '^/api': '/api'//路径重写 
            //     },
            // },
            '/omt': { //使用"/api"来代替"真实地址" 
                target: 'http://localhost:8080',//真实地址
                //target: 'http://localhost:8082',//真实地址
                //target: 'http://172.16.161.97:5000',//真实地址
                //target: 'http://localhost:5000',//真实地址
                //target: 'http://172.16.166.239:80',//真实地址
                //target: 'http://172.16.161.103:80',//真实地址
                //secure: false,//https:
                changeOrigin: true,//改变源 
                pathRewrite: {
                    '^/omt': '/omt'//路径重写 
                },
            },
            '/data': { //使用"/api"来代替"真实地址" 
            target: 'http://localhost:8080',//真实地址
            //target: 'http://localhost:8082',//真实地址
            //target: 'http://172.16.161.97:5000',//真实地址
            //target: 'http://localhost:5000',//真实地址
            //target: 'http://172.16.166.239:80',//真实地址
            //target: 'http://172.16.161.103:80',//真实地址
            //secure: false,//https:
            changeOrigin: true,//改变源 
            pathRewrite: {
                '^/data': '/data'//路径重写 
            },
        },
        '/operations': { //使用"/api"来代替"真实地址" 
        target: 'http://localhost:8080',//真实地址
        //target: 'http://localhost:8082',//真实地址
        //target: 'http://172.16.161.97:5000',//真实地址
        //target: 'http://localhost:5000',//真实地址
        //target: 'http://172.16.166.239:80',//真实地址
        //target: 'http://172.16.161.103:80',//真实地址
        //secure: false,//https:
        changeOrigin: true,//改变源 
        pathRewrite: {
            '^/operations': '/operations'//路径重写 
        },
    },
        },
        
        // before: app => {}
    }, // 第三方插件配置




};