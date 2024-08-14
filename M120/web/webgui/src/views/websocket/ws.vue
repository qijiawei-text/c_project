<template>
  <section id="increment" class="group2">
    <h1>this is websocket test</h1>
    <div class="title">libwebsockets "dumb-increment-protocol"</div>

    <table>
      <tr>
        <td>
          <table class="content" width="200px">
            <tr>
              <td align="center">
                <input
                  type="button"
                  id="offset"
                  value="Reset counter"
                  @click="reset"
                />
              </td>
            </tr>
            <tr>
              <td width="200px" align="center">
                <div id="number"></div>
              </td>
            </tr>
            <tr>
              <td id="wsdi_statustd" align="center" class="explain">
                <div id="wsdi_status">Not initialized</div>
              </td>
            </tr>

            <input
              type="button"
              name="hello"
              id="1"
              value="click to send"
              @click="sendtext"
            />
          </table>
        </td>
      </tr>

      <tr>
        <td class="explain">
          The incrementing number is coming from the server and is individual
          for each connection to the server... try opening a second browser
          window.
          <br />
          <br />
          The button zeros just this connection's number.
        </td>
      </tr>
    </table>
  </section>
</template>

<script>
export default {
  data() {
    return {
      websock: null,
      reconnectData: null,
      lockReconnect: false, //避免重复连接，因为onerror之后会立即触发 onclose
      timeout: 10000, //10s一次心跳检测
      timeoutObj: null,
      serverTimeoutObj: null,
      recvdata: "",
    };
  },
  created() {
    this.initWebSocket();
    console.log("websocket has been inited...");
  },
  methods: {
    initWebSocket() {
      console.log("启动中");
      //let wsurl = "ws://172.16.161.103:7681/"; //
      let wsurl = "ws://localhost/ws";
      //let wsurl = "ws://10.220.10.28/ws";
      //let wsurl = "ws://172.16.161.103/ws";
      console.log("wsurl= " + wsurl);
      this.websock = new WebSocket(wsurl, "dumb-increment-protocol");
      this.websock.onopen = this.websocketonopen; //连接成功
      this.websock.onmessage = this.websocketonmessage; //广播成功

      this.websock.onmessage = function got_packet(msg) {
        console.log("received message" + msg.data);
        document.getElementById("number").textContent = msg.data + "\n";
      };

      this.websock.onerror = this.websocketonerror; //连接断开，失败
      this.websock.onclose = this.websocketclose; //连接关闭
    }, //初始化weosocket
    websocketonopen() {
      console.log("连接成功");
      document.getElementById("wsdi_statustd").style.backgroundColor =
        "#40ff40";
      document.getElementById("wsdi_status").textContent =
        " websocket connection opened ";
      this.heatBeat();
    }, //连接成功
    websocketonerror() {
      console.log("连接失败");
      //this.reconnect();
    }, //连接失败
    websocketclose() {
      console.log("断开连接");
      document.getElementById("wsdi_statustd").style.backgroundColor =
        "#ff4040";
      document.getElementById("wsdi_status").textContent =
        " websocket connection CLOSED ";
      //this.reconnect();
    }, //各种问题导致的 连接关闭
    websocketonmessage(data) {
      this.heatBeat(); //收到消息会刷新心跳检测，如果一直收到消息，就推迟心跳发送
      console.log("接收到数据", data);
      this.recvdata = data;
      //let msgData = JSON.parse(data);
    }, //数据接收
    websocketsend(data) {
      this.websock.send(JSON.stringify(data));
    }, //数据发送
    reconnect() {
      if (this.lockReconnect) {
        //这里很关键，因为连接失败之后之后会相继触发 连接关闭，不然会连接上两个 WebSocket
        return;
      }
      this.lockReconnect = true;
      this.reconnectData && clearTimeout(this.reconnectData);
      this.reconnectData = setTimeout(() => {
        this.initWebSocket();
        this.lockReconnect = false;
      }, 5000);
    }, //socket重连
    heatBeat() {
      this.timeoutObj && clearTimeout(this.timeoutObj);
      this.serverTimeoutObj && clearTimeout(this.serverTimeoutObj);
      this.timeoutObj = setTimeout(() => {
        this.websocketsend({ type: "心跳检测" }); //根据后台要求发送
        this.serverTimeoutObj = setTimeout(() => {
          this.websock.close(); //如果  5秒之后我们没有收到 后台返回的心跳检测数据 断开socket，断开后会启动重连机制
        }, 5000);
      }, this.timeout);
    }, //心跳检测
    destroyed() {
      this.lockReconnect = true;
      this.websock.close(); //离开路由之后断开websocket连接
      clearTimeout(this.reconnectData); //离开清除 timeout
      clearTimeout(this.timeoutObj); //离开清除 timeout
      clearTimeout(this.serverTimeoutObj); //离开清除 timeout
    },
    reset() {
      this.websock.send("reset\n");
      //console.log("restart websocket......");
      //this.initWebSocket();
    },
    sendtext() {
      console.log("send data to websocket server");
      this.websock.send("next\n");
    },
  },
};
</script>

<style lang="scss" scoped></style>
