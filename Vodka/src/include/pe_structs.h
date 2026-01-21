#ifndef PE_STRUCTS
#define PE_STRUCTS

#include <stdint.h>

struct ImageDosHeader {
    // Base MZ header
    uint16_t signature;
    uint16_t extra_bytes;
    uint16_t number_of_pages;
    uint16_t entries_in_relocation_table;
    uint16_t header_size;
    uint16_t minimum_allocator;
    uint16_t maximum_allocator;
    uint16_t initial_ss_value;
    uint16_t initial_sp_value;
    uint16_t checksum;
    uint16_t inital_ip_value;
    uint16_t inital_cs_value;
    uint16_t relocation_table_offset;
    uint16_t overlay;
    
    // PE extension for MZ header
    uint16_t reserved_bytes[4];
    uint16_t oem_identifier;
    uint16_t oem_info;
    uint16_t reserved_bytes_2[10];
    uint32_t pe_header_address;
};

struct ImageFileHeader {
    //uint32_t signature; А вы уверены что оно тут есть??? Я чёт не могу найти инфу по сигнатуре имгфайлхеадере))
    // Я даже на оф. сайте Майкрософт не могу его найти, ЛОЛ!
    uint16_t machine;
    uint16_t number_of_sections;
    uint32_t time_date_stamp;
    uint32_t symbol_table_address;
    uint32_t number_of_symbols;
    uint16_t optional_header_size;
    uint16_t characteristics;
};

struct ImageDataDirectory {
    uint32_t virtual_address;
    uint32_t size;
};

struct ImageOptionalHeader32 {
    uint16_t magic_number;
    uint8_t major_linker_version;
    uint8_t minor_linker_version;
    uint32_t size_of_code;
    uint32_t size_of_initalized_data;
    uint32_t size_of_uninitialized_data;
    uint32_t address_of_entry_point;
    uint32_t base_of_code;
    uint32_t base_of_data;
    uint32_t image_base;
    uint32_t section_alignment;
    uint32_t file_alignment;
    uint16_t major_operating_system_version;
    uint16_t minor_operating_system_version;
    uint16_t major_image_version;
    uint16_t minor_image_version;
    uint16_t major_subsystem_version;
    uint16_t minor_subsystem_version;
    uint32_t win32_version_value; // на 32-битных ОС равна 0
    uint32_t size_of_image;
    uint32_t size_of_headers;
    uint32_t checksum;
    uint16_t subsystem;
    uint16_t dll_characteristics;
    uint32_t size_of_stack_reserve;
    uint32_t size_of_stack_commit;
    uint32_t size_of_heap_reserve;
    uint32_t size_of_heap_commit;
    uint32_t loader_flags;
    uint32_t number_of_rva_and_sizes;
    struct ImageDataDirectory data_directory[16]; 
};

struct ImageNtHeaders32 {
    uint32_t signature;
    struct ImageFileHeader file_header;
    struct ImageOptionalHeader32 optional_header;
};

struct ImageSectionHeader {
    char name[8];
    union { // Ну конечно, unionы! То, чье отсутсвие делало этот эксперемент провальным с марта 2025.
        uint32_t physical_address;
        uint32_t virtual_size;
    };
    uint32_t virtual_address;
    uint32_t size_of_raw_data;
    uint32_t pointer_to_raw_data;
    uint32_t pointer_to_relocations;
    uint32_t pointer_to_linenumbers;
    uint16_t number_of_relocations;
    uint16_t number_of_linenumbers;
    uint32_t characteristics;
};

struct ImageImportDescriptor {
    uint32_t characteristics;
    uint32_t original_first_thunk;
    uint32_t time_date_stamp;
    uint32_t forwarder_chain;
    uint32_t name;
    uint32_t first_thunk;
};

struct ImageBoundImportDescriptor {
    uint32_t time_date_stamp;
    uint16_t offset_module_name;
    uint16_t number_of_module_forwarder_refs;
};


struct ImageImportByName {
    uint16_t hint;
    char name[1];
};

struct ImageBaseRelocation {
    uint32_t virtual_address;
    uint32_t size_of_block;
};

struct RichHeaderInfo {
    int size;
    char* buffer_address;
    int entries;
};

struct RichHeaderEntry {
    uint16_t prod_id;
    uint16_t build_id;
    uint32_t use_count;
};

struct RichHeader {
    struct RichHeaderEntry entries;
};

struct IltEntry32 {
    uint32_t ordinal;
    uint32_t hint_name_tabe;
    uint32_t ordinal_name_flag;
};

struct BaseRelocEntry {
    uint16_t offset;
    uint16_t type;
};

struct ImageExportDirectory {
    uint32_t characteristics;
    uint32_t time_date_stamp;
    uint16_t major_version;
    uint16_t minor_version;
    uint32_t name;
    uint32_t base;
    uint32_t number_of_functions;
    uint32_t number_of_names;
    uint32_t address_of_functions;
    uint32_t address_of_names;
    uint32_t address_of_names_ordinals;
};

struct ImageThunkData32 {
    uint32_t forwarder_string;
    uint32_t function;
    uint32_t ordinal;
    uint32_t address_of_data;
};

#endif
