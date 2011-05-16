#ifndef _LINUX_IRQNR_H
#define _LINUX_IRQNR_H

/*
 * Generic irq_desc iterators:
 */
#ifdef __KERNEL__


extern int nr_irqs;
extern struct irq_desc *irq_to_desc(unsigned int irq);

# define for_each_irq_desc(irq, desc)					\
	for (irq = 0, desc = irq_to_desc(irq); irq < nr_irqs;		\
	     irq++, desc = irq_to_desc(irq))				\
		if (!desc)						\
			;						\
		else


# define for_each_irq_desc_reverse(irq, desc)				\
	for (irq = nr_irqs - 1, desc = irq_to_desc(irq); irq >= 0;	\
	     irq--, desc = irq_to_desc(irq))				\
		if (!desc)						\
			;						\
		else

#ifdef CONFIG_SMP
#define irq_node(irq)	(irq_to_desc(irq)->node)
#else
#define irq_node(irq)	0
#endif


#define for_each_irq_nr(irq)                   \
       for (irq = 0; irq < nr_irqs; irq++)

#endif /* __KERNEL__ */

#endif
