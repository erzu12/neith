cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build

rm compile_commands.json
compdb -p build/ list > compile_commands.json

make -C build

printf "\n\n=============================================================================\n\n"

cd build/tests/src/
./neithTest
