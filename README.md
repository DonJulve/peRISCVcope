# peRISCVcope

peRISCVcope is a simple rv32i interpreter written in C++ for teaching
purposes. Currently, it is in alpha status.

The full code is available upon request. Please e-mail @dariosg for getting
access to the source code and the lab handouts.

For a description of the architecture and the idea behind this project please
read the following paper [peRISCVcope: A Tiny Teaching-Oriented RISC-V
Interpreter](https://www.doi.org/10.1109/DCIS55711.2022.9970050).

If you want to cite this work, please use the following reference:
D. Suárez Gracia, A. Valero, R. G. Tejero, M. Villarroya and V. Viñals,
"_peRISCVcope: A Tiny Teaching-Oriented RISC-V Interpreter_," 2022 37th
Conference on Design of Circuits and Integrated Circuits (DCIS), Pamplona,
Spain, 2022, pp. 01-06, doi: 10.1109/DCIS55711.2022.9970050.

Feedback is welcome and appreciated.

## Cross-Compiler

    git clone https://github.com/riscv/riscv-gnu-toolchain.git
    cd riscv-gnu-toolchain
    ./configure --prefix=$HOME/usr/riscv/  --with-arch=rv32g --with-abi=ilp32d
    make

## References

* Good [ELF](https://www.ics.uci.edu/~aburtsev/238P/hw/hw3-elf/hw3-elf.html) explanations
* [RISC-V ELF](https://github.com/riscv-non-isa/riscv-elf-psabi-doc/)

## Code completed by Javier Julve Yubero (student of the GyS subject)

## How to compile

1. Clone the repository
   ```sh
    git clone https://github.com/DonJulve/peRISCVcope.git
    cd peRISCVcope
   ```

2. Give execution permissions to the build.sh script
   ```sh
    chmod +x build.sh
   ```

3. Use build.sh script to compile the project
   ```sh
    ./build.sh
   ```
    
## How to run the examples

1. Go to the binary location
   ```sh
   cd build-debug/src
    ```
3. Use one of the following commands:
    ```sh
    ./periscvcope ../../examples/add_array
    ./periscvcope ../../examples/factorial
    ```
