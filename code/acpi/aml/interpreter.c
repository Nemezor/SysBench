#include <acpi/aml/interpreter.h>
#include <acpi/aml/opcodes.h>
#include <acpi/aml/structs.h>
#include <acpi/tables.h>
#include <print.h>
#include <memory.h>

//#define AML_DEBUG
//#define AML_OPCODE
//#define AML_RAW

void aml_log_opcode(char* string) {
    #ifdef AML_OPCODE
    printf("[[AMLI]: DEBUG %s\n", string);
    #endif
}

char aml_is_LeadNameChar(char c) {
    return (c >= CHAR_ALPHA_START && c <= CHAR_ALPHA_END) | (c == UNDERSCORE_CHAR);
}

char aml_is_DigitChar(char c) {
    return c >= DIGIT_CHAR_START && c <= DIGIT_CHAR_END;
}

char aml_is_NameChar(char c) {
    return aml_is_LeadNameChar(c) || aml_is_DigitChar(c);
}

uint32_t aml_get_PkgLength(uint8_t* code, uint32_t* index) {
    uint32_t i = *index;
    uint8_t leadByte = code[i];
    uint32_t length = 0;
    if (leadByte & 0xC0) {
        // multi-byte
        uint32_t bytes = (uint32_t)((leadByte & 0xC0) >> 6);
        length |= (uint32_t)(leadByte & 0xF);
        length |= ((uint32_t)code[i + 1] << 4);
        if (bytes > 1) {
            length |= ((uint32_t)code[i + 2] << 12);
        }
        if (bytes > 2) {
            length |= ((uint32_t)code[i + 3] << 20);
        }
        i += bytes + 1;
    }else{
        // single-byte
        length = (uint32_t)(leadByte & 0x3F);
        i++;
    }
    *index = i;
    return length;
}

void aml_process(struct acpi_header* table) {
    uint8_t* code = (uint8_t*)((void*)table);
    uint32_t length = table->length;

    printf("[[AMLI]: Processing %i bytes of AML at %X\n", length, code);

    #ifdef AML_RAW
    printf("[[AMLI]: Raw AML dump:\n");
    for (uint32_t i = 0; i < length; i++) {
        printf("%x ", code[i]);
    }
    printf("\n");
    #endif

    for (uint32_t i = 0; i < length; i++) {

    }
}
