<<<<<<< HEAD
/*
 * Copyright (c) 2015, 2017-2019, The Linux Foundation. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
=======
/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2015, 2018, The Linux Foundation. All rights reserved. */
>>>>>>> v4.19.83

#ifndef __QCOM_CLK_ALPHA_PLL_H__
#define __QCOM_CLK_ALPHA_PLL_H__

#include <linux/clk-provider.h>
#include "clk-regmap.h"

<<<<<<< HEAD
struct pll_vco_data {
	unsigned long freq;
	u8 post_div_val;
};

=======
/* Alpha PLL types */
enum {
	CLK_ALPHA_PLL_TYPE_DEFAULT,
	CLK_ALPHA_PLL_TYPE_HUAYRA,
	CLK_ALPHA_PLL_TYPE_BRAMMO,
	CLK_ALPHA_PLL_TYPE_FABIA,
	CLK_ALPHA_PLL_TYPE_MAX,
};

enum {
	PLL_OFF_L_VAL,
	PLL_OFF_ALPHA_VAL,
	PLL_OFF_ALPHA_VAL_U,
	PLL_OFF_USER_CTL,
	PLL_OFF_USER_CTL_U,
	PLL_OFF_CONFIG_CTL,
	PLL_OFF_CONFIG_CTL_U,
	PLL_OFF_TEST_CTL,
	PLL_OFF_TEST_CTL_U,
	PLL_OFF_STATUS,
	PLL_OFF_OPMODE,
	PLL_OFF_FRAC,
	PLL_OFF_MAX_REGS
};

extern const u8 clk_alpha_pll_regs[CLK_ALPHA_PLL_TYPE_MAX][PLL_OFF_MAX_REGS];

>>>>>>> v4.19.83
struct pll_vco {
	unsigned long min_freq;
	unsigned long max_freq;
	u32 val;
};

enum pll_type {
	ALPHA_PLL,
	TRION_PLL,
	REGERA_PLL,
	FABIA_PLL,
	AGERA_PLL,
	LUCID_PLL,
};

/**
 * struct clk_alpha_pll - phase locked loop (PLL)
 * @offset: base address of registers
 * @soft_vote: soft voting variable for multiple PLL software instances
 * @soft_vote_mask: soft voting mask for multiple PLL software instances
 * @inited: flag that's set when the PLL is initialized
 * @vco_table: array of VCO settings
<<<<<<< HEAD
 * @vco_data: array of VCO data settings like post div
=======
 * @regs: alpha pll register map (see @clk_alpha_pll_regs)
>>>>>>> v4.19.83
 * @clkr: regmap clock handle
 */
struct clk_alpha_pll {
	u32 offset;
<<<<<<< HEAD
	struct alpha_pll_config *config;
	bool inited;

	u32 *soft_vote;
	u32 soft_vote_mask;
	/* Soft voting values */
#define PLL_SOFT_VOTE_PRIMARY	BIT(0)
#define PLL_SOFT_VOTE_CPU	BIT(1)
#define PLL_SOFT_VOTE_AUX	BIT(2)
=======
	const u8 *regs;
>>>>>>> v4.19.83

	const struct pll_vco *vco_table;
	size_t num_vco;

	const struct pll_vco_data *vco_data;
	size_t num_vco_data;

#define SUPPORTS_OFFLINE_REQ	BIT(0)
#define SUPPORTS_FSM_MODE	BIT(2)
<<<<<<< HEAD
	/*
	 * Some PLLs support dynamically updating their rate without disabling
	 * the PLL first. Set this flag to enable this support.
	 */
#define SUPPORTS_DYNAMIC_UPDATE	BIT(3)
#define SUPPORTS_SLEW		BIT(4)
	/* Associated with soft_vote for multiple PLL software instances */
#define SUPPORTS_FSM_VOTE	BIT(5)
#define SUPPORTS_NO_SLEW	BIT(6)
#define SUPPORTS_NO_PLL_LATCH	BIT(7)
=======
#define SUPPORTS_DYNAMIC_UPDATE	BIT(3)
>>>>>>> v4.19.83
	u8 flags;

	struct clk_regmap clkr;
	enum pll_type type;
	unsigned long min_supported_freq;
};

enum postdiv_type {
	POSTDIV_EVEN,
	POSTDIV_ODD,
};

/**
 * struct clk_alpha_pll_postdiv - phase locked loop (PLL) post-divider
 * @offset: base address of registers
 * @regs: alpha pll register map (see @clk_alpha_pll_regs)
 * @width: width of post-divider
<<<<<<< HEAD
 * @post_div_shift: shift to differentiate between odd and even post-divider
 * @post_div_table: table with PLL odd and even post-divider settings
 * @num_post_div: Number of PLL post-divider settings
=======
 * @post_div_shift: shift to differentiate between odd & even post-divider
 * @post_div_table: table with PLL odd and even post-divider settings
 * @num_post_div: Number of PLL post-divider settings
 *
>>>>>>> v4.19.83
 * @clkr: regmap clock handle
 */
struct clk_alpha_pll_postdiv {
	u32 offset;
	u8 width;
<<<<<<< HEAD
	int post_div_shift;
	const struct clk_div_table *post_div_table;
	size_t num_post_div;
	struct clk_regmap clkr;
	enum pll_type type;
	enum postdiv_type postdiv;
=======
	const u8 *regs;

	struct clk_regmap clkr;
	int post_div_shift;
	const struct clk_div_table *post_div_table;
	size_t num_post_div;
>>>>>>> v4.19.83
};

struct alpha_pll_config {
	u32 l;
	u32 frac;
	u32 alpha;
<<<<<<< HEAD
	u32 alpha_u;
	u32 user_ctl_val;
	u32 user_ctl_hi_val;
	u32 user_ctl_hi1_val;
=======
	u32 alpha_hi;
>>>>>>> v4.19.83
	u32 config_ctl_val;
	u32 config_ctl_hi_val;
	u32 config_ctl_hi1_val;
	u32 test_ctl_val;
	u32 test_ctl_mask;
	u32 test_ctl_hi_val;
	u32 test_ctl_hi_mask;
	u32 test_ctl_hi1_val;
	u32 main_output_mask;
	u32 aux_output_mask;
	u32 aux2_output_mask;
	u32 early_output_mask;
	u32 alpha_en_mask;
	u32 alpha_mode_mask;
	u32 pre_div_val;
	u32 pre_div_mask;
	u32 post_div_val;
	u32 post_div_mask;
	u32 vco_val;
	u32 vco_mask;
	u32 alpha_en_mask;
};

extern const struct clk_ops clk_alpha_pll_ops;
extern const struct clk_ops clk_alpha_pll_hwfsm_ops;
extern const struct clk_ops clk_alpha_pll_postdiv_ops;
<<<<<<< HEAD
extern const struct clk_ops clk_trion_pll_ops;
extern const struct clk_ops clk_trion_fixed_pll_ops;
extern const struct clk_ops clk_trion_pll_postdiv_ops;
extern const struct clk_ops clk_regera_pll_ops;
extern const struct clk_ops clk_alpha_pll_slew_ops;
extern const struct clk_ops clk_pll_sleep_vote_ops;
extern const struct clk_ops clk_fabia_pll_ops;
extern const struct clk_ops clk_fabia_fixed_pll_ops;
extern const struct clk_ops clk_generic_pll_postdiv_ops;
extern const struct clk_ops clk_agera_pll_ops;
extern const struct clk_ops clk_alpha_pll_lucid_ops;
extern const struct clk_ops clk_alpha_pll_fixed_lucid_ops;
extern const struct clk_ops clk_alpha_pll_postdiv_lucid_ops;
=======
extern const struct clk_ops clk_alpha_pll_huayra_ops;
extern const struct clk_ops clk_alpha_pll_postdiv_ro_ops;

extern const struct clk_ops clk_alpha_pll_fabia_ops;
extern const struct clk_ops clk_alpha_pll_fixed_fabia_ops;
extern const struct clk_ops clk_alpha_pll_postdiv_fabia_ops;
>>>>>>> v4.19.83

int clk_alpha_pll_configure(struct clk_alpha_pll *pll, struct regmap *regmap,
			     const struct alpha_pll_config *config);
<<<<<<< HEAD
int clk_trion_pll_configure(struct clk_alpha_pll *pll, struct regmap *regmap,
				const struct alpha_pll_config *config);
int clk_regera_pll_configure(struct clk_alpha_pll *pll, struct regmap *regmap,
				const struct alpha_pll_config *config);
int clk_fabia_pll_configure(struct clk_alpha_pll *pll, struct regmap *regmap,
				const struct alpha_pll_config *config);
int clk_agera_pll_configure(struct clk_alpha_pll *pll, struct regmap *regmap,
				const struct alpha_pll_config *config);
int clk_lucid_pll_configure(struct clk_alpha_pll *pll, struct regmap *regmap,
				const struct alpha_pll_config *config);
=======
void clk_fabia_pll_configure(struct clk_alpha_pll *pll, struct regmap *regmap,
				const struct alpha_pll_config *config);

>>>>>>> v4.19.83
#endif
