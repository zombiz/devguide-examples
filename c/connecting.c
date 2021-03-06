#include <libcouchbase/couchbase.h>
#include <string.h>
#include <stdlib.h>

static void
die(lcb_error_t rc, const char *msg)
{
    fprintf(stderr, "%s failed. (0x%x, %s)\n", msg, rc, lcb_strerror(NULL, rc));
    exit(EXIT_FAILURE);
}

int
main(int argc, char **argv)
{
    lcb_t instance;
    struct lcb_create_st cropts;
    lcb_error_t rc;

    memset(&cropts, 0, sizeof cropts);
    cropts.version = 3;
    cropts.v.v3.connstr = "couchbase://10.0.0.31/default";

    rc = lcb_create(&instance, &cropts);
    if (rc != LCB_SUCCESS) {
        die(rc, "Creating instance");
    }

    rc = lcb_connect(instance);
    if (rc != LCB_SUCCESS) {
        die(rc, "Connection scheduling");
    }

    /* This function required to actually schedule the operations on the network */
    lcb_wait(instance);

    /* Determines if the bootstrap/connection succeeded */
    rc = lcb_get_bootstrap_status(instance);
    if (rc != LCB_SUCCESS) {
        die(rc, "Connection bootstraping");
    } else {
        printf("Connection succeeded. Cluster has %d nodes\n",
            lcb_get_num_nodes(instance));
    }

    lcb_destroy(instance);
    return 0;
}
