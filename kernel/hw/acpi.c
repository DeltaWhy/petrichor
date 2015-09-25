#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "hw/acpi.h"
#include "hw/port.h"
#include "hw/screen.h"
#include "mboot.h"
#include "mem/vmm.h"
#include "mem/pmm.h"

acpi_rsd *rsdp = NULL;
acpi_sdt *rsdt = NULL;
int num_sdts;
acpi_sdt **sdts;
acpi_fadt *fadt;
acpi_sdt *dsdt;

uint32_t PM1aControlBlock;
uint32_t PM1bControlBlock;
uint32_t SMI_CommandPort;
uint8_t AcpiEnable;
uint16_t slp_typa;
uint16_t slp_typb;
bool can_acpi_shutdown = false;

static bool acpi_checksum(acpi_sdt *sdt)
{
    unsigned char sum = 0;
    for (unsigned int i = 0; i < sdt->length; i++) {
        sum += ((char *)sdt)[i];
    }
    return sum == 0;
}

void init_acpi(void)
{
    char *magic = "RSD PTR ";
    // search for magic string in the EBDA
    uint16_t *ebda_segment = (uint16_t *)(0x40E);
    uint32_t *ebda_ptr = (uint32_t *)((*ebda_segment)<<4);
    for (int i = 0; i < 1024/4; i+=2) {
        if (ebda_ptr[i] == ((uint32_t *)magic)[0]) {
            if (ebda_ptr[i+1] == ((uint32_t *)magic)[1]) {
                rsdp = (acpi_rsd *)(ebda_ptr+i);
            }
        }
    }

    // search for magic string in high BIOS memory
    uint32_t *high_ptr = (uint32_t *)(0x000E0000);
    uint32_t *high_end = (uint32_t *)(0x000FFFFF);
    for (; high_ptr < high_end; high_ptr += 2) {
        if (high_ptr[0] == ((uint32_t *)magic)[0]) {
            if (high_ptr[1] == ((uint32_t *)magic)[1]) {
                rsdp = (acpi_rsd *)high_ptr;
            }
        }
    }

    if (rsdp == NULL) {
        print("no ACPI RSD PTR found\n");
        return;
    }
    // check the checksum
    int sum = 0;
    unsigned char *p = (unsigned char *)rsdp;
    for (unsigned int i=0; i < sizeof(acpi_rsd); i++) {
        sum += p[i];
    }
    if ((sum & 0xFF) != 0) {
        print("found ACPI RSD signature but checksum was invalid\n");
        rsdp = NULL;
        return;
    }

    rsdt = rsdp->rsdt_addr;
    printf("rsdt=%p length=%#x\n", rsdt, rsdt->length);
    if (!acpi_checksum(rsdt)) {
        print("found ACPI RSDT but checksum was invalid\n");
        rsdt = NULL;
        return;
    }

    num_sdts = (rsdt->length - sizeof(acpi_sdt))/4;
    sdts = (acpi_sdt **)(rsdt+1);
    for (int i = 0; i < num_sdts; i++) {
        acpi_sdt *sdt = sdts[i];
        if (memcmp(sdt->signature, "FACP", 4) == 0) {
            fadt = (acpi_fadt *)sdt;
        }
    }
    if (fadt == NULL) {
        print("didn't find ACPI FADT\n");
        return;
    }
    if (!acpi_checksum(&fadt->header)) {
        print("found ACPI FADT but checksum was invalid\n");
        fadt = NULL;
        return;
    }

    PM1aControlBlock = fadt->PM1aControlBlock;
    PM1bControlBlock = fadt->PM1bControlBlock;
    SMI_CommandPort = fadt->SMI_CommandPort;
    AcpiEnable = fadt->AcpiEnable;

    dsdt = fadt->dsdt_addr;
    if (!acpi_checksum(dsdt)) {
        print("found ACPI DSDT but checksum was invalid\n");
        dsdt = NULL;
        return;
    }

    // search for the \_S5 object in the DSDT
    // magic that I took from the osdev forums
    char *s5ptr = (char *)(dsdt+1);
    for (unsigned int i = 0; i < dsdt->length - sizeof(acpi_sdt) - 4; i++) {
        if (memcmp(s5ptr+i, "_S5_", 4) == 0) {
            s5ptr += i;
            break;
        }
    }
    if (memcmp(s5ptr, "_S5_", 4) != 0) {
        print("didn't find \\_S5 object in DSDT\n");
        return;
    }
    if (s5ptr[4] != 0x12 || (s5ptr[-1] != 0x08 && (s5ptr[-1] != '\\' || s5ptr[-2] != 0x08))) {
        print("invalid structure for \\_S5 object\n");
        return;
    }

    s5ptr += 5;
    s5ptr += ((*s5ptr &0xC0)>>6) +2;
    if (*s5ptr == 0x0A) s5ptr++;
    slp_typa = *(s5ptr)<<10;
    s5ptr++;
    if (*s5ptr == 0x0A) s5ptr++;
    slp_typb = *(s5ptr)<<10;

    can_acpi_shutdown = true;
}

void acpi_reclaim_memory(void)
{
    mboot_mmap_entry *e = boot_info->mmap_addr;
    for (; ((uint32_t)e) < ((uint32_t)boot_info->mmap_addr) + boot_info->mmap_length;
            e = (mboot_mmap_entry *)((uint32_t)e + e->size + 4)) {
        if (e->type != MBOOT_MMAP_TYPE_ACPI_RECLAIMABLE) continue;
        if (e->addr_hi != 0 || e->len_hi != 0) continue;
        if (e->addr_lo < 0x200000) continue;
        vaddr_t p = e->addr_lo;
        for (unsigned int i=0; i < (e->len_lo >> 12); i++) {
            vmm_free_page(p);
            p += 4096;
        }
    }
}

void acpi_enable(void)
{
    // check if acpi is enabled
    if ( (inportw((unsigned int) PM1aControlBlock) & 1) == 0 ) {
        // check if acpi can be enabled
        if (SMI_CommandPort != 0 && AcpiEnable != 0) {
            outportb(SMI_CommandPort, AcpiEnable); // send acpi enable command
            // give 3 seconds time to enable acpi
            int i;
            for (i=0; i<300; i++ ) {
                if ( (inportw((unsigned int) PM1aControlBlock) & 1) == 1 )
                    break;
                //sleep(10);
            }
            if (PM1bControlBlock != 0)
                for (; i<300; i++ ) {
                    if ( (inportw((unsigned int) PM1bControlBlock) & 1) == 1 )
                        break;
                    //sleep(10);
                }
            if (i<300) {
                //wrstr("enabled acpi.\n");
                //return 0;
            } else {
                //wrstr("couldn't enable acpi.\n");
                //return -1;
            }
        } else {
            //wrstr("no known way to enable acpi.\n");
            //return -1;
        }
    } else {
        //wrstr("acpi was already enabled.\n");
        //return 0;
    }
}

void acpi_shutdown(void)
{
    if (!can_acpi_shutdown) {
        print("ACPI is not enabled.\n");
        return;
    }
    acpi_enable();
    outportw(PM1aControlBlock, slp_typa | (1<<13));
    if (PM1bControlBlock != 0) {
        outportw(PM1bControlBlock, slp_typb | (1<<13));
    }
    print("ACPI shutdown failed.\n");
}
