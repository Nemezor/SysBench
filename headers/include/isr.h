#ifndef INTERRUPT_IDT_ISR_H
#define INTERRUPT_IDT_ISR_H

#include <idt.h>

void isr_install(struct idt_table* idt);

#endif
