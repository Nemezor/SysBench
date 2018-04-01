#include <multiboot.h>
#include <setup.h>
#include <print.h>
#include <serial.h>
#include <text_device.h>
#include <utils.h>
#include <pic.h>
#include <apic.h>
#include <system.h>
#include <fpu.h>
#include <memory.h>
#include <smp.h>
#include <pit.h>
#include <scheduler.h>
#include <renderer.h>
#include <acpi.h>
#include <vbe.h>
#include <render/software.h>
#include <math.h>
#include <render/textures.h>
#define LOAD_TEXTURES
#include "resources/textures.c"
#undef LOAD_TEXTURES
#define LOAD_MODELS
#include "resources/triangles_1.c"
#undef LOAD_MODELS
#include <console.h>
#include <temp.h>

#define SYSBENCH_VERSION "0.1"

#define PRINT_FPS

struct text_device* setup_serial_textdev;

void setup_putchar(char c) {
    setup_serial_textdev->putchar(c);
    console_putchar(c);
}

char setup_getcolor() { return 0; };
char setup_canprint() { return 1; };
void setup_setcolor(char color) {};

struct text_device setup_textdev;

void __setup_kernel_entry(struct multiboot_info* mbinfo, uint32_t bootloader_return) {
    if (bootloader_return != MULTIBOOT_BOOTLOADER_MAGIC) {
        stop();
    }

    setup_textdev.putchar = &setup_putchar;
    setup_textdev.setcolor = &setup_setcolor;
    setup_textdev.getcolor = &setup_getcolor;
    setup_textdev.canprint = &setup_canprint;

    // initialize serial and printing
    serial_initialize();
    setup_serial_textdev = serial_get_text_device();
    print_set_output(&setup_textdev);
    console_clear_screen();

    printf("\n   _____            __                    __  \n");
    printf("  / ___/__  _______/ /_  ___  ____  _____/ /_ \n");
    printf("  \\__ \\/ / / / ___/ __ \\/ _ \\/ __ \\/ ___/ __ \\\n");
    printf(" ___/ / /_/ (__  ) /_/ /  __/ / / / /__/ / / /\n");
    printf("/____/\\__, /____/_.___/\\___/_/ /_/\\___/_/ /_/ \n");
    printf("     /____/                                  \n\n");
    printf("##############################################\n\n");
    printf("[[init]: Starting Sysbench %s\n", SYSBENCH_VERSION);

    // enable the FPU and set up SSE
    fpu_init();

    // initialize memory management
    memory_init();

    // disable the 8259 PICs
    pic_map(32, 40);
    pic_disable();

    // scan for ACPI tables and parse them
    char acpi_supported = acpi_initialize();
    // enable the APIC
    uint8_t core_id;
    if (acpi_supported) {
        core_id = apic_enable();
    }else{
        printf("[[main]: ACPI and APIC are required to run SysBench!\n");
        printf("[[main]: Halting...\n");
        stop();
    }
    scheduler_add_worker(core_id);
    pit_setup_sleep();

    uint8_t* cpu_ids = smp_ids();
    uint32_t cpus = smp_count();
    for (uint32_t i = 0; i < cpus; i++) {
        char status = smp_boot(cpu_ids[i]);
        if (status == SMP_STATUS_RUNNING) {
            scheduler_add_worker(cpu_ids[i]);
        }
    }

    // initialize the framebuffer
    if (vbe_initialize(core_id, mbinfo)) {
        printf("[[main]: A VBE framebuffer is required to run SysBench!\n");
        printf("[[main]: Halting...\n");
        stop();
    }

    char temp_support = temp_supported();

    struct gfx_device *gfx_vbe = vbe_get_gfx_device();
    struct gfx_render *renderer = gfx_vbe->renderer;

    uint32_t fps = 0;
    uint64_t last_update = current_time();

    matrix_t projection;
    matrix_t transformation;
    //TODO: get monitor width and height from EDID
    matrix_perspective(&projection, radians(70.0f), 16, 9, 0.01f, 500.0f);
    matrix_identity(&transformation);
    renderer->set_projection(&projection);
    renderer->set_transformation(&transformation);
    renderer->set_viewport(0, 0, renderer->get_width(), renderer->get_height());
    //renderer->texture_bind(0, &texture_dirt);

    float* triangles_vertices = memory_reserve(TRIANGLES_1_COUNT * 48);
    float* triangles_colors = memory_reserve(TRIANGLES_1_COUNT * 48);

    memcpy(triangles_1_vertices, triangles_vertices, TRIANGLES_1_COUNT * 48);
    memcpy(triangles_1_colors, triangles_colors, TRIANGLES_1_COUNT * 48);

    while (1) {
        // TODO: change to near -inf
        renderer->clear(0xFF000000, -2000000.0f);
        // vertices have to be re-copied every frame
        memcpy(triangles_1_vertices, triangles_vertices, TRIANGLES_1_COUNT * 48);
        float rotation = (current_time() % 10000) / 10000.0f * 360.0f;
        matrix_identity(&transformation);
        matrix_translate(&transformation, 0.0f, 0.0f, -75.0f);
        matrix_rotate_translation(&transformation, radians(rotation), 0, 1, 0);
        renderer->set_transformation(&transformation);
        renderer->draw_triangles(TRIANGLES_1_COUNT, triangles_vertices, 0, triangles_colors, 0);

        gfx_vbe->update_screen();
        fps++;
        if (last_update + 1000 < current_time()) {
            last_update = current_time();
            printf("[[GFX ]: %u FPS\n", fps);
            fps = 0;
            if (temp_support) {
                printf("[[temp]: CPU: %i°C\n", get_temperature());
            }
        }
    }

    printf("[[main]: Halting...\n");
    stop();
}
