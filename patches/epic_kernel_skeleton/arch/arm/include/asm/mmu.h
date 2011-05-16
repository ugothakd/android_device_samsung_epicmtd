#ifndef __ARM_MMU_H
#define __ARM_MMU_H


typedef struct {
	unsigned int id;
	unsigned int kvm_seq;
} mm_context_t;

#define ASID(mm)	((mm)->context.id & 255)


#endif
