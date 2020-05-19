cmd_/root/module/Hello.ko := ld -r -m elf_x86_64 -T ./scripts/module-common.lds --build-id  -o /root/module/Hello.ko /root/module/Hello.o /root/module/Hello.mod.o
