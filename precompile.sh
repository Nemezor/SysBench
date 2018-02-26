shopt -s nullglob
shopt -s globstar

echo -e "\n#### Pre-compiling Library ####\n"

find libcode -name '*' -type d -print0 | xargs -0 -I file bash -c 'mkdir precompile/file > /dev/null 2>&1'

asm_files=()
c_files=("pci/codes.c")

for file in ${asm_files}
do
    echo "A: libcode/$file"
    nasm -f elf64 libcode/$file -o precompile/libcode/$file.o
done

for file in ${c_files}
do
    echo "C: libcode/$file"
    gcc -m64 -ffreestanding -O3 -Wall -Wno-unused-parameter -Wno-unused-variable -Wextra -nostdlib -nostartfiles -nodefaultlibs -mcmodel=large -mno-red-zone -c libcode/$file -o precompile/libcode/$file.o
done
