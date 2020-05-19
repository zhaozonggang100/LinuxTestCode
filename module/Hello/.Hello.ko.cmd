cmd_/root/module/Hello/Hello.ko := ld -r -m elf_x86_64 -T ./scripts/module-common.lds --build-id  -o /root/module/Hello/Hello.ko /root/module/Hello/Hello.o /root/module/Hello/Hello.mod.o ;  true
