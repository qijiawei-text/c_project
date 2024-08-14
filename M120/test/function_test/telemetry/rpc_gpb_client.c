#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "sysrepo.h"
#include "sysrepo/values.h"

int
main(int argc, char **argv)
{
    sr_conn_ctx_t *connection = NULL;
    sr_session_ctx_t *session = NULL;
    int rc = SR_ERR_OK;
    sr_val_t *input = NULL, *output = NULL;
    size_t output_cnt = 0, input_cnt = 0;

    if (argc != 3) {
        /* run as a RPC handler */
        printf("Usage: <IP> <port>");
        exit(1);
    }

    /* connect to sysrepo */
    rc = sr_connect("example_application", SR_CONN_DEFAULT, &connection);
    if (SR_ERR_OK != rc) {
        fprintf(stderr, "Error by sr_connect: %s\n", sr_strerror(rc));
        goto cleanup;
    }

    /* start session */
    rc = sr_session_start(connection, SR_DS_RUNNING, SR_SESS_DEFAULT, &session);
    if (SR_ERR_OK != rc) {
        fprintf(stderr, "Error by sr_session_start: %s\n", sr_strerror(rc));
        goto cleanup;
    }

    /* run as a RPC caller */
    printf("Executing RPC 'refresh-subscription' of 'telemetry-subscription':\n");
    /* allocate input values */
    input_cnt = 3;
    rc = sr_new_values(input_cnt, &input);
    if (SR_ERR_OK != rc) {
        goto cleanup;
    }

    /* set 'input/destination-address' leaf */
    rc = sr_val_set_xpath(&input[0], "/telemetry-subscription:refresh-subscription/destination-address");
    if (SR_ERR_OK != rc) {
        return rc;
    }
    input[0].type = SR_STRING_T;
    input[0].data.string_val = argv[1];

    /* set 'input/destination-port' leaf */
    rc = sr_val_set_xpath(&input[1], "/telemetry-subscription:refresh-subscription/destination-port");
    if (SR_ERR_OK != rc) {
        goto cleanup;
    }
    input[1].type = SR_UINT16_T;
    input[1].data.uint16_val = atoi(argv[2]);

    /* set 'input/expiry-seconds' leaf */
    rc = sr_val_set_xpath(&input[2], "/telemetry-subscription:refresh-subscription/expiry-seconds");
    if (SR_ERR_OK != rc) {
        return rc;
    }
    input[2].type = SR_UINT32_T;
    input[2].data.uint32_val = 200;

    printf(">>> RPC Input:\n\n");
    for (size_t i = 0; i < input_cnt; ++i) {
        sr_print_val(&input[i]);
    }
    printf("\n");

    /* execute RPC */
    rc = sr_rpc_send(session, "/telemetry-subscription:refresh-subscription", input, input_cnt, &output, &output_cnt);
    if (SR_ERR_OK != rc) {
        goto cleanup;
    }

    /* print output values */
    printf("\n>>> Received an RPC response:\n\n");
    for (size_t i = 0; i < output_cnt; ++i) {
        sr_print_val(output + i);
    }
    printf("\n");

    /* don't forget to de-allocate the output values */
    sr_free_values(output, output_cnt);

cleanup:
    if (NULL != session) {
        sr_session_stop(session);
    }
    if (NULL != connection) {
        sr_disconnect(connection);
    }
    return rc;
}
