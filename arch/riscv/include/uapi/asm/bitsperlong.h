/*
<<<<<<<< HEAD:lib/vdso/datapage.h
 * Userspace implementations of __get_datapage
========
 * Copyright (C) 2012 ARM Ltd.
 * Copyright (C) 2015 Regents of the University of California
>>>>>>>> v4.19.83:arch/riscv/include/uapi/asm/bitsperlong.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

<<<<<<<< HEAD:lib/vdso/datapage.h
#ifndef __VDSO_DATAPAGE_H
#define __VDSO_DATAPAGE_H

#error	"vdso: Provide a user space architecture specific definition or"
#error	"prototype for struct vdso_data *__get_datapage(void).  Also define"
#error	"ARCH_CLOCK_FIXED_MASK if not provided by cs_mask."

#endif /* __VDSO_DATAPAGE_H */
========
#ifndef _UAPI_ASM_RISCV_BITSPERLONG_H
#define _UAPI_ASM_RISCV_BITSPERLONG_H

#define __BITS_PER_LONG (__SIZEOF_POINTER__ * 8)

#include <asm-generic/bitsperlong.h>

#endif /* _UAPI_ASM_RISCV_BITSPERLONG_H */
>>>>>>>> v4.19.83:arch/riscv/include/uapi/asm/bitsperlong.h
