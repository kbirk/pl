# pl

[![Build Status](https://travis-ci.org/kbirk/pl.svg?branch=master)](https://travis-ci.org/kbirk/pl)

> Mosaic Arthroplasty Surgery Planner

## Description

Mosaic arthroplasty is a surgical procedure to repair a cartilage defect. Small plugs of cartilage and bone are transplanted into the defect from a non-load-bearing part of the joint.

This codebase represents an implementation of [cartilage surface prediction](
http://research.cs.queensu.ca/home/jstewart/papers/surfacePrediction.pdf) and [automated planning of mosaic arthroplasty](http://research.cs.queensu.ca/home/jstewart/papers/planning.pdf) developed by the
[Joints and Connective Tissues Laboratory](http://research.cs.queensu.ca/home/jstewart/research.html) under the supervision and guidance of [James Stewart](http://research.cs.queensu.ca/home/jstewart/).

<p align="center">
    <img width="600" src="https://rawgit.com/kbirk/pl/master/screenshot.png" alt="pl" />
</p>

## Dependencies

- C++11 compiler (g++ 5.1+ or clang 3.8+)
- GPU driver support for OpenGL 4.3+

#### Linux

Install CMake:

```bash
sudo aptitude install cmake
```

Install dependencies:

```bash
sudo aptitude install clang-format
sudo aptitude install libsdl2-dev
sudo aptitude install libepoxy-dev
```

#### OSX

Install CMake:

```bash
brew install cmake
```

Install dependencies:

```bash
brew install clang-format
brew install sdl2
brew install libepoxy
```

**NOTE:** OSX only has driver support for OpenGL 4.1, therefore the GPU planning functionality is disabled.

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

## Controls

#### Exit

    ESC:         Exits the application.

#### Camera Controls:

    0-9:         Changes the view input / output file number.
    SHIFT+R:     Read in an existing view file corresponding the the # set with 0-9.
    SHIFT+W:     Write the existing view to a file corresponding the the # set with 0-9.
    SPACEBAR:    Toggles between `rotation` and `translation` camera modes.
    Z:           Reset camera to model.

#### Boundary Controls:

    SHIFT+D:     Creates a new donor boundary site.
    SHIFT+S:     Creates a new defect boundary site.
    DELETE:      Removes a selected boundary point.
    CTRL+DELETE: Removes a selected boundary.

#### Plan Controls:

    SHIFT+N:     Clears the existing plan.
    SHIFT+O:     Exports the plan file.
    SHIFT+P:     Generate a new plan.
    P:           Toggle plan visibility.
    M:           Update graft marker positions.
    R:           Set graft edit mode to `rotation`
    T:           Set graft edit mode to `translation`
    V:           Toggle selected component visibility.
