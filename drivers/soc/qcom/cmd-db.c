<<<<<<< HEAD
/* Copyright (c) 2016-2018, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <soc/qcom/cmd-db.h>

#define RESOURCE_ID_LEN 8
#define NUM_PRIORITY  2
#define MAX_SLV_ID 8
#define CMD_DB_MAGIC 0x0C0330DBUL
#define SLAVE_ID_MASK 0x7
#define SLAVE_ID_SHIFT 16
#define CMD_DB_STANDALONE_MASK BIT(0)

struct entry_header {
	uint64_t res_id;
	u32 priority[NUM_PRIORITY];
	u32 addr;
	u16 len;
	u16 offset;
};

struct rsc_hdr {
	u16  slv_id;
	u16  header_offset;	/* Entry header offset from data  */
	u16  data_offset;	/* Entry offset for data location */
	u16  cnt;	/* Number of entries for HW type */
	u16  version;	/* MSB is Major and LSB is Minor version
			 * identifies the HW type of Aux Data
			 */
	u16 reserved[3];
};

struct cmd_db_header {
	u32 version;
	u32 magic_num;
	struct rsc_hdr header[MAX_SLV_ID];
	u32 check_sum;
	u32 reserved;
	u8 data[];
};

struct cmd_db_entry {
	const char resource_id[RESOURCE_ID_LEN + 1]; /* Unique id per entry */
	const u32 addr; /* TCS Addr Slave ID + Offset address */
	const u32 priority[NUM_PRIORITY]; /* Bitmask for DRV IDs */
	u32       len;                                 /* Aux data len */
	u16       version;
	u8        data[];
};

/* CMD DB QUERY TYPES */
enum cmd_db_query_type {
	CMD_DB_QUERY_RES_ID = 0,
	CMD_DB_QUERY_ADDRESS,
	CMD_DB_QUERY_INVALID,
	CMD_DB_QUERY_MAX = 0x7ffffff,
};

static void __iomem *start_addr;
static struct cmd_db_header *cmd_db_header;
static int cmd_db_status = -EPROBE_DEFER;

static u64 cmd_db_get_u64_id(const char *id)
{
	uint64_t rsc_id = 0;
	uint8_t *ch  = (uint8_t *)&rsc_id;
	int i;

	for (i = 0; ((i < sizeof(rsc_id)) && id[i]); i++)
		ch[i] = id[i];

	return rsc_id;
}

static int cmd_db_get_header(u64 query, struct entry_header *eh,
		struct rsc_hdr *rh, bool use_addr)
{
	struct rsc_hdr *rsc_hdr;
	int i, j;

	if (!cmd_db_header)
		return -EPROBE_DEFER;
=======
/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright (c) 2016-2018, The Linux Foundation. All rights reserved. */

#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/of_reserved_mem.h>
#include <linux/platform_device.h>
#include <linux/types.h>

#include <soc/qcom/cmd-db.h>

#define NUM_PRIORITY		2
#define MAX_SLV_ID		8
#define SLAVE_ID_MASK		0x7
#define SLAVE_ID_SHIFT		16

/**
 * struct entry_header: header for each entry in cmddb
 *
 * @id: resource's identifier
 * @priority: unused
 * @addr: the address of the resource
 * @len: length of the data
 * @offset: offset from :@data_offset, start of the data
 */
struct entry_header {
	u8 id[8];
	__le32 priority[NUM_PRIORITY];
	__le32 addr;
	__le16 len;
	__le16 offset;
};

/**
 * struct rsc_hdr: resource header information
 *
 * @slv_id: id for the resource
 * @header_offset: entry's header at offset from the end of the cmd_db_header
 * @data_offset: entry's data at offset from the end of the cmd_db_header
 * @cnt: number of entries for HW type
 * @version: MSB is major, LSB is minor
 * @reserved: reserved for future use.
 */
struct rsc_hdr {
	__le16 slv_id;
	__le16 header_offset;
	__le16 data_offset;
	__le16 cnt;
	__le16 version;
	__le16 reserved[3];
};

/**
 * struct cmd_db_header: The DB header information
 *
 * @version: The cmd db version
 * @magic: constant expected in the database
 * @header: array of resources
 * @checksum: checksum for the header. Unused.
 * @reserved: reserved memory
 * @data: driver specific data
 */
struct cmd_db_header {
	__le32 version;
	u8 magic[4];
	struct rsc_hdr header[MAX_SLV_ID];
	__le32 checksum;
	__le32 reserved;
	u8 data[];
};

/**
 * DOC: Description of the Command DB database.
 *
 * At the start of the command DB memory is the cmd_db_header structure.
 * The cmd_db_header holds the version, checksum, magic key as well as an
 * array for header for each slave (depicted by the rsc_header). Each h/w
 * based accelerator is a 'slave' (shared resource) and has slave id indicating
 * the type of accelerator. The rsc_header is the header for such individual
 * slaves of a given type. The entries for each of these slaves begin at the
 * rsc_hdr.header_offset. In addition each slave could have auxiliary data
 * that may be needed by the driver. The data for the slave starts at the
 * entry_header.offset to the location pointed to by the rsc_hdr.data_offset.
 *
 * Drivers have a stringified key to a slave/resource. They can query the slave
 * information and get the slave id and the auxiliary data and the length of the
 * data. Using this information, they can format the request to be sent to the
 * h/w accelerator and request a resource state.
 */

static const u8 CMD_DB_MAGIC[] = { 0xdb, 0x30, 0x03, 0x0c };

static bool cmd_db_magic_matches(const struct cmd_db_header *header)
{
	const u8 *magic = header->magic;

	return memcmp(magic, CMD_DB_MAGIC, ARRAY_SIZE(CMD_DB_MAGIC)) == 0;
}

static struct cmd_db_header *cmd_db_header;


static inline void *rsc_to_entry_header(struct rsc_hdr *hdr)
{
	u16 offset = le16_to_cpu(hdr->header_offset);

	return cmd_db_header->data + offset;
}

static inline void *
rsc_offset(struct rsc_hdr *hdr, struct entry_header *ent)
{
	u16 offset = le16_to_cpu(hdr->data_offset);
	u16 loffset = le16_to_cpu(ent->offset);

	return cmd_db_header->data + offset + loffset;
}

/**
 * cmd_db_ready - Indicates if command DB is available
 *
 * Return: 0 on success, errno otherwise
 */
int cmd_db_ready(void)
{
	if (cmd_db_header == NULL)
		return -EPROBE_DEFER;
	else if (!cmd_db_magic_matches(cmd_db_header))
		return -EINVAL;

	return 0;
}
EXPORT_SYMBOL(cmd_db_ready);

static int cmd_db_get_header(const char *id, struct entry_header *eh,
			     struct rsc_hdr *rh)
{
	struct rsc_hdr *rsc_hdr;
	struct entry_header *ent;
	int ret, i, j;
	u8 query[8];

	ret = cmd_db_ready();
	if (ret)
		return ret;
>>>>>>> v4.19.83

	if (!eh || !rh)
		return -EINVAL;

<<<<<<< HEAD
	rsc_hdr = &cmd_db_header->header[0];

	for (i = 0; i < MAX_SLV_ID ; i++, rsc_hdr++) {
		struct entry_header *ent;

		if (!rsc_hdr->slv_id)
			break;

		ent = (struct entry_header *)(start_addr
				+ sizeof(*cmd_db_header)
				+ rsc_hdr->header_offset);

		for (j = 0; j < rsc_hdr->cnt; j++, ent++) {
			if (use_addr) {
				if (ent->addr == (u32)(query))
					break;
			} else if (ent->res_id == query)
				break;
		}

		if (j < rsc_hdr->cnt) {
			memcpy(eh, ent, sizeof(*ent));
			memcpy(rh, &cmd_db_header->header[i], sizeof(*rh));
			return 0;
		}
	}
	return -ENODEV;
}

static int cmd_db_get_header_by_addr(u32 addr,
		struct entry_header *ent_hdr,
		struct rsc_hdr *rsc_hdr)
{
	return cmd_db_get_header((u64)addr, ent_hdr, rsc_hdr, true);
}

static int cmd_db_get_header_by_rsc_id(const char *resource_id,
		struct entry_header *ent_hdr,
		struct rsc_hdr *rsc_hdr)
{
	u64 rsc_id = cmd_db_get_u64_id(resource_id);

	return cmd_db_get_header(rsc_id, ent_hdr, rsc_hdr, false);
}

u32 cmd_db_get_addr(const char *resource_id)
=======
	/* Pad out query string to same length as in DB */
	strncpy(query, id, sizeof(query));

	for (i = 0; i < MAX_SLV_ID; i++) {
		rsc_hdr = &cmd_db_header->header[i];
		if (!rsc_hdr->slv_id)
			break;

		ent = rsc_to_entry_header(rsc_hdr);
		for (j = 0; j < le16_to_cpu(rsc_hdr->cnt); j++, ent++) {
			if (memcmp(ent->id, query, sizeof(ent->id)) == 0)
				break;
		}

		if (j < le16_to_cpu(rsc_hdr->cnt)) {
			memcpy(eh, ent, sizeof(*ent));
			memcpy(rh, rsc_hdr, sizeof(*rh));
			return 0;
		}
	}

	return -ENODEV;
}

/**
 * cmd_db_read_addr() - Query command db for resource id address.
 *
 * @id: resource id to query for address
 *
 * Return: resource address on success, 0 on error
 *
 * This is used to retrieve resource address based on resource
 * id.
 */
u32 cmd_db_read_addr(const char *id)
>>>>>>> v4.19.83
{
	int ret;
	struct entry_header ent;
	struct rsc_hdr rsc_hdr;

<<<<<<< HEAD
	ret = cmd_db_get_header_by_rsc_id(resource_id, &ent, &rsc_hdr);

	return ret < 0 ? 0 : ent.addr;
}

bool cmd_db_get_priority(u32 addr, u8 drv_id)
{
	int ret;
	struct entry_header ent;
	struct rsc_hdr rsc_hdr;

	ret = cmd_db_get_header_by_addr(addr, &ent, &rsc_hdr);

	return ret < 0 ? false : (bool)(ent.priority[0] & (1 << drv_id));

}
int cmd_db_get_aux_data(const char *resource_id, u8 *data, int len)
=======
	ret = cmd_db_get_header(id, &ent, &rsc_hdr);

	return ret < 0 ? 0 : le32_to_cpu(ent.addr);
}
EXPORT_SYMBOL(cmd_db_read_addr);

/**
 * cmd_db_read_aux_data() - Query command db for aux data.
 *
 *  @id: Resource to retrieve AUX Data on.
 *  @data: Data buffer to copy returned aux data to. Returns size on NULL
 *  @len: Caller provides size of data buffer passed in.
 *
 *  Return: size of data on success, errno otherwise
 */
int cmd_db_read_aux_data(const char *id, u8 *data, size_t len)
>>>>>>> v4.19.83
{
	int ret;
	struct entry_header ent;
	struct rsc_hdr rsc_hdr;
<<<<<<< HEAD
=======
	u16 ent_len;
>>>>>>> v4.19.83

	if (!data)
		return -EINVAL;

<<<<<<< HEAD
	ret = cmd_db_get_header_by_rsc_id(resource_id, &ent, &rsc_hdr);

	if (ret)
		return ret;

	if (ent.len < len)
		return -EINVAL;

	len = (ent.len < len) ? ent.len : len;

	memcpy_fromio(data,
			start_addr + sizeof(*cmd_db_header)
			+ rsc_hdr.data_offset + ent.offset,
			len);
	return len;
}

int cmd_db_get_aux_data_len(const char *resource_id)
=======
	ret = cmd_db_get_header(id, &ent, &rsc_hdr);
	if (ret)
		return ret;

	ent_len = le16_to_cpu(ent.len);
	if (len < ent_len)
		return -EINVAL;

	len = min_t(u16, ent_len, len);
	memcpy(data, rsc_offset(&rsc_hdr, &ent), len);

	return len;
}
EXPORT_SYMBOL(cmd_db_read_aux_data);

/**
 * cmd_db_read_aux_data_len - Get the length of the auxiliary data stored in DB.
 *
 * @id: Resource to retrieve AUX Data.
 *
 * Return: size on success, 0 on error
 */
size_t cmd_db_read_aux_data_len(const char *id)
>>>>>>> v4.19.83
{
	int ret;
	struct entry_header ent;
	struct rsc_hdr rsc_hdr;

<<<<<<< HEAD
	ret = cmd_db_get_header_by_rsc_id(resource_id, &ent, &rsc_hdr);

	return ret < 0 ? 0 : ent.len;
}

u16 cmd_db_get_version(const char *resource_id)
=======
	ret = cmd_db_get_header(id, &ent, &rsc_hdr);

	return ret < 0 ? 0 : le16_to_cpu(ent.len);
}
EXPORT_SYMBOL(cmd_db_read_aux_data_len);

/**
 * cmd_db_read_slave_id - Get the slave ID for a given resource address
 *
 * @id: Resource id to query the DB for version
 *
 * Return: cmd_db_hw_type enum on success, CMD_DB_HW_INVALID on error
 */
enum cmd_db_hw_type cmd_db_read_slave_id(const char *id)
>>>>>>> v4.19.83
{
	int ret;
	struct entry_header ent;
	struct rsc_hdr rsc_hdr;
<<<<<<< HEAD

	ret = cmd_db_get_header_by_rsc_id(resource_id, &ent, &rsc_hdr);
	return ret < 0 ? 0 : rsc_hdr.version;
}

int cmd_db_ready(void)
{
	return cmd_db_status;
}

int cmd_db_is_standalone(void)
{
	if (cmd_db_status < 0)
		return cmd_db_status;

	return !!(cmd_db_header->reserved & CMD_DB_STANDALONE_MASK);
}

int cmd_db_get_slave_id(const char *resource_id)
{
	int ret;
	struct entry_header ent;
	struct rsc_hdr rsc_hdr;

	ret = cmd_db_get_header_by_rsc_id(resource_id, &ent, &rsc_hdr);
	return ret < 0 ? 0 : (ent.addr >> SLAVE_ID_SHIFT) & SLAVE_ID_MASK;
}

static void *cmd_db_start(struct seq_file *m, loff_t *pos)
{
	struct cmd_db_header *hdr = m->private;
	int slv_idx, ent_idx;
	struct entry_header *ent;
	int total = 0;

	for (slv_idx = 0; slv_idx < MAX_SLV_ID; slv_idx++) {

		if (!hdr->header[slv_idx].cnt)
			continue;
		ent_idx = *pos - total;
		if (ent_idx < hdr->header[slv_idx].cnt)
			break;

		total += hdr->header[slv_idx].cnt;
	}

	if (slv_idx == MAX_SLV_ID)
		return NULL;

	ent = start_addr + hdr->header[slv_idx].header_offset + sizeof(*hdr);
	return &ent[ent_idx];

}

static void cmd_db_stop(struct seq_file *m, void *v)
{
}

static void *cmd_db_next(struct seq_file *m, void *v, loff_t *pos)
{
	(*pos)++;
	return cmd_db_start(m, pos);
}

static int cmd_db_seq_show(struct seq_file *m, void *v)
{
	struct entry_header *eh = v;
	struct cmd_db_header *hdr = m->private;
	char buf[9]  = {0};

	if (!eh)
		return 0;

	memcpy(buf, &eh->res_id, min(sizeof(eh->res_id), sizeof(buf)));

	seq_printf(m, "Address: 0x%05x, id: %s", eh->addr, buf);

	if (eh->len) {
		int slv_id = (eh->addr >> SLAVE_ID_SHIFT) & SLAVE_ID_MASK;
		u8 aux[32] = {0};
		int len;
		int k;

		len = min_t(u32, eh->len, sizeof(aux));

		for (k = 0; k < MAX_SLV_ID; k++) {
			if (hdr->header[k].slv_id == slv_id)
				break;
		}

		if (k == MAX_SLV_ID)
			return -EINVAL;

		memcpy_fromio(aux, start_addr + hdr->header[k].data_offset
			+ eh->offset + sizeof(*cmd_db_header), len);

		seq_puts(m, ", aux data: ");

		for (k = 0; k < len; k++)
			seq_printf(m, "%02x ", aux[k]);

	}
	seq_puts(m, "\n");
	return 0;
}

static const struct seq_operations cmd_db_seq_ops = {
	.start = cmd_db_start,
	.stop = cmd_db_stop,
	.next = cmd_db_next,
	.show = cmd_db_seq_show,
};

static int cmd_db_file_open(struct inode *inode, struct file *file)
{
	int ret = seq_open(file, &cmd_db_seq_ops);
	struct seq_file *s = (struct seq_file *)(file->private_data);

	s->private = inode->i_private;
	return ret;
}

static const struct file_operations cmd_db_fops = {
	.owner = THIS_MODULE,
	.open = cmd_db_file_open,
	.read = seq_read,
	.release = seq_release,
	.llseek = no_llseek,
};

static int cmd_db_dev_probe(struct platform_device *pdev)
{
	struct resource res;
	void __iomem *dict;

	dict = of_iomap(pdev->dev.of_node, 0);
	if (!dict) {
		cmd_db_status = -ENOMEM;
		pr_err("Command DB dictionary addr not found.\n");
		goto failed;
	}

	/*
	 * Read start address and size of the command DB address from
	 * shared dictionary location
	 */
	res.start = readl_relaxed(dict);
	res.end = res.start + readl_relaxed(dict + 0x4);
	res.flags = IORESOURCE_MEM;
	res.name = NULL;
	iounmap(dict);

	start_addr = devm_ioremap_resource(&pdev->dev, &res);

	cmd_db_header = devm_kzalloc(&pdev->dev,
			sizeof(*cmd_db_header), GFP_KERNEL);

	if (!cmd_db_header) {
		cmd_db_status = -ENOMEM;
		pr_err("Command DB header not found.\n");
		goto failed;
	}

	memcpy(cmd_db_header, start_addr, sizeof(*cmd_db_header));

	if (cmd_db_header->magic_num != CMD_DB_MAGIC) {
		pr_err("%s(): Invalid Magic\n", __func__);
		cmd_db_status = -EINVAL;
		goto failed;
	}
	cmd_db_status = 0;
	of_platform_populate(pdev->dev.of_node, NULL, NULL, &pdev->dev);

	if (!debugfs_create_file("cmd_db", 0444, NULL,
				cmd_db_header, &cmd_db_fops))
		pr_err("Couldn't create debugfs\n");

	if (cmd_db_is_standalone() == 1)
		pr_info("Command DB is initialized in standalone mode.\n");

failed:
	return cmd_db_status;
}

static const struct of_device_id cmd_db_match_table[] = {
	{.compatible = "qcom,cmd-db"},
	{},
};

static struct platform_driver cmd_db_dev_driver = {
	.probe = cmd_db_dev_probe,
	.driver = {
		.name = "cmd-db",
		.owner = THIS_MODULE,
		.of_match_table = cmd_db_match_table,
	},
};

int __init cmd_db_device_init(void)
=======
	u32 addr;

	ret = cmd_db_get_header(id, &ent, &rsc_hdr);
	if (ret < 0)
		return CMD_DB_HW_INVALID;

	addr = le32_to_cpu(ent.addr);
	return (addr >> SLAVE_ID_SHIFT) & SLAVE_ID_MASK;
}
EXPORT_SYMBOL(cmd_db_read_slave_id);

static int cmd_db_dev_probe(struct platform_device *pdev)
{
	struct reserved_mem *rmem;
	int ret = 0;

	rmem = of_reserved_mem_lookup(pdev->dev.of_node);
	if (!rmem) {
		dev_err(&pdev->dev, "failed to acquire memory region\n");
		return -EINVAL;
	}

	cmd_db_header = memremap(rmem->base, rmem->size, MEMREMAP_WB);
	if (IS_ERR_OR_NULL(cmd_db_header)) {
		ret = PTR_ERR(cmd_db_header);
		cmd_db_header = NULL;
		return ret;
	}

	if (!cmd_db_magic_matches(cmd_db_header)) {
		dev_err(&pdev->dev, "Invalid Command DB Magic\n");
		return -EINVAL;
	}

	return 0;
}

static const struct of_device_id cmd_db_match_table[] = {
	{ .compatible = "qcom,cmd-db" },
	{ },
};

static struct platform_driver cmd_db_dev_driver = {
	.probe  = cmd_db_dev_probe,
	.driver = {
		   .name = "cmd-db",
		   .of_match_table = cmd_db_match_table,
	},
};

static int __init cmd_db_device_init(void)
>>>>>>> v4.19.83
{
	return platform_driver_register(&cmd_db_dev_driver);
}
arch_initcall(cmd_db_device_init);
