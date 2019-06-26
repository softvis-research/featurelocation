# featurelocation
Calculates all elementary sets for configurations of the SPLC feature location challenge 2019

Program FEATURELOCATION
Version 1.0
Author: Ulrich Eisenecker
Date: June 2019
Purpose: Calculates all elementary sets for configurations of the SPLC feature location challenge

Files required for compilation

1. featurelocation.cpp
2. kombinatorik.cpp
3. kombinatorik.h

For compilation use g++ 9.1.0
Take care to add "-lstdc++fs" under "Project build options" --> "Linker settings" --> "Other linker options"

Warnings about comparing integer expressions of different signedness can be safely ignored.

Execution:

./featurelocation PATH_TO_CONFIG_DIR PATH_TO_OUTPUT_FILE

PATH_TO_CONFIG_DIR should contain all configurations of a given scenario.Each configuration file
shoould specify a certain configuration using all files. Features, which are not included in the
configuration, shouöd be prefix with "not_".
Important: The directoy with configurations should not contain any other files than configuration files,
because the program featurelocatin will try to interpret them as configuration files.

The output file describes for each elementary subset of the given configuration how it can be computed 
and to which extent this can be done.
Each subset is printed in a new line.
If a subset can be computed precisely the line start with "+++ ", otherwise it starts with "--- ". 
Afterwards it follows an internal ordinal number for the subsets starting with zero. After that the
subset is specified.

Any set enclosed in "[]" denotes a set which only exists if its elemens (= feature names) are present in
the given configuration. This corresponds to "IF (feature_a AND feature_b) ..." in the code.
Any set enclosed in "{}" denots a set with features as elements. If the set contains more than one element,
this corresponds to "IF (feature_a OR fetaure_b) ..." in the code.
Sets enclosed in "()" contain other sets as elements and correspond to systems. In this case the
system is described in terms of all eleentary subsets it consists of.

Well, after the ordinal number the corresponding elementary subset is printed. The an equal signs follows.
Obviosly, this is only adequate, when the line starts with "+++ ".
Afterwards the keyword "INTERSECTION" all systms are printed which contain the specific subset and which thus 
should be intersetced. Then all corresponding systems are printed. The intersection of these systems has to
be set diminished (operator "\") by all the systems following the keyword "UNION". These systems do
not contain the specific subset, hence, their union has to be computed.
After that follows again an equal sign followed by the result set. If the result set contains exactly one
element, the specific subset ca be exactly determind in the goven scenario defined by its configurations.
If it has more than one element, the specific subset is among these elements, but can not be exactly
computed.




