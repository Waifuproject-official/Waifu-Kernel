<<<<<<< HEAD
/* Copyright (c) 2016-2017, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2016-2018, The Linux Foundation. All rights reserved.
>>>>>>> v4.19.83
 */

#ifndef __SOC_QCOM_TCS_H__
#define __SOC_QCOM_TCS_H__

#define MAX_RPMH_PAYLOAD	16

<<<<<<< HEAD
struct tcs_cmd {
	u32 addr;		/* slv_id:18:16 | offset:0:15 */
	u32 data;		/* data for resource (or read response) */
	bool complete;		/* wait for completion before sending next */
};

enum rpmh_state {
	RPMH_SLEEP_STATE,	/* Sleep */
	RPMH_WAKE_ONLY_STATE,	/* Wake only */
	RPMH_ACTIVE_ONLY_STATE,	/* Active only (= AMC) */
	RPMH_AWAKE_STATE,	/* Use Wake TCS for Wake & Active (AMC = 0) */
};

struct tcs_mbox_msg {
	enum rpmh_state state;	/* request state */
	bool is_complete;	/* wait for resp from accelerator */
	bool is_read;		/* expecting a response from RPMH */
	bool is_control;	/* private control messages */
	bool invalidate;	/* invalidate sleep/wake commands */
	u32 num_payload;	/* Limited to MAX_RPMH_PAYLOAD in one msg */
	struct tcs_cmd *payload;/* array of tcs_cmds */
=======
/**
 * rpmh_state: state for the request
 *
 * RPMH_SLEEP_STATE:       State of the resource when the processor subsystem
 *                         is powered down. There is no client using the
 *                         resource actively.
 * RPMH_WAKE_ONLY_STATE:   Resume resource state to the value previously
 *                         requested before the processor was powered down.
 * RPMH_ACTIVE_ONLY_STATE: Active or AMC mode requests. Resource state
 *                         is aggregated immediately.
 */
enum rpmh_state {
	RPMH_SLEEP_STATE,
	RPMH_WAKE_ONLY_STATE,
	RPMH_ACTIVE_ONLY_STATE,
};

/**
 * struct tcs_cmd: an individual request to RPMH.
 *
 * @addr: the address of the resource slv_id:18:16 | offset:0:15
 * @data: the resource state request
 * @wait: wait for this request to be complete before sending the next
 */
struct tcs_cmd {
	u32 addr;
	u32 data;
	u32 wait;
};

/**
 * struct tcs_request: A set of tcs_cmds sent together in a TCS
 *
 * @state:          state for the request.
 * @wait_for_compl: wait until we get a response from the h/w accelerator
 * @num_cmds:       the number of @cmds in this request
 * @cmds:           an array of tcs_cmds
 */
struct tcs_request {
	enum rpmh_state state;
	u32 wait_for_compl;
	u32 num_cmds;
	struct tcs_cmd *cmds;
>>>>>>> v4.19.83
};

#endif /* __SOC_QCOM_TCS_H__ */
