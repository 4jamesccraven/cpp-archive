default: build

build:
    if [ ! -d ./build ]; then cmake -S . -B build; fi
    cmake --build ./build --parallel $(nproc)

run ARGS: build
    @./build/proj {{ ARGS }}

clean:
    if [ -d ./build ]; then rm -fr ./build; fi
