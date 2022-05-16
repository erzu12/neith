cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -B build

rm compile_commands.json
compdb -p build/ list > compile_commands.json

make -j 16 -C build

printf "\n\n=============================================================================\n\n"
gdb build/testapp/src/testapp

