/*
 * arch/arm/kernel/return_address.c
 *
 * Copyright (C) 2009 Uwe Kleine-Koenig <u.kleine-koenig@pengutronix.de>
 * for Pengutronix
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */
#include <linux/module.h>


#warning "TODO: return_address should use unwind tables"

void *return_address(unsigned int level)
{
	return NULL;
}


EXPORT_SYMBOL_GPL(return_address);
