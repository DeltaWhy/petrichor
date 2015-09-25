#ifndef ACPI_H
#define ACPI_H
typedef struct acpi_sdt {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} acpi_sdt;

typedef struct acpi_rsd {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    acpi_sdt *rsdt_addr;
} acpi_rsd;

typedef struct acpi_gas {
    uint8_t address_space;
    uint8_t bit_width;
    uint8_t bit_offset;
    uint8_t access_size;
    uint64_t address;
} acpi_gas;

typedef struct acpi_fadt {
    acpi_sdt header;
    uint32_t firmware_ctrl;
    acpi_sdt *dsdt_addr;

    uint8_t  Reserved;
    uint8_t  PreferredPowerManagementProfile;
    uint16_t SCI_Interrupt;
    uint32_t SMI_CommandPort;
    uint8_t  AcpiEnable;
    uint8_t  AcpiDisable;
    uint8_t  S4BIOS_REQ;
    uint8_t  PSTATE_Control;
    uint32_t PM1aEventBlock;
    uint32_t PM1bEventBlock;
    uint32_t PM1aControlBlock;
    uint32_t PM1bControlBlock;
    uint32_t PM2ControlBlock;
    uint32_t PMTimerBlock;
    uint32_t GPE0Block;
    uint32_t GPE1Block;
    uint8_t  PM1EventLength;
    uint8_t  PM1ControlLength;
    uint8_t  PM2ControlLength;
    uint8_t  PMTimerLength;
    uint8_t  GPE0Length;
    uint8_t  GPE1Length;
    uint8_t  GPE1Base;
    uint8_t  CStateControl;
    uint16_t WorstC2Latency;
    uint16_t WorstC3Latency;
    uint16_t FlushSize;
    uint16_t FlushStride;
    uint8_t  DutyOffset;
    uint8_t  DutyWidth;
    uint8_t  DayAlarm;
    uint8_t  MonthAlarm;
    uint8_t  Century;
    uint16_t BootArchitectureFlags;
    uint8_t  Reserved2;
    uint32_t Flags;
    acpi_gas ResetReg;
    uint8_t  ResetValue;
    uint8_t  Reserved3[3];
} acpi_fadt;

void init_acpi(void);
void acpi_reclaim_memory(void);
void acpi_shutdown(void);
#endif
