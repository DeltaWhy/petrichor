.global loader                          # making entry point visible to linker
.global stack_top
.global stack_bottom

# setting up the Multiboot header - see GRUB docs for details
.set ALIGN,    1<<0                     # align loaded modules on page boundaries
.set MEMINFO,  1<<1                     # provide memory map
.set FLAGS,    ALIGN | MEMINFO          # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002               # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS)         # checksum required

.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# reserve initial kernel stack space
stack_bottom:
.skip 16384                             # reserve 16 KiB stack
stack_top:
.comm boot_info, 4

loader:
    movl  $stack_top, %esp               # set up the stack, stacks grow downwards
    movl  %ebx, boot_info

    push  %eax
    call  kmain                         # call kernel proper

    cli
hang:
    hlt                                 # halt machine should kernel return
    jmp   hang
