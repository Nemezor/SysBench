echo -e "\n#### Running ####\n"

qemu-system-x86_64 -vga std -serial stdio -drive file=boot.iso,if=none,id=drive-ide0-0-1,readonly=on -device ide-cd,bus=ide.0,unit=1,drive=drive-ide0-0-1,id=ide0-0-1,bootindex=1 -m 8192 -smp 4,sockets=1,cores=4,threads=1
