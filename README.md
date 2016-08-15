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

Clone the repo:

```bash
git clone git@github.com:kbirk/pl.git
```

Build the makefiles:

```bash
cd pl
mkdir build
cd build
cmake ../
```

Build the executable:

```bash
make
```

## Usage

Run the executable with a plan file:

```bash
./planner ./resources/patient1/plan0.csv
```
