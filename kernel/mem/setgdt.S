.global setGdt
.global reloadSegments

gdtr:
    .int 0 # For limit storage
    .long 0 # For base storage
 
setGdt:
    MOV   4(%esp), %EAX
    MOV   %EAX, (gdtr + 2)
    MOV   8(%esp), %AX
    MOV   %AX, (gdtr)
    LGDT  (gdtr)
    RET

reloadSegments:
    LJMP $0x08, $reload_CS
    reload_CS:
    MOV $0x10, %ax
    MOV %ax, %ds
    MOV %ax, %es
    MOV %ax, %fs
    MOV %ax, %gs
    MOV %ax, %ss
    RET
