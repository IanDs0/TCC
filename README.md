### Instaling for Ubunto in Bash 
## install Rust

`curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh`




## install RISC-V and Xtensa targets

cargo install espup

espup install

cat $HOME/export-esp.sh >> ~/.bashrc




## install std Development Requirements

`cargo install ldproxy`



## Generating Projects from Templates

# Install cargo generate

`cargo install cargo-generate`

# Generate a project based in one of the templates

- esp-template

`cargo generate esp-rs/esp-template`

* esp-idf-template

`cargo generate esp-rs/esp-idf-template cargo`

# Build/Run the generated project

`cargo build`

ou 

`cargo run`


## Instaling espFlash

sudo apt update

sudo apt-get install libuv-dev

sudo apt install build-essential


cargo install espflash

cargo install cargo-espflash

## Permisions

ls -l /dev/ttyUSB0

sudo chmod a+rw /dev/ttyUSB0

sudo usermod -a -G dialout $USER
