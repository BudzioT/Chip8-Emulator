# Chip8-Emulator

## :computer: Info
A Chip-8 emulator made in C++. It always you to access and play ROMS made for this system.
This emulator was written in C++, with CMake, using following libraries:<br>
- SDL2       v-2.28.5

## :hammer: How to build the project
- Download SDL2 and CMake.
(Example based off Ubuntu)
```
sudo apt install cmake
sudo apt-get install libsdl2-dev.dnf
```
  
- Make a directory for build
- Run cmake and direct it to the directory with CMakeLists.txt
- Run make
```
mkdir cmake-build
cd cmake-build
cmake ..
make
```
WINDOWS:
```
mkdir cmake-build
cd cmake-build
cmake .. -DSDL2_PATH="path/to/SDL2"
make
```
- Run the program with ROM path as the first argument.
```
./Chip8_Emulator path/to/ROM
```
- If you want more options, write --help as the first argument.
```
./Chip8_Emulator --help
```
  

## :camera:Screenshots

## :video_game: Controls

Chip-8 has hex keypad (0-F), it was converted to:
  | Chip-8 | Keyboard |
  | :----: | :------: |
  | `1`    | `1`      |
  | `2`    | `2`      |
  | `3`    | `3`      |
  | `4`    | `Q`      |
  | `5`    | `W`      |
  | `6`    | `E`      |
  | `7`    | `A`      |
  | `8`    | `S`      |
  | `9`    | `D`      |
  | `0`    | `X`      |
  | `A`    | `Z`      |
  | `B`    | `C`      |
  | `C`    | `4`      |
  | `D`    | `R`      |
  | `E`    | `F`      |
  | `F`    | `V`      |

## :page_facing_up: Links to libraries
SDL website: https://www.libsdl.org/<br>
SDL: https://github.com/libsdl-org/SDL/releases/tag/release-2.28.5<br>

