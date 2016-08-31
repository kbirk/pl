# pl

> Mosaic Arthroplasty Surgical Planner

## Dependencies

#### Linux

Install CMake:

```bash
sudo aptitude install cmake
```

Install Dependencies:

```bash
sudo aptitude install libsdl2-dev
sudo aptitude install libepoxy-dev
```

## Build

Build the makefiles:

```bash
mkdir build
cd build
cmake ..
```

Compile and link the executable:

```bash
make
```

## Usage

Run the executable with a plan file:

```bash
./planner ./resources/patient1/plan0.csv
```
