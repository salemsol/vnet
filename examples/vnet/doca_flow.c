#include "doca_flow.h"
#include "gw.h"
#include "doca_log.h"
#include "doca_dpdk.h"
#include "doca_dpdk_priv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DOCA_LOG_MODULE(doca_flow);

struct doca_flow_fwd_tbl {
    const char * name;
    void * handler;
    uint32_t id;
    struct doca_flow_fwd_table_cfg cfg;
};

uint8_t *doca_flow_port_priv_data(struct doca_flow_port *p)
{
    return &p->user_data[0];
}

int doca_flow_init(struct doca_flow_cfg *cfg,struct doca_flow_error *err)
{
    DOCA_LOG_INFO("total sessions = %d\n",cfg->total_sessions);
    if (err) {
        *err = *err;
    }
    doca_dpdk_init(cfg);
    return 0;
}


/**
 * @brief 
 *
 * @param match
 *
 * @return 
 */
struct doca_flow_pipeline_entry *doca_flow_pipeline_add_entry(uint16_t pipe_queue, 
                      struct doca_flow_pipeline *pipeline, struct doca_flow_match *match,
                      struct doca_flow_actions *actions,struct doca_flow_monitor *mon,
                      struct doca_flow_fwd_tbl *fwd, struct doca_flow_error *err)
{
    if(pipeline == NULL || match == NULL || actions == NULL || mon == NULL)
        return NULL;
	pipe_queue = pipe_queue;
	return doca_dpdk_pipe_create_flow(pipeline, match, actions, mon, &fwd->cfg, err);
}

int doca_flow_rm_entry(uint16_t pipe_queue, struct doca_flow_pipeline_entry *entry)
{
    DOCA_LOG_INFO("(pipe %d) HW release id%d",pipe_queue, entry->id);
	// TODO: how to get the port id?
	//doca_dpdk_free_flow(0, entry->pipe_entry);
    free(entry);
    return 0;
}

/**
 * @brief 
 *
 * @param cfg
 * @param port
 *
 * @return 
 */
struct doca_flow_port * doca_flow_port_start(struct doca_flow_port_cfg *cfg, struct doca_flow_error *err)
{
	struct doca_flow_port *port = NULL;

	if (cfg == NULL)
		return NULL;
	switch(cfg->type) {
	case DOCA_FLOW_PORT_DPDK:
		DOCA_LOG_INFO("port is dpdk, matching port id");
		// init all required data sturcures for port.
		break;
	case DOCA_FLOW_PORT_DPDK_BY_ID:
		//TODO: need to parse devargs
		DOCA_LOG_INFO("new doca port type:dpdk port id:%s", cfg->devargs);
		cfg->port_id = atoi(cfg->devargs);
		port = doca_dpdk_port_start(cfg, err);
		break;
	default:
		DOCA_LOG_ERR("unsupported port type");
		err->message = "unsupported port type";
		return NULL;
	}
	return port;
}

/**
 * @brief - close port
 *  release all resources used by port, including all HW rules.
 *
 * @param port
 *
 * @return 
 */
int doca_flow_port_stop(struct doca_flow_port *port)
{
    if (port != NULL) {
       printf("port id = %d stopped\n", port->port_id); 
    }
    return 0;
}

struct doca_flow_pipeline *doca_flow_create_pipe(struct doca_flow_pipeline_cfg *cfg, struct doca_flow_error *err)
{
	if (cfg == NULL)
		return NULL;
	return doca_dpdk_create_pipe(cfg, err);
}

void doca_flow_destroy(uint16_t port_id)
{
	doca_dpdk_destroy(port_id);
}

void doca_flow_dump_pipeline(uint16_t port_id)
{
	doca_dpdk_dump_pipeline(port_id);
}

struct doca_flow_fwd_tbl *doca_flow_create_fwd_tbl(struct doca_flow_fwd_table_cfg *cfg)
{
    static uint32_t fwd_id = 0;
    struct doca_flow_fwd_tbl *tbl = malloc(sizeof(struct doca_flow_fwd_tbl));
    memset(tbl, 0, sizeof(struct doca_flow_fwd_tbl));
    tbl->cfg = *cfg;
    tbl->id = fwd_id++;
    DOCA_LOG_INFO("add fwd tbl");
    return tbl;
}
