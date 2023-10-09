cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -B build
printf "\n\n=============================================================================\n\n"
rm compile_commands.json
compdb -p build/ list > compile_commands.json
printf "\n\n=============================================================================\n\n"
# mv build/compile_commands.json .
# make CXXFLAGS="-O2" -j 16 -C build
cmake --build build --config Release -j 16
printf "\n\n=============================================================================\n\n"

mangohud --dlsym build/testapp/src/testapp
#./build/testapp/src/testapp
