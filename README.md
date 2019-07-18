# PathFind
Practice with command-line input, reading from a file, and dynamic memory allocation in C.

# Problem Statement
Read a set of topographic (land elevation) data from a 2D array and write a function to compute the path(s) through the mountains. For this project, the program is required to read input from the command-line into our main() function. Data passed into the main() comes through as char* argv[].

# Background
There are many contexts in which you want to know the most efficient way to travel over land. When traveling through mountains (let’s say you’re walking) perhaps you want to take the route that requires the least total change in elevation with each step you take, call it the path of least resistance. Given some topographic data it should be possible to calculate a "greedy" lowest-elevation-change walk from one side of a map to the other.

This project differs in that I am required to read the name of a data file from the command line, open the file, determine the size of the array required, then dynamically allocate both the 2d array topography[][] and the 1d array sumList[].

This program will read in a file encoded as a plain ascii text file from NOAA representing the average elevations of patches of land, with each number representing a path roughly 700 x 700 meters. The data file included with the project (topo983by450.txt)  is the elevation data for a region of the Southwestern United States. The data comes as one large, space-separated, list of integers. The sample file given has 442350 integers representing a grid of 983 columns by 450 rows. Each integer is the average elevation in meters of each cell in the grid.
