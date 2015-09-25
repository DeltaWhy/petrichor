.global setIdt

idtr:
    .int 0
    .long 0

setIdt:
    MOV   4(%esp), %EAX
    MOV   %EAX, (idtr + 2)
    MOV   8(%esp), %AX
    MOV   %AX, (idtr)
    LIDT  (idtr)
    RET
