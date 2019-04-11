### Requirements

* Create a program that takes in match statistics from a hide-and-seek game and generates heatmaps and other stats.

#### Outputs

* How many hiders were found while facing the seeker within a 90 degree cone of vision?
* How many hiders were in partial or full cover by obstacles from the seeker's point of view?
* A heatmap of locations where players were found
* A heatmap of locations where players were found while facing the seeker (as explained above)
* A heatmap of locations where players were found where the hider was in partial or full cover (as explained above)

A heatmap should be printed as an `M*N` sized 2D grid of integers.

#### Command-line Inputs

`<X> <Y> <Width> <Height> <M> <N>`

* X, Y - Position in world where center of heatmap sample area should be
* Width, Height - Size of heatmap sample area
* M, N - Resolution of heapmap output grid
  * Each "cell" of the heatmap will represent an area `Width / M` units wide and `Height / N` units tall

#### Given

* Records of when players were "found" during a game. These contain:
  * World position of Seeker
  * Rotation of Seeker (given as an int32 between 0 and 1024)
  * World position of the found Hider
  * Rotation of Hider (given as an int32 between 0 and 1024)
  * Number of seconds that had passed since the beginning of the game when the Hider was found
* Level geometry data:
  * A collection of geometric shapes and their position in the world
    * Circles:
      * Center position (X, Y)
      * Radius
    * Rectangle
      * Center position (X, Y)
      * Width on X axis
      * Depth on Y axis

## Building the code

* The Boost C++ Libraries are installed on the building machine at `D:\local\boost_1_69_0` using [this installer for pre-compiled binaries](https://bintray.com/boostorg/release/download_file?file_path=1.69.0%2Fbinaries%2Fboost_1_69_0-msvc-14.1-64.exe)
  * Other boost installs will (probably) work but *Properties > VC++ Directories > Include Directories* and *Properties > VC++ Directories > Library Directories* settings of both projects will need to be changed accordingly
