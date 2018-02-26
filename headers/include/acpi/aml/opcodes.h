#ifndef ACPI_AML_OPCODES_H
#define ACPI_AML_OPCODES_H

#define ZERO_OP 0x00
#define ONE_OP 0x01
#define ALIAS_OP 0x06
#define NAME_OP 0x08
#define BYTE_PREFIX 0x0A
#define WORD_PREFIX 0x0B
#define DWORD_PREFIX 0x0C
#define STRING_PREFIX 0x0D
#define QWORD_PREFIX 0x0E
#define SCOPE_OP 0x10
#define BUFFER_OP 0x11
#define PACKAGE_OP 0x12
#define VAR_PACKAGE_OP 0x13
#define METHOD_OP 0x14
#define DUAL_NAME_PREFIX 0x2E // char .
#define MULTI_NAME_PREFIX 0x2F // char /
#define EXT_OP_PREFIX 0x5B // [
// require EXT_OP_PREFIX before opcode
#define MUTEX_OP_EXT 0x01
#define EVENT_OP_EXT 0x02
#define COND_REF_OF_OP_EXT 0x12
#define CREATE_FIELD_OP_EXT 0x13
#define LOAD_TABLE_OP_EXT 0x1F
#define LOAD_OP_EXT 0x20
#define STALL_OP_EXT 0x21
#define SLEEP_OP_EXT 0x22
#define ACQUIRE_OP_EXT 0x23
#define SIGNAL_OP_EXT 0x24
#define WAIT_OP_EXT 0x25
#define RESET_OP_EXT 0x26
#define RELEASE_OP_EXT 0x27
#define FROM_BCD_OP_EXT 0x28
#define TO_BCD_OP_EXT 0x29
#define UNLOAD_OP_EXT 0x2A
#define REVISION_OP_EXT 0x30
#define DEBUG_OP_EXT 0x31
#define FATAL_OP_EXT 0x32
#define TIMER_OP_EXT 0x33
#define OP_REGION_OP_EXT 0x80
#define FIELD_OP_EXT 0x81
#define DEVICE_OP_LIST_EXT 0x82
#define PROCESSOR_OP_EXT 0x83
#define POWER_RES_OP_EXT 0x84
#define THERMAL_ZONE_OP_EXT 0x85
#define INDEX_FIELD_OP_EXT 0x86
#define BANK_FIELD_OP_EXT 0x87
#define DATA_REGION_OP_EXT 0x88
// ----
#define LOCAL0_OP 0x60 // char `
#define LOCAL1_OP 0x61 // char a
#define LOCAL2_OP 0x62 // char b
#define LOCAL3_OP 0x63 // char c
#define LOCAL4_OP 0x64 // char d
#define LOCAL5_OP 0x65 // char e
#define LOCAL6_OP 0x66 // char f
#define LOCAL7_OP 0x67 // char g
#define ARG0_OP 0x68 // char h
#define ARG1_OP 0x69 // char i
#define ARG2_OP 0x6A // char j
#define ARG3_OP 0x6B // char k
#define ARG4_OP 0x6C // char l
#define ARG5_OP 0x6D // char m
#define ARG6_OP 0x6E // char n
#define STORE_OP 0x70
#define REF_OF_OP 0x71
#define ADD_OP 0x72
#define CONCAT_OP 0x73
#define SUBTRACT_OP 0x74
#define INCREMENT_OP 0x75
#define DECREMENT_OP 0x76
#define MULTIPLY_OP 0x77
#define DIVIDE_OP 0x78
#define SHIFT_LEFT_OP 0x79
#define SHIFT_RIGHT_OP 0x7A
#define AND_OP 0x7B
#define NAND_OP 0x7C
#define OR_OP 0x7D
#define NOR_OP 0x7E
#define XOR_OP 0x7F
#define NOT_OP 0x80
#define FIND_SET_LEFT_BIT_OP 0x81
#define FIND_SET_RIGHT_BIT_OP 0x82
#define DEREF_OF_OP 0x83
#define CONCAT_RES_OP 0x84
#define MOD_OP 0x85
#define NOTIFY_OP 0x86
#define SIZE_OF_OP 0x87
#define INDEX_OP 0x88
#define MATCH_OP 0x89
#define CREATE_DWORD_FIELD_OP 0x8A
#define CREATE_WORD_FIELD_OP 0x8B
#define CREATE_BYTE_FIELD_OP 0x8C
#define CREATE_BIT_FIELD_OP 0x8D
#define TYPE_OP 0x8E
#define CREATE_QWORD_FIELD_OP 0x8F
#define LAND_OP 0x90
#define LOR_OP 0x91
#define LNOT_OP 0x92
#define LEQUAL_OP 0x93 // when prefixed with LNOT_OP = LNOTEQUAL_OP
#define LGREATER_OP 0x94 // when prefixed with LNOT_OP = LLESSEQUAL_OP
#define LLESS_OP 0x95 // when prefixed with LNOT_OP = LGREATEREQUAL_OP
#define TO_BUFFER_OP 0x96
#define TO_DECIMAL_STRING_OP 0x97
#define TO_HEX_STRING_OP 0x98
#define TO_INTEGER_OP 0x99
#define TO_STRING_OP 0x9C
#define COPY_OBJECT_OP 0x9D
#define MID_OP 0x9E
#define CONTINUE_OP 0x9F
#define IF_OP 0xA0
#define WHILE_OP 0xA2
#define NOOP_OP 0xA3
#define RETURN_OP 0xA4
#define BREAK_OP 0xA5
#define BREAK_POINT_OP 0xCC
#define ONES_OP 0xFF

#define ROOT_CHAR 0x5C // char backslash
#define PARENT_PREFIX_CHAR 0x5E // char ^
#define UNDERSCORE_CHAR 0x5F // char _
#define CHAR_ALPHA_START 0x41
#define CHAR_ALPHA_END 0x5A
#define DIGIT_CHAR_START 0x30
#define DIGIT_CHAR_END 0x39

#endif
