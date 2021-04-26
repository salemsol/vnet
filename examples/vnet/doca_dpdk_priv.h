#ifndef _DOCA_DPDK_PRIV_H_
#define _DOCA_DPDK_PRIV_H_

#include "doca_dpdk.h"
#include "doca_flow.h"

struct doca_flow_fwd_tbl {
	const char *name;
	void *handler;
	uint32_t id;
	struct doca_flow_fwd cfg;
};


struct doca_flow_pipeline_entry {
	LIST_ENTRY(doca_flow_pipeline_entry) next;
	int id;
	uint32_t meter_id;
	void *pipe_entry;
};

#define TMP_BUFF 128
struct doca_flow_pipeline {
	LIST_ENTRY(doca_flow_pipeline) next;
	char name[TMP_BUFF];
	void *handler;
	uint32_t id;
	uint32_t pipe_entry_id;
	uint32_t nb_pipe_entrys;
	struct doca_flow_fwd fwd_tbl;
        struct doca_dpdk_pipeline flow;
	rte_spinlock_t entry_lock;
	LIST_HEAD(, doca_flow_pipeline_entry) entry_list;
};

struct doca_flow_port {
	uint32_t port_id;
	int idx;

	rte_spinlock_t pipe_lock;
	LIST_HEAD(, doca_flow_pipeline) pipe_list;
	uint8_t user_data[0];
};

#endif
