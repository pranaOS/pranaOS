extern swift_msg_lookup
global swift_msgSend

swift_msgSend:
    call swift_msg_lookup
    jmp [eax]