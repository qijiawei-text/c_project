#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sysrepo.h"
#include "sysrepo/values.h"
#include "pbc.h"
#include "readfile.h"
#define MAXSIZE 4096

int
main(int argc, char *argv[])
{
    int rc = SR_ERR_OK;
    int sock;
    struct sockaddr_in servAddr;
    struct sockaddr_in clntAddr;
    unsigned int cliAddrLen;
    uint8_t buffer[MAXSIZE];
    unsigned short servPort;
    int recvMsgSize;
    servPort = 1500;
    int count = 0;
    int iSockopt = 1;
    struct pbc_slice sl;

    struct pbc_env *env = pbc_new();
    struct pbc_slice slice;

    struct pbc_rmessage *stream = NULL;

    /* read_file("../../../../../libs/util-lib/optical_top.pb", &slice); */
    read_file("optical_top.pb", &slice);
    if (slice.buffer == NULL) {
        printf("Error[%d] by read_file optical_top: %s", __LINE__, sr_strerror(rc));
        rc = SR_ERR_INTERNAL;
        return rc;
    }
    rc = pbc_register(env, &slice);
    free(slice.buffer);
    if (rc != SR_ERR_OK) {
        printf("Error[%d] by pbc_register optical_top: %s", __LINE__, sr_strerror(rc));
        rc = SR_ERR_INTERNAL;
        return rc;
    }

    /* read_file("../../../../../libs/util-lib/optical_port_top.pb", &slice); */
    read_file("optical_port_top.pb", &slice);
    if (slice.buffer == NULL) {
        printf("Error[%d]  by read_file optical_port_top: %s", __LINE__, sr_strerror(rc));
        rc = SR_ERR_INTERNAL;
        return rc;
    }
    rc = pbc_register(env, &slice);
    free(slice.buffer);
    if (rc != SR_ERR_OK) {
        printf("Error[%d]  by pbc_register optical_port_top: %s", __LINE__, sr_strerror(rc));
        rc = SR_ERR_INTERNAL;
        return rc;
    }

    /* read_file("../../../../../libs/util-lib/optical_edfa.pb", &slice); */
    read_file("optical_edfa.pb", &slice);
    if (slice.buffer == NULL) {
        printf("Error[%d]  by read_file optical_edfa: %s", __LINE__, sr_strerror(rc));
        rc = SR_ERR_INTERNAL;
        return rc;
    }
    rc = pbc_register(env, &slice);
    free(slice.buffer);
    if (rc != SR_ERR_OK) {
        printf("Error[%d]  by pbc_register optical_edfa: %s", __LINE__, sr_strerror(rc));
        rc = SR_ERR_INTERNAL;
        return rc;
    }

    /* read_file("../../../../../libs/util-lib/optical_voa.pb", &slice); */
    read_file("optical_voa.pb", &slice);
    if (slice.buffer == NULL) {
        printf("Error[%d]  by read_file optical_voa: %s", __LINE__, sr_strerror(rc));
        rc = SR_ERR_INTERNAL;
        return rc;
    }
    rc = pbc_register(env, &slice);
    free(slice.buffer);
    if (rc != SR_ERR_OK) {
        printf("Error[%d]  by pbc_register optical_voa: %s", __LINE__, sr_strerror(rc));
        rc = SR_ERR_INTERNAL;
        return rc;
    }

    /* read_file("../../../../../libs/util-lib/optical_port_managed.pb", &slice); */
    read_file("optical_port_managed.pb", &slice);
    if (slice.buffer == NULL) {
        printf("Error[%d]  by read_file optical_port_managed: %s", __LINE__, sr_strerror(rc));
        rc = SR_ERR_INTERNAL;
        return rc;
    }
    rc = pbc_register(env, &slice);
    free(slice.buffer);
    if (rc != SR_ERR_OK) {
        printf("Error[%d]  by pbc_register optical_port_managed: %s", __LINE__, sr_strerror(rc));
        rc = SR_ERR_INTERNAL;
        return rc;
    }

    if (argc != 2) {
        /* run as a RPC handler */
        printf("Usage: <port> \n");
        exit(1);
    }

    servPort = atoi(argv[1]);

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("socket() failed.\n");
    }

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &iSockopt, sizeof(iSockopt)) == -1) {
        printf("setsockopt() failed. \n");
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(servPort);
    /* Bind to the local address */
    if ((bind(sock, (struct sockaddr *)&servAddr, sizeof(servAddr))) < 0) {
        printf("bind() failed.\n");
    }

    for (;; ) {
        /* Set the size of the in-out parameter */
        cliAddrLen = sizeof(clntAddr);
        /* Block until receive message from a client */
        if ((recvMsgSize = recvfrom(sock, buffer, MAXSIZE, 0, (struct sockaddr *)&clntAddr, &cliAddrLen)) < 0) {
            printf("recvfrom() failed.\n");
        }

        printf("Message: %d\n", count);
        sl.buffer = buffer;
        sl.len = recvMsgSize;
        stream = pbc_rmessage_new(env, "OpticalStream", &sl);
        printf("system_id = %s\n", pbc_rmessage_string(stream, "system_id", 0, NULL));

        long long unsigned int timestamp;
        uint32_t low, high;
        low = pbc_rmessage_integer(stream, "timestamp", 0, &high);
        timestamp = ((uint64_t)high) << 32 | (uint64_t)low;
        printf("timestamp = %llu\n", timestamp);
        printf("sequence_number = %d\n", pbc_rmessage_integer(stream, "sequence_number", 0, NULL));
        printf("version_major = %d\n", pbc_rmessage_integer(stream, "version_major", 0, NULL));
        printf("version_minor = %d\n", pbc_rmessage_integer(stream, "version_minor", 0, NULL));

        struct pbc_rmessage *data = pbc_rmessage_message(stream, "optical_telemetry_data", 0);
        if (NULL == data) {
            printf("Error[%d]  data is null", __LINE__);
            continue;
        }

        struct pbc_rmessage *voa = pbc_rmessage_message(data, "voa", 0);
        if (NULL == voa) {
            printf("Error[%d]  voa is null", __LINE__);
            continue;
        }
        struct pbc_rmessage *voa_info;

        int voa_info_n = pbc_rmessage_size(voa, "voa_info");

        for (int i = 0; i < voa_info_n; i++) {
            printf("voa Entry: %d\n", i);
            voa_info = pbc_rmessage_message(voa, "voa_info", i);
            if (NULL == voa_info) {
                printf("Error[%d]  voa_info is null", __LINE__);
                continue;
            }
            printf("voa_id = %d\n", pbc_rmessage_integer(voa_info, "voa_id", i, NULL));
            printf("measured_atten = %f\n", pbc_rmessage_real(voa_info, "measured_atten", i));
            printf("target_atten = %f\n", pbc_rmessage_real(voa_info, "target_atten", i));
            printf("pwr_in = %f\n", pbc_rmessage_real(voa_info, "pwr_in", i));
            printf("pwr_out = %f\n", pbc_rmessage_real(voa_info, "pwr_out", i));
            printf("\n");
        }

        struct pbc_rmessage *edfa = pbc_rmessage_message(data, "edfa", 0);
        struct pbc_rmessage *edfa_info;

        int edfa_info_n = pbc_rmessage_size(edfa, "edfa_info");

        for (int i = 0; i < edfa_info_n; i++) {
            printf("edfa Entry: %d\n", i);
            edfa_info = pbc_rmessage_message(edfa, "edfa_info", i);
            if (NULL == edfa_info) {
                printf("Error[%d]  edfa_info is null", __LINE__);
                continue;
            }

            printf("Received: edfa id = %d\n", pbc_rmessage_integer(edfa_info, "edfa_id", i, NULL));
            printf("Received: edfa maint_state = %d\n", pbc_rmessage_integer(edfa_info, "maint_state", i, NULL));
            printf("Received: edfa oper_state = %d\n", pbc_rmessage_integer(edfa_info, "oper_state", i, NULL));
            printf("Received: edfa ctrl_mode = %d\n", pbc_rmessage_integer(edfa_info, "ctrl_mode", i, NULL));
            printf("Received: edfa gain_switch_mode = %d\n", pbc_rmessage_integer(edfa_info, "gain_switch_mode", i, NULL));
            printf("Received: edfa target_gain = %f\n", pbc_rmessage_real(edfa_info, "target_gain", i));
            printf("Received: edfa target_gain_tilt = %f\n", pbc_rmessage_real(edfa_info, "target_gain_tilt", i));
            printf("Received: edfa pwr_in = %f\n", pbc_rmessage_real(edfa_info, "pwr_in", i));
            printf("Received: edfa pwr_out = %f\n", pbc_rmessage_real(edfa_info, "pwr_out", i));
            printf("Received: edfa measured_gain = %f\n", pbc_rmessage_real(edfa_info, "measured_gain", i));
            printf("\n");
        }

        struct pbc_rmessage *optical_port = pbc_rmessage_message(data, "optical_port", 0);
        struct pbc_rmessage *managed_port;
        int managed_port_n = pbc_rmessage_size(optical_port, "managed_port");

        for (int i = 0; i < managed_port_n; i++) {
            printf("managed_port Entry: %d\n", i + 1);
            managed_port = pbc_rmessage_message(optical_port, "managed_port", i);
            if (NULL == managed_port) {
                printf("Error[%d]  managed_port is null", __LINE__);
                continue;
            }

            printf("Received: port id = %d\n", pbc_rmessage_integer(managed_port, "port_id", i, NULL));
            printf("Received: maint_state = %d\n", pbc_rmessage_integer(managed_port, "maint_state", i, NULL));
            printf("Received: oper_state = %d\n", pbc_rmessage_integer(managed_port, "oper_state", i, NULL));

            struct pbc_rmessage *port_data = pbc_rmessage_message(managed_port, "port_data", 0);
            if (NULL == port_data) {
                printf("Error[%d]  port_data is null", __LINE__);
                continue;
            }
            printf("Received: total_pwr_in = %f\n", pbc_rmessage_real(port_data, "total_pwr_in", 0));
            printf("Received: total_pwr_out = %f\n", pbc_rmessage_real(port_data, "total_pwr_out", 0));
            printf("Received: oms pwr_in = %f\n", pbc_rmessage_real(port_data, "oms_pwr_in", i));
            printf("Received: oms pwr_out = %f\n", pbc_rmessage_real(port_data, "oms_pwr_out", i));
            printf("Received: oms backreflection_pwr = %f\n", pbc_rmessage_real(port_data, "oms_backreflection_pwr", i));
            printf("Received: oms return_loss = %f\n", pbc_rmessage_real(port_data, "oms_return_loss", i));
            
            struct pbc_rmessage *osc_info = pbc_rmessage_message(port_data, "osc_info", 0);
            if (NULL == osc_info) {
                printf("Error[%d]  osc_info is null", __LINE__);
                continue;
            }
            printf("Received: osc_wavelength = %f\n", pbc_rmessage_real(osc_info, "osc_wavelength", 0));
            printf("Received: osc_pwr_in = %f\n", pbc_rmessage_real(osc_info, "osc_pwr_in", 0));
            printf("Received: osc_pwr_out = %f\n", pbc_rmessage_real(osc_info, "osc_pwr_out", 0));

            printf("\n");
        }

        pbc_rmessage_delete(stream);
        count++;
    }

}
