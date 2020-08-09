# Interview-Prep
A bunch of custom-implemented algorithms and data-structures. Made using C++, Python, and Javascript to prep myself for interviews.
### Requires c++17 to compile
In the terminal:
    g++ --std=c++17 binaryTrees.cpp
    ./a.out

## Contains
- DataStructure
- Algorithms
- Timing Utilites for Benchmarking
- General exercises for solidifying my knowledge of various aspects of a given language

## My Favorite Examples
- lifeCycle.cpp
- binaryTrees.cpp
- graph.cpp
- sort2.cpp

### Example output from compiling/running lifeCycle.cpp
      Constructed 1 (Foo)
      Constructed 2 based on 1 (Copy: Foo)
      Constructed 3 based on 2 (Move: Foo)
      Constructed 4 (Bar)
      Assigned 1 = 4 (Copy: Bar)
      Assigned 2 = 4 (Move: Bar)

    Constructions: 4
      2 default
      1 copy
      1 move
    Assigns: 2
      1 copy
      1 move

      Destroyed 4 (* moved *)
      Destroyed 3 (Foo)
      Destroyed 2 (Bar)
      Destroyed 1 (Bar)

### Example output from compiling/running binaryTrees.cpp
    Creating binary tree with 10000000 random numbers
    Total Duration: 12194706us | 12194.7ms
    Depth: 55
    Nodes: 6331031
    Min Possible Depth: 23

    Searching for a random number
    Testing 1000000 times
    Total Duration: 1393282us | 1393.28ms
    Average duration per call: 1us | 0.001ms
    Average find: 0.994759

    Rebasing...
    Total Duration: 3317548us | 3317.55ms
    Depth: 23
    Nodes: 6331031
    Min Possible Depth: 23

    Searching for a random number
    Testing 1000000 times
    Total Duration: 1373707us | 1373.71ms
    Average duration per call: 1us | 0.001ms
    Average find: 0.994807
    
### Example output from compiling/running graph.cpp
    
    ---------------------------------------------------------------------------------------
    Depth-First-Search. Unlimited Node Visits. Limited Connection Uses.
    ---------------------------------------------------------------------------------------

    Searching for Connections between Portland and New York City...
    Portland -> Los Angeles -> New York City | 7.4
    Portland -> Los Angeles -> Barcelona -> Miami -> New York City | 27.2
    Portland -> Los Angeles -> Barcelona -> Miami -> Las Vegas -> Miami -> New York City | 33.6
    Portland -> Los Angeles -> Barcelona -> Miami -> Las Vegas -> Los Angeles -> New York City | 34.4
    Portland -> Los Angeles -> Barcelona -> Los Angeles -> New York City | 33.8
    Portland -> Los Angeles -> Barcelona -> London -> New York City | 26.9
    Portland -> Los Angeles -> Seattle -> New York City | 9.6
    Portland -> Los Angeles -> Seattle -> Los Angeles -> New York City | 13.8
    Portland -> Seattle -> New York City | 5.4

    Searching for Connections between Los Angeles and London...
    Los Angeles -> Barcelona -> London | 15.4
    Los Angeles -> Barcelona -> Miami -> Barcelona -> London | 34.2
    Los Angeles -> Barcelona -> Miami -> New York City -> London | 34.1
    Los Angeles -> Barcelona -> Miami -> New York City -> Seattle -> New York City -> London | 42.3
    Los Angeles -> New York City -> London | 14.3

    Searching for Connections between Portland and Iceland...
    There are none :/

    ---------------------------------------------------------------------------------------
    Breadth-First-Search. Unlimited Node Visits. Limited Connection Uses.
    ---------------------------------------------------------------------------------------

    Searching for Connections between Portland and New York City...
    Portland -> Seattle -> New York City | 5.4
    Portland -> Los Angeles -> New York City | 7.4
    Portland -> Las Vegas -> Los Angeles -> New York City | 8
    Portland -> Las Vegas -> Miami -> New York City | 7.2
    Portland -> Seattle -> Los Angeles -> New York City | 9.6
    Portland -> Los Angeles -> Seattle -> New York City | 9.6
    Portland -> Los Angeles -> Barcelona -> London -> New York City | 26.9
    Portland -> Los Angeles -> Barcelona -> Los Angeles -> New York City | 33.8
    Portland -> Los Angeles -> Barcelona -> Miami -> New York City | 27.2

    Searching for Connections between Los Angeles and London...
    Los Angeles -> New York City -> London | 14.3
    Los Angeles -> Barcelona -> London | 15.4
    Los Angeles -> Seattle -> New York City -> London | 16.5
    Los Angeles -> Las Vegas -> Miami -> New York City -> London | 15.9
    Los Angeles -> Las Vegas -> Miami -> Barcelona -> London | 16

    Searching for Connections between Portland and Iceland...
    There are none :/

### Example output from compiling/running sort2.cpp

    All tests performed on unsorted lists of 10000 random numbers

    Bubble Sort: 
    Testing 10 times
    Total Duration: 16021992us | 16022ms
    Average duration per call: 1602199us | 1602.2ms

    Selection Sort: 
    Testing 10 times
    Total Duration: 2682276us | 2682.28ms
    Average duration per call: 268227us | 268.227ms

    Merge Sort:
    Testing 10 times
    Total Duration: 84763us | 84.763ms
    Average duration per call: 8476us | 8.476ms

    Quick Sort:
    Testing 10 times
    Total Duration: 34483us | 34.483ms
    Average duration per call: 3448us | 3.448ms

    Inefficient Search:
    Testing 1000 times
    Total Duration: 32832us | 32.832ms
    Average duration per call: 32us | 0.032ms
    Success Ratio: 0.662

    Binary Search: (Uses a sorted list)
    Testing 1000 times
    Total Duration: 484us | 0.484ms
    Average duration per call: 0us | 0ms
    Success Ratio: 0.625
