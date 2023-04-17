# The CUTCP Benchmark

A summary of ["Acceleration of Cutoff Pair Potentials for Molecular
Modeling Applications"](https://dl.acm.org/doi/abs/10.1145/1366230.1366277).  

## Introduction  

Molecular modeling allows for an increased understanding of molecular structures; it can be studied on highly parallel architectures, such as GPUs.  

The most costly computation in molecular modeling applications is the calculation of nonbonded pair potentials, due the electrostatic and van der Waals interactions between pairs of atoms; this complexity is quadratic, but can be made linear by introducing a cutoff distance, beyond which the pair potential is set to zero.  

## Background

The current simulations comprise 100,000 to 1,000,000 atoms, and simulate for long timescales, of 100 ns (100 million time steps).

### Cutoff Pair Potentials in Molecular Modeling

In molecular modeling, the molecules are governed by Newton's laws, and are subject to distance-dependent forces exerted by the other nonbonded atoms.  

The work due to bonded forces grows linearly with the number of atoms, whereas the number of nonbonded interactions increases quadratically as the number of
atomic pairs in the system; this complexity has been reduced by using a cutoff pair potential in which the pairwise interaction is zero beyond a cutoff distance.  
Forces must be calculated at each timestep, which are in the order of femtoseconds.  

Each atom _i_ is modeled as a point charge, with position _r<sub>i</sub>_ and partial charge _q<sub>i</sub>_.  

The equation for the electrostatic potential in position _r<sub>x</sub>_, given the charges in _r<sub>1</sub>_,...,_r<sub>N</sub>_ is  

TODO  

where the coefficient _s(|r<sub>i</sub>-r<sub>x</sub>|) = s(r)_, can be defined according to the methodology; in a cutoff implementation, _s(r)_ is 0 if _r >= r<sub>cutoff</sub>_.  
The potential is calculated over _M_ regularly spaced lattice points, to generate an electrostatic potential map of the volume.  

Additionally, nonbonded Van Der Waals interactions and weak dipole attraction are also computed, with a similar cutoff coefficient.  

In this paper two algorithmic solutions are considered:  
1. The **direct summation** approach with _s(r) = 1_ (or cutoff distance infinity), whose theoretical complexity is _O(MN)_.  
2. The **cutoff** approach with _s(r) = 0 if r >= r<sub>cutoff</sub>_, whose theoretical complexity is _O(M+N)_.  

The basic algorithm for evaluating a cutoff pair potential involves performing spatial hashing of the atoms into bins of a uniform grid (**broad phase**), then considering for each bin its interactions with nearby bins that lie within or along the cutoff distance (**narrow phase**).  

## Implementation  

The volume is decomposed into planes, and each plane is computed by a separate kernel call.  
Each plane is divided into tiles, and each tile is computed by a thread block.  
Each tile contains 8 points, and each point is computed by a thread.  
