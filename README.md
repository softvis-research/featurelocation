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
Each configuration file should specify a certain configuration using all features.
Features, which are not included in the configuration, can be simply omitted in the config file, or they may be prefixed with `not_`.  
**Important:** The directory with configurations should not contain any other files than configuration files, because the program will try to interpret them as configuration files.

Here is an example of a config file named `config.001` using a consequent `not_`-specification to explicitly express, that features `A`, `B`, and `C` are not present:
`not_A`
`not_B`
`not_C`
The next example, the file name is `config.004` contains only one line, namely `C`.
This indicates, that feaure `C` ist present, while all non-specified features are absent.

## Output Format

The output file describes for each elementary subset of the given configuration how it can be computed  and to which extent this can be done.
Each subset is printed in a new line.
If a subset can be computed precisely the line start with `+++ `, otherwise it starts with `--- `.  
Here is an example line of an elementary subset, which can be precisely computed:  
`+++ 4 { core }  = INTERSECTION ( { core } ) \ UNION ( ) = ( { core } )`

The next token after `+++` is  an internal ordinal number identifying the subsets.
The first subset has the internal ordinal number `0`.
This number is followed by a specification of this subset, here `{ core }`.
The element name `core` is reserved for the core system without any feature activated.
After the equal sign, `=`, it is described how to compute the subset, which is pretty simple here.
The left term `INTERSECTION ( { core } )` means, that the systems contained in the set of systems, which is embraced by `()`, have to be intersected. Since the only element of this systems set is „{ core }“, this is a very simple operation. Now follows the operator for set difference, `\`.
The right term is a list of systems, which have to be united.
Here this list ist empty.
Again, an equal sign follows.
Eventually the set of systems is printed.
If the subset of interest can be precisely computed, this systems set contains only one element, namely the subset itself.
Now we take a look at an example line with an elementary subset, which can not be exactly computed:  
`--- 0 [ 3 2 ]  = INTERSECTION ( [ 3 2 ] [ 3 2 1 ] [ 3 1 ] [ 2 1 ] { core } { 1 } { 1 2 } { 1 2 3 } { 1 3 } { 2 } { 2 3 } { 3 } ) \ UNION ( { core } { 1 } { 1 2 } { 1 2 3 } { 1 3 } { 2 } { 2 3 } { 3 } ) = ( [ 3 2 ] [ 3 2 1 ] [ 3 1 ] [ 2 1 ] )`

The interesting subset is `[ 3 2 ]`. 
This set is enclosed in square brackets which indicates, that this set exists only, if both features, `3` and `2`, are present in the given system.
It refers to code, which is specific to the interaction of feature `3` and feature `2`.
It follows the set of systems containing this special subset. 
All of these systes have to be intersected. 
From the resulting set the union of all systems not containing this special subset will be subtracted. 
Obviously, the resulting contains not only the specific subset, but three more sets. 
This means, that this set comes closest to the specific subset, but the subset itself can not be precisely computed.
Two more explanations: The set `{ 1 3 }` denotes code which is included for either feature `1`, for feature `3`, or for both features.
The set `{ 1 }` refers  to code which is exclusively specific to feature `1`.  
To summarize:  
`+++` starts a line of an elementary subset, which can be precisely computed.  
`---` starts a line of an eleentary subset, whicht can not be precisely computed.  
`{ }` curly brackets enclose ordinary sets of pure feature-code or or-feature-code.  
`[  ]` square brackets enclose sets of code specific to interaction of certain features.  
`()` parentheses enclose sets of systems.

