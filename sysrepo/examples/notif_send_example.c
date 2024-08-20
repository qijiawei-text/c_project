/**
 * @file notif_send_example.c
 * @author Michal Vasko <mvasko@cesnet.cz>
 * @brief example of an application that sends a notification
 *
 * @copyright
 * Copyright (c) 2019 CESNET, z.s.p.o.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */
#define _GNU_SOURCE

#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libyang/libyang.h>

#include "sysrepo.h"

int
main(int argc, char **argv)
{
    sr_conn_ctx_t *connection = NULL;
    sr_session_ctx_t *session = NULL;
    int rc = SR_ERR_OK;
    struct lyd_node *tree = NULL, *notif;
    const struct ly_ctx *ctx = NULL;
    const char *path, *node_path = NULL, *node_val;

    if ((argc < 2) || (argc > 4) || (argc == 3)) {
        printf("%s <notification-path> [<node-to-set> <node-value>]\n", argv[0]);
        return EXIT_FAILURE;
    }
    path = argv[1];
    if (argc > 2) {
        node_path = argv[2];
        node_val = argv[3];
    }

    printf("Application will send notification \"%s\".\n\n", path);

    /* turn logging on */
    sr_log_stderr(SR_LL_WRN);

    /* connect to sysrepo */
    rc = sr_connect(0, &connection);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }
    ctx = sr_acquire_context(connection);

    /* start session */
    rc = sr_session_start(connection, SR_DS_RUNNING, &session);
    if (rc != SR_ERR_OK) {
        goto cleanup;
    }

    /* create the notification */
    if (lyd_new_path2(NULL, ctx, path, NULL, 0, 0, 0, &tree, &notif)) {
        printf("Creating notification \"%s\" failed (%s).\n", path, ly_last_logmsg());
        goto cleanup;
    }

    /* add the input value */
    if (node_path) {
        if (lyd_new_path(notif, NULL, node_path, node_val, 0, NULL)) {
            printf("Creating value \"%s\" failed (%s).\n", node_path, ly_last_logmsg());
            goto cleanup;
        }
    }

    /* send the notification */
    rc = sr_notif_send_tree(session, notif, 0, 0);
    if (rc != SR_ERR_OK) {
        printf("Failed to send the notification.\n");
        goto cleanup;
    }

cleanup:
    lyd_free_tree(tree);
    if (ctx) {
        sr_release_context(connection);
    }
    sr_disconnect(connection);
    return rc ? EXIT_FAILURE : EXIT_SUCCESS;
}
