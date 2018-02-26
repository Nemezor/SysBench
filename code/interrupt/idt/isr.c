#include <isr.h>
#include <idt.h>
#include <stdint.h>
#include <print.h>
#include <apic.h>
#include <fpu.h>
#include <pit.h>
#include <smp.h>
#include <system.h>

struct interrupt_frame {
    uint64_t rip;
    uint64_t cs;
    uint64_t flags;
    uint64_t sp;
    uint64_t ss;
};

void isr_send_eoi() {
    uint64_t idtr = idt_get_idtr();
    struct idt_table* idt = (struct idt_table*)idtr;
    apic_eoi(idt->apic_base);
}

__attribute__((interrupt)) void isr_div_by_zero(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Division by zero!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_debug(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Debug interrupt!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_nmi(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Non-maskable interrupt!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_breakpoint(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Breakpoint interrupt!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_into(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: INTO error!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_bound(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: BOUND error!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_invalid_opcode(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Invalid opcode!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_coprocessor_not_available(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Coprocessor not available!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_double_fault(struct interrupt_frame* frame, uint64_t code) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Double fault!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_coprocessor_segment_overrun(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Coprocessor segment overrun!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_tss_error(struct interrupt_frame* frame, uint64_t code) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: TSS error!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_segment_not_present(struct interrupt_frame* frame, uint64_t code) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Segment not present!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_stack_fault(struct interrupt_frame* frame, uint64_t code) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Stack fault!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_gpf(struct interrupt_frame* frame, uint64_t code) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: General protection fault!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_page_fault(struct interrupt_frame* frame, uint64_t code) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Page fault! %i\n", code);
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_coprocessor_error(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Coprocessor zero!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_spurious(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: APIC spurious interrupt!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_pic(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: PIC IRQ fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_dead(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Dead IRQ fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_timer(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    pit_irq0_trigger();
    system_irq_increment_time();
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_keyboard(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Keyboard IRQ fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_com2(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: COM2 IRQ fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_com1(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: COM1 IRQ fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_lpt2(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: LPT2 IRQ fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_floppy(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Floppy IRQ fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_lpt1(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: LPT1 IRQ fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_rtc(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Real-Time Clock IRQ fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_irq9(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    //printf("[[ISR ]: IRQ9 fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_irq10(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: IRQ10 fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_irq11(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: IRQ11 fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_ps2mouse(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: PS/2 Mouse IRQ fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_coprocessor(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Coprocessor fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_ataprimary(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Primary ATA IRQ fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_irq_atasecondary(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    printf("[[ISR ]: Secondary ATA IRQ fired!\n");
    isr_send_eoi();
    fpu_load(&fpu_state);
}

__attribute__((interrupt)) void isr_ipi_wakeup(struct interrupt_frame* frame) {
    char fpu_state[512];
    fpu_save(&fpu_state);
    isr_send_eoi();
    fpu_load(&fpu_state);
}

void isr_install(struct idt_table* idt) {
    idt_set_entry(idt, &isr_div_by_zero, 0);
    idt_set_entry(idt, &isr_debug, 1);
    idt_set_entry(idt, &isr_nmi, 2);
    idt_set_entry(idt, &isr_breakpoint, 3);
    idt_set_entry(idt, &isr_into, 4);
    idt_set_entry(idt, &isr_bound, 5);
    idt_set_entry(idt, &isr_invalid_opcode, 6);
    idt_set_entry(idt, &isr_coprocessor_not_available, 7);
    idt_set_entry(idt, &isr_double_fault, 8);
    idt_set_entry(idt, &isr_coprocessor_segment_overrun, 9);
    idt_set_entry(idt, &isr_tss_error, 10);
    idt_set_entry(idt, &isr_segment_not_present, 11);
    idt_set_entry(idt, &isr_stack_fault, 12);
    idt_set_entry(idt, &isr_gpf, 13);
    idt_set_entry(idt, &isr_page_fault, 14);
    idt_set_entry(idt, &isr_coprocessor_error, 15);

    for (int i = 0; i < 16; i++) {
        idt_set_entry(idt, &isr_pic, i + 32);
        idt_set_entry(idt, &isr_irq_dead, i + 48);
    }

    if (smp_is_bsp(idt->processor_id)) {
        idt_set_entry(idt, &isr_irq_timer, 48);
        idt_set_entry(idt, &isr_irq_keyboard, 49);
        idt_set_entry(idt, &isr_irq_dead, 50);
        idt_set_entry(idt, &isr_irq_com2, 51);
        idt_set_entry(idt, &isr_irq_com1, 52);
        idt_set_entry(idt, &isr_irq_lpt2, 53);
        idt_set_entry(idt, &isr_irq_floppy, 54);
        idt_set_entry(idt, &isr_irq_lpt1, 55);
        idt_set_entry(idt, &isr_irq_rtc, 56);
        idt_set_entry(idt, &isr_irq_irq9, 57);
        idt_set_entry(idt, &isr_irq_irq10, 58);
        idt_set_entry(idt, &isr_irq_irq11, 59);
        idt_set_entry(idt, &isr_irq_ps2mouse, 60);
        idt_set_entry(idt, &isr_irq_coprocessor, 61);
        idt_set_entry(idt, &isr_irq_ataprimary, 62);
        idt_set_entry(idt, &isr_irq_atasecondary, 63);
    }

    idt_set_entry(idt, &isr_ipi_wakeup, SMP_WAKE_INTERRUPT);
    idt_set_entry(idt, &isr_spurious, 255);
}
