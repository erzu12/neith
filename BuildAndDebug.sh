cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -B buildDebug

rm compile_commands.json
compdb -p buildDebug/ list > compile_commands.json

make -j 16 -C buildDebug

printf "\n\n=============================================================================\n\n"
#gdb build/testapp/src/testapp

