# Feature Location

Calculates all elementary sets for configurations of the [SPLC feature location challenge 2019](https://variability-challenges.github.io/2018/ArgoUMLSPL/index.html).

Author: [Ulrich Eisenecker](http://home.uni-leipzig.de/svis/Research%20Group/#UlrichEisenecker)

## Compilation

Execute `g++ featurelocation.cpp kombinatorik.cpp -lstdc++fs`

Beware: Tested only with g++ 9.1.0  
Warnings about comparing integer expressions of different signedness can be safely ignored.

## Usage

Execute `./featurelocation PATH_TO_CONFIG_DIR PATH_TO_OUTPUT_FILE`

`PATH_TO_CONFIG_DIR` should contain all configurations of a given scenario.
Each configuration file should specify a certain configuration using all files.
Features, which are not included in the configuration, should be prefix with `not_`.  
**Important:** The directory with configurations should not contain any other files than configuration files, because the program will try to interpret them as configuration files.

## Output Format

The output file describes for each elementary subset of the given configuration how it can be computed  and to which extent this can be done.
Each subset is printed in a new line.
If a subset can be computed precisely the line start with `+++ `, otherwise it starts with `--- `. 
Afterwards it follows an internal ordinal number for the subsets starting with zero. 
After that the subset is specified.

Any set enclosed in `[]` denotes a set which only exists if its elements (= feature names) are present in the given configuration. 
This corresponds to `IF (feature_a AND feature_b) ...` in the code.
Any set enclosed in `{}` denotes a set with features as elements. 
If the set contains more than one element, this corresponds to `IF (feature_a OR feature_b) ...` in the code.
Sets enclosed in `()` contain other sets as elements and correspond to systems. 
In this case the system is described in terms of all elementary subsets it consists of.

Well, after the ordinal number the corresponding elementary subset is printed. 
Then an equal signs follows.
Obviously, this is only adequate, when the line starts with `+++ `.
Afterwards the keyword `INTERSECTION` all systems are printed which contain the specific subset and which thus should be intersected. 
Then, all corresponding systems are printed. 
The intersection of these systems has to be set diminished (operator `\`) by all the systems following the keyword `UNION`. 
These systems do not contain the specific subset, hence, their union has to be computed.
After that follows again an equal sign followed by the result set.
If the result set contains exactly one element, the specific subset ca be exactly determined in the given scenario defined by its configurations.
If it has more than one element, the specific subset is among these elements, but can not be exactly computed.
