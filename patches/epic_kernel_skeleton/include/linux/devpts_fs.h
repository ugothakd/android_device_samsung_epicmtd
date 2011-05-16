/* -*- linux-c -*- --------------------------------------------------------- *
 *
 * linux/include/linux/devpts_fs.h
 *
 *  Copyright 1998-2004 H. Peter Anvin -- All Rights Reserved
 *
 * This file is part of the Linux kernel and is made available under
 * the terms of the GNU General Public License, version 2, or at your
 * option, any later version, incorporated herein by reference.
 *
 * ------------------------------------------------------------------------- */

#ifndef _LINUX_DEVPTS_FS_H
#define _LINUX_DEVPTS_FS_H

#include <linux/errno.h>


int devpts_new_index(struct inode *ptmx_inode);
void devpts_kill_index(struct inode *ptmx_inode, int idx);
/* mknod in devpts */
int devpts_pty_new(struct inode *ptmx_inode, struct tty_struct *tty);
/* get tty structure */
struct tty_struct *devpts_get_tty(struct inode *pts_inode, int number);
/* unlink */
void devpts_pty_kill(struct tty_struct *tty);



#endif /* _LINUX_DEVPTS_FS_H */
