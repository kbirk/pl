# balthazar


## Dependencies

#### Linux

Install CMake:

```bash
sudo aptitude install cmake
```

Install Dependencies:

```bash
sudo aptitude install libxmu-dev
sudo aptitude install libxi-dev
sudo aptitude install freeglut3-dev
sudo aptitude install libepoxy-dev
```

#### OSX

Install CMake:

```bash
brew install cmake
```

Install Dependencies:

```bash
brew install libepoxy
```

## Build

Clone the repo:

```bash
git clone git@github.com:kbirk/balthazar.git
```

Build the makefiles:

```bash
cd balthazar
mkdir build
cd build
cmake ../
```

Build the executable:

```bash
make
```

## Usage

Run the executable:

```bash
./balthazar
```
