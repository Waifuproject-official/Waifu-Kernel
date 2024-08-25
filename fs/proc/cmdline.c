// SPDX-License-Identifier: GPL-2.0
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static int cmdline_proc_show(struct seq_file *m, void *v)
{
	seq_puts(m, saved_command_line);
	seq_putc(m, '\n');
	return 0;
}

static int __init proc_cmdline_init(void)
{
<<<<<<< HEAD
	char *offset_addr;

	offset_addr = strstr(saved_command_line, "androidboot.mode=reboot");
	if (offset_addr != NULL)
		strncpy(offset_addr + 17, "normal", 6);

	proc_create("cmdline", 0, NULL, &cmdline_proc_fops);
=======
	proc_create_single("cmdline", 0, NULL, cmdline_proc_show);
>>>>>>> v4.19.83
	return 0;
}
fs_initcall(proc_cmdline_init);
