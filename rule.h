/*
 * Copyright (c) 2016, YAO Wei <njustyw at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* inclusion guard */
#ifndef __RULE_H__
#define __RULE_H__

#include "list.h"

union match_payload {
	char host[256];
};

struct hit_record {
	uint32_t id;
	uint32_t saddr;
	uint32_t daddr;
	uint16_t sport;
	uint16_t dport;
	uint8_t smac[6];
	uint8_t dmac[6];
	uint64_t time;
	struct list_head list;
};

struct rule {
	uint32_t id;
	char human_protocol[16];
	char human_saddr[16];
	char human_daddr[16];
	uint16_t sport;
	uint16_t dport;
	char human_action[16];
	union match_payload condition;
	char action_params[4][256];
	uint8_t protocol;
	uint8_t master_protocol;
	uint8_t action;
	uint8_t used;
	uint32_t saved_hits;
	uint32_t saddr0, saddr1;
	uint32_t daddr0, daddr1;
	uint64_t hits;
	struct list_head list;
	struct list_head hit_head;
};

#define T01_ACTION_REDIRECT 		1
#define T01_ACTION_REJECT		2
#define T01_ACTION_CONFUSE		3

int load_rules(const char *filename, void *ndpi_mask);

int save_rules(const char *filename);

int save_rules_background(const char *filename);

void destroy_rules();

struct rule *match_rule_from_packet(void *flow, void *packet);

int add_one_hit_record(struct rule *r, uint64_t time, uint32_t saddr,
		       uint32_t daddr, uint16_t sport, uint16_t dport,
		       uint8_t smac[], uint8_t dmac[]);

void release_buffer(char **out);
int get_ruleids(char **out, size_t *out_len);
int get_rule(uint32_t id, char **out, size_t *out_len);
int get_rules(uint32_t *id, size_t len, char **out, size_t *out_len);
int get_hits(uint32_t rule_id, int offset, int limit, char **out,
	     size_t *out_len);
int delete_rule(uint32_t id);
int update_rule(uint32_t id, const char *body, int body_len);
int create_rule(const char *body, int body_len, char **out, size_t *out_len);

#endif /* __RULE_H__ */
