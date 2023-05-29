cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cp build/compile_commands.json .
cd build && make && cd ..
