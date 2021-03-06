# newOopsApp
#### application for daily use by newspaper-couriers and call-centers
#### "Oops"-situation for distribution it is missing items (newspapers, flyers, journals etc.) at the client's frontdoor

![newOopsApp1_subs](https://user-images.githubusercontent.com/70653782/132023831-d4a7c78d-d4ff-4e22-9e18-a2e07663ee74.gif)

#### newOopsApp allows to send information with predefined options directly to support (message or EMail, for example)

![newOopsApp2](https://user-images.githubusercontent.com/70653782/132024356-1169d271-6e34-4414-a09d-65bb64b00d73.gif)


Draft of a kind-of-blueprint/scheme:
![newOopsApp](https://user-images.githubusercontent.com/70653782/128698923-9eb79366-3add-4f3f-868c-4cbb882e8a13.png)



## How to use
newOopsApp uses CMake to generate a set of Makefiles for the build process. 

To generate these Makefiles and build newOopsApp (on Linux), perform the following steps:

```bash
# Clone this repository
$ git clone https://github.com/CubeElement/newOopsApp

# Go into the repository
$ cd cube-tools

# Create and change to a new, empty build directory by running mkdir build, cd build: 
$ mkdir Build
$ cd Build/

# Run cmake
$ cmake ..

# Start the build process
$ make

# Start the application
$ ./newOopsApp
```
