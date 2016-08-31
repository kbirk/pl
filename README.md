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
