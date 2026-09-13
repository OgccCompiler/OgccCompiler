# GPU C-Compiler

A high-performance C compiler designed to optimize and compile code directly for GPU execution.

---

## Features
* GPU Acceleration: Leverages parallel processing power.
* C Standard Support: Implements core features of the C language.
* Parallelization: Automatically optimizes loops for parallel execution.

## Download and Installation

### Precompiled Binaries
If you want to use the compiler without building it from source, you can download the ready-to-use installers (including .deb packages for Linux and install.exe for Windows) from our website:

[Download Precompiled Binaries](https://URL)

### Building From Source
Before building the compiler, ensure you have a host C compiler (GCC or Clang), CMake (Version 3.10 or higher), and your target GPU SDKs installed.

Clone the repository and build the project:

```bash
git clone https://github.com/OgccCompiler.git
cd OgccCompiler
mkdir build && cd build
cmake ..
make
```

## Usage
To compile a C file using this GPU compiler, run the following commands:

```bash
./ogcc -o my_program my_program.c
./my_program
```

## Contributing
Contributions are welcome. Since this project is licensed under the LGPLv2.1, please follow these guidelines:
1. Fork the repository.
2. Create a new feature branch (git checkout -b feature/your-feature).
3. Commit your changes.
4. Open a Pull Request.

Note: Modifying the core compiler toolchain requires those specific changes to remain open source under the LGPLv2.1 terms.

## License
This project is licensed under the GNU Lesser General Public License v2.1 (LGPLv2.1). See the LICENSE file for details.
