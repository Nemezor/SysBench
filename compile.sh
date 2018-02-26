shopt -s nullglob
shopt -s globstar

echo -e "\n#### Compiling ####\n"

cp -a precompile/* workdir/

./para

echo -e "\n#### Linking ####\n"

ld -z max-page-size=0x1000 -O0 -T link.ld -o workdir/kernel.bin workdir/code/**/*.o -nostdlib -Lheaders
du -sh workdir/kernel.bin

echo -e "\n#### Creating ISO ####\n"

mkdir -p workdir/iso/boot/grub
touch workdir/iso/boot/grub/grub.cfg
echo -e "set default=0\n" >> workdir/iso/boot/grub/grub.cfg
echo -e "set timeout=2\n" >> workdir/iso/boot/grub/grub.cfg
echo -e "menuentry \"SysBench\" {\n" >> workdir/iso/boot/grub/grub.cfg
echo -e "\tmultiboot /boot/kernel.bin\n" >> workdir/iso/boot/grub/grub.cfg
echo -e "}" >> workdir/iso/boot/grub/grub.cfg
mv workdir/kernel.bin workdir/iso/boot/
grub-mkrescue -o boot.iso workdir/iso

echo -e "\n#### Cleaning up ####\n"
#find workdir -name '*.asm.o' -print0 | xargs -0 -I file bash -c 'echo "A: file"; rm file;'
#find workdir -name '*.c.o' -print0 | xargs -0 -I file bash -c 'echo "C: file"; rm file;'
rm -r workdir/*
