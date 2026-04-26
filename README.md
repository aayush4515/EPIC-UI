# EPIC-UI
This is a UI for the EPIC SLURM cluster.


## Installation:
This requires the Qt package to build and run.

`sudo dnf install qt-creator qt6-qtbase-devel`
Downloads Qt.

Then, create a new build folder
`mkdir build`
and 
`cd build`

Now run cmake
`cmake ..`
`cmake --build .`

and run the UI
`./EPIC_UI`
