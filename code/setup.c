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
#include "gfx/software/textures.c"
#undef LOAD_TEXTURES
#include <console.h>

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

    struct gfx_device *gfx_vbe = vbe_get_gfx_device();
    struct gfx_render *renderer = gfx_vbe->renderer;

    uint32_t fps = 0;
    uint64_t last_update = current_time();

    float vertices[] = {
        -1.0,-1.0,-1.0, 0.0f,  -1.0,-1.0, 1.0, 0.0f,  -1.0, 1.0, 1.0, 0.0f,
         1.0, 1.0,-1.0, 0.0f,  -1.0,-1.0,-1.0, 0.0f,  -1.0, 1.0,-1.0, 0.0f,
         1.0,-1.0, 1.0, 0.0f,  -1.0,-1.0,-1.0, 0.0f,   1.0,-1.0,-1.0, 0.0f,
         1.0, 1.0,-1.0, 0.0f,   1.0,-1.0,-1.0, 0.0f,  -1.0,-1.0,-1.0, 0.0f,
        -1.0,-1.0,-1.0, 0.0f,  -1.0, 1.0, 1.0, 0.0f,  -1.0, 1.0,-1.0, 0.0f,
         1.0,-1.0, 1.0, 0.0f,  -1.0,-1.0, 1.0, 0.0f,  -1.0,-1.0,-1.0, 0.0f,
        -1.0, 1.0, 1.0, 0.0f,  -1.0,-1.0, 1.0, 0.0f,   1.0,-1.0, 1.0, 0.0f,
         1.0, 1.0, 1.0, 0.0f,   1.0,-1.0,-1.0, 0.0f,   1.0, 1.0,-1.0, 0.0f,
         1.0,-1.0,-1.0, 0.0f,   1.0, 1.0, 1.0, 0.0f,   1.0,-1.0, 1.0, 0.0f,
         1.0, 1.0, 1.0, 0.0f,   1.0, 1.0,-1.0, 0.0f,  -1.0, 1.0,-1.0, 0.0f,
         1.0, 1.0, 1.0, 0.0f,  -1.0, 1.0,-1.0, 0.0f,  -1.0, 1.0, 1.0, 0.0f,
         1.0, 1.0, 1.0, 0.0f,  -1.0, 1.0, 1.0, 0.0f,   1.0,-1.0, 1.0, 0.0f
    };
    float vertices_copy[12 * 3 * 4];
    float texture_coords[] = {
        0.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,
        0.0f, 0.0f,   0.0f, 1.0f,   1.0f, 0.0f,
        0.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,
        0.0f, 0.0f,   1.0f, 0.0f,   0.0f, 1.0f,
        0.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,
        0.0f, 0.0f,   1.0f, 0.0f,   0.0f, 1.0f,
        0.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,
        0.0f, 0.0f,   1.0f, 0.0f,   0.0f, 1.0f,
        0.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,
        0.0f, 0.0f,   1.0f, 0.0f,   0.0f, 1.0f,
        0.0f, 0.0f,   1.0f, 1.0f,   0.0f, 1.0f,
        0.0f, 0.0f,   1.0f, 0.0f,   0.0f, 1.0f
    };
    float colors[] = {
        0.583, 0.771, 0.014, 1.0f,   0.609, 0.115, 0.436, 1.0f,   0.327, 0.483, 0.844, 1.0f,
        0.822, 0.569, 0.201, 1.0f,   0.435, 0.602, 0.223, 1.0f,   0.310, 0.747, 0.185, 1.0f,
        0.597, 0.770, 0.761, 1.0f,   0.559, 0.436, 0.730, 1.0f,   0.359, 0.583, 0.152, 1.0f,
        0.483, 0.596, 0.789, 1.0f,   0.559, 0.861, 0.639, 1.0f,   0.195, 0.548, 0.859, 1.0f,
        0.014, 0.184, 0.576, 1.0f,   0.771, 0.328, 0.970, 1.0f,   0.406, 0.615, 0.116, 1.0f,
        0.676, 0.977, 0.133, 1.0f,   0.971, 0.572, 0.833, 1.0f,   0.140, 0.616, 0.489, 1.0f,
        0.997, 0.513, 0.064, 1.0f,   0.945, 0.719, 0.592, 1.0f,   0.543, 0.021, 0.978, 1.0f,
        0.279, 0.317, 0.505, 1.0f,   0.167, 0.620, 0.077, 1.0f,   0.347, 0.857, 0.137, 1.0f,
        0.055, 0.953, 0.042, 1.0f,   0.714, 0.505, 0.345, 1.0f,   0.783, 0.290, 0.734, 1.0f,
        0.722, 0.645, 0.174, 1.0f,   0.302, 0.455, 0.848, 1.0f,   0.225, 0.587, 0.040, 1.0f,
        0.517, 0.713, 0.338, 1.0f,   0.053, 0.959, 0.120, 1.0f,   0.393, 0.621, 0.362, 1.0f,
        0.673, 0.211, 0.457, 1.0f,   0.820, 0.883, 0.371, 1.0f,   0.982, 0.099, 0.879, 1.0f
    };

    matrix_t projection;
    matrix_t transformation;
    matrix_perspective(&projection, radians(70.0f), 16, 9, 0.01f, 500.0f);
    matrix_identity(&transformation);
    matrix_translate(&transformation, 0.0f, 0.0f, 3.0f);
    renderer->set_projection(&projection);
    renderer->set_transformation(&transformation);
    renderer->set_viewport(0, 0, renderer->get_width(), renderer->get_height());
    renderer->texture_bind(0, &texture_dirt);

    while (1) {
        // TODO: change to near -inf
        renderer->clear(0xFF000000, -2000000.0f);
        memcpy(vertices, vertices_copy, 12 * 3 * 4 * sizeof(float));
        float rotation = (current_time() % 5000) / 5000.0f * 360.0f;
        matrix_identity(&transformation);
        matrix_translate(&transformation, 0.0f, 0.0f, 7.0f);
        matrix_rotate_translation(&transformation, radians(rotation), 0, 1, 0);
        renderer->set_transformation(&transformation);
        renderer->draw_triangles(12, vertices_copy, texture_coords, colors, 0);
        gfx_vbe->update_screen();
        fps++;
        if (last_update + 1000 < current_time()) {
            last_update = current_time();
            printf("[[GFX ]: %u FPS\n", fps);
            fps = 0;
        }
    }

    printf("[[main]: Halting...\n");
    stop();
}
