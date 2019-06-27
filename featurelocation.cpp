#include <iostream>
#include <experimental/filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <set>
#include "kombinatorik.h"

using namespace std;
using namespace experimental::filesystem;

const string negate_feature = "not_";
typedef vector<string> featuresType;

string purifyFeaturename(const string& line)
{
    string name = line;
    if (line.find(negate_feature) == 0)
        name = line.substr(negate_feature.size(),string::npos);
    while (name.find(" ") == 0)
        name = name.substr(1,string::npos);
    return name;
}

// unprocessed line assumed!
bool featureIsDeselected(const string& line)
{
    if (line.find(negate_feature) == 0)
        return true;
    else
        return false;

}

// unprocessed line assumed!
bool featureIsSelected(const string& line)
{
    return !featureIsDeselected(line);
}

bool featureAlreadyKnown(const featuresType& features,const string& featurename)
{
    if (find(features.begin(),features.end(),featurename) != features.end())
        return true;
    else
        return false;
}

bool featureUnknown(const featuresType& features,const string& featurename)
{
    return !featureAlreadyKnown(features,featurename);
}

featuresType preprocessConfigfiles(const string& pathToConfigs)
{
    unsigned configfiles = 0;
    featuresType features;
    for(auto& p: directory_iterator(pathToConfigs))
    {
        ++configfiles;
        ifstream config(p.path());
        string featurename;
        while (config)
        {
            config >> featurename;
            if (config)
            {
                featurename = purifyFeaturename(featurename);
                if (featurename != "")
                {
                    if (featureUnknown(features,featurename))
                        features.push_back(featurename);
                }
            }
        }
     }
    sort(features.begin(),features.end());
    clog << "preprocessed " << configfiles << " configurations with " << features.size() << " features." << endl;
    return features;
}

typedef set<int> elementarySetType;
typedef set<elementarySetType> setOfElementarySetsType;
typedef set<setOfElementarySetsType> setOfSystemsType;

void print(const elementarySetType& aSet,const featuresType& features,ostream& output = cout)
{
    const char beginSet = '{';
    const char endSet = '}';
    const char beginInteractionSet = '[';
    const char endInteractionSet = ']';
    bool interactionSet = false;
    if (aSet.size() > 0 && *(aSet.begin()) < 0)
        interactionSet = true;
    output << (interactionSet == true ? beginInteractionSet : beginSet) << ' ';
    for (auto elementP = aSet.begin(); elementP != aSet.end(); ++elementP)
    {
        auto element = (*elementP < 0 ? -*elementP : *elementP);
        output << (element == 0 ? "core" : features.at(element - 1)) << ' ';
    }
    output << (interactionSet == true ? endInteractionSet : endSet) << ' ';
    output << flush;
}

void print(const setOfElementarySetsType& aSet,const featuresType& features,ostream& output = cout)
{
    const char beginSet = '(';
    const char endSet = ')';
    output << beginSet << ' ';
    for (auto elementP = aSet.begin(); elementP != aSet.end(); ++elementP)
    {
        print(*elementP,features,output);
    }
    output << endSet << flush;
}

setOfElementarySetsType generateAllElementarySubsets(size_t numberOfFeatures)
{
    setOfElementarySetsType subsets;
    for (unsigned int i = 0; i <= numberOfFeatures; ++i)
    {
        elementarySetType aSet;
        aSet.insert(i);
        subsets.insert(aSet);
    }
    for (unsigned int i = 2; i <= numberOfFeatures; ++i)
    {
        UrneOWOR u(numberOfFeatures,i);
        for (unsigned j = 0; j < u.ziehungen(); ++j)
        {
            elementarySetType aSet,interactionSet;
            for (unsigned k = 0; k < u.k(); ++k)
            {
                aSet.insert(u.element(k) + 1);
                interactionSet.insert(-(u.element(k) + 1)); // treat interactions in a special way
            }
            subsets.insert(aSet);
            subsets.insert(interactionSet);
            u.naechsteZiehung();
       }
    }
    clog << subsets.size() << " elementary subsets generated." << endl;
    return subsets;
}

setOfSystemsType generateAllSystems(size_t numberOfFeatures,const setOfElementarySetsType& elementarySubsets)
{
    setOfSystemsType systems;
    elementarySetType coreSet;
    coreSet.insert(0);
    setOfElementarySetsType core;
    for (unsigned int i = 0; i <= numberOfFeatures; ++i)
    {
        setOfElementarySetsType aSystem;
        aSystem.insert(coreSet);
        for (auto k = elementarySubsets.begin(); k !=elementarySubsets.end(); ++k)
        {
            if (k->find(i) != k->end())
                aSystem.insert(*k);
        }
        systems.insert(aSystem);
    }
    for (unsigned int i = 2; i <= numberOfFeatures; ++i)
    {
        UrneOWOR u(numberOfFeatures,i);
        do
        {
            setOfElementarySetsType aSystem;
            for (unsigned j = 0; j < u.k(); ++j)
            {
                aSystem.insert(coreSet);
                for (auto k = elementarySubsets.begin(); k !=elementarySubsets.end(); ++k)
                {
                    auto m = k->find(u.element(j) + 1);
                    if (m != k->end())
                    {
                        aSystem.insert(*k);
                    }
                }
            };
            elementarySetType interactionSet;
            for (unsigned l = 0; l < u.k(); ++l)
            {
                interactionSet.insert(-int(u.element(l) + 1));
            }
            aSystem.insert(interactionSet);
            for (auto l = 2; l < u.k(); l++)
            {
                UrneOWOR i_u(u.k(),l);
                if (i_u.ziehungen() > 0)
                do
                {
                    elementarySetType is;
                    for (auto m = 0; m < i_u.k(); ++m)
                    {
                        is.insert(-int(u.element(i_u.element(m)) + 1));
                    }
                    aSystem.insert(is);
              } while (i_u.naechsteZiehung());
            }
            systems.insert(aSystem);
       } while (u.naechsteZiehung());
    }
    clog << systems.size() << " systems generated." << endl;
    return systems;
}

unsigned featureIndex(const featuresType& features,const string& name)
{
    auto pos = find(features.begin(),features.end(),name);
    if (pos == features.end())
    {
        cerr << "Unexpected featurename \"" << name << "\" during processing of configurations. Program aborted!" << endl;
        exit(1);
    }
    auto position = pos - features.begin();
   return position + 1;
}

setOfSystemsType processConfigfiles(const string& pathToConfigs,const featuresType& features,const setOfSystemsType& allSystems)
{
    setOfSystemsType challenge;
    unsigned configfiles = 0;
    for(auto& p: directory_iterator(pathToConfigs))
    {
       vector<bool> selection(features.size() + 1);
        selection.at(0) = true; // core set, i.e., index value at position 0, is always included
        ++configfiles;
        ifstream config(p.path());
        string featurename;
        while (config)
        {
            config >> featurename;
            if (config)
            {
                bool isSelected = featureIsSelected(featurename);
                featurename = purifyFeaturename(featurename);
                if (featurename != "")
                {
                    selection.at(featureIndex(features,featurename)) = isSelected;//(negatedSpecification == true ? false : true);
                }
            }
        }
        for (auto systemP = allSystems.begin(); systemP != allSystems.end(); ++systemP)
        {
            bool selected = true;
            for (unsigned featureNo = 0; featureNo <= features.size(); ++featureNo)
            {
                elementarySetType single_feature;
                single_feature.insert(featureNo);
                if (selection.at(featureNo) == true)
                {
                    if (systemP->find(single_feature) == systemP->end())
                    {
                        selected = false;
                        break;
                    }
                }
                else
                {
                    if (systemP->find(single_feature) != systemP->end())
                    {
                        selected = false;
                        break;
                    }
                }
            }
            if (selected == true)
            {
                challenge.insert(*systemP);
            }
        }
    }
    clog << "processed " << configfiles << " configurations with " << features.size() << " features." << endl;
    return challenge;
}

void computeElementarySubsets(const setOfSystemsType& challenge,const setOfElementarySetsType& elementarySubsets,const featuresType& features,const string& output)
{
    unsigned setNo = 0,
             successes = 0,
             fails = 0;
    ofstream result(output);
    if (!result)
    {
        cerr << "Could not open output file \"" << output << "\". Program aborted." << endl;
        exit(2);
    }
    for (auto subset = elementarySubsets.begin(); subset != elementarySubsets.end(); ++subset)
    {
        setOfSystemsType includes,
                         excludes;
        for (auto system = challenge.begin(); system != challenge.end(); ++system)
        {
            if (system->find(*subset) != system->end())
            {
                includes.insert(*system);
            }
            else
            {
                excludes.insert(*system);
            }
        }
        setOfElementarySetsType intersectionSet,
                                resultSet,
                                unionSet,
                                differenceSet;
        resultSet.clear();
        if (includes.size() > 0)
        {
            intersectionSet = *(includes.begin());
        }
        for (auto itIncludes = includes.begin(); itIncludes != includes.end(); ++itIncludes)
        {
            set_intersection(intersectionSet.begin(),intersectionSet.end(),itIncludes->begin(),itIncludes->end(),inserter(resultSet,resultSet.begin()));
            intersectionSet = resultSet;
            resultSet.clear();
        }
        resultSet.clear();
        if (excludes.size() > 0)
        {
            unionSet = *(excludes.begin());
        }
        for (auto itExcludes = excludes.begin(); itExcludes != excludes.end(); ++itExcludes)
        {
            set_union(unionSet.begin(),unionSet.end(),itExcludes->begin(),itExcludes->end(),inserter(resultSet,resultSet.begin()));
            unionSet = resultSet;
            resultSet.clear();
        }
        set_difference(intersectionSet.begin(),intersectionSet.end(),unionSet.begin(),unionSet.end(),inserter(differenceSet,differenceSet.begin()));
        result << (differenceSet.size() == 1 ? (++successes,"+++ ") : (++fails,"--- ")) << setNo++ << ' ';
        print(*subset,features,result);
        result << " = INTERSECTION " ;
        print(intersectionSet,features,result);
        result << " \\ UNION ";
        print(unionSet,features,result);
        result << " = ";
        print(differenceSet,features,result);
        result << endl << endl;
    }
    result << endl << "Summary: " << successes << " elementary subsets precisely computed, "
                                      << fails << " elementary subsets can not be precisely computed." << endl << endl;
  }

int main(int argc,char* argv[])
{
    clog << "Computing Feature Location Sets - Version 1.0 by Ulrich W. Eisenecker, Leipzig, June 2019" << endl;
    if (argc != 3)
    {
        cerr << "Execute the program with \n   " << argv[0] << " pathToConfigs partToResult\n Program aborted." << endl;
        exit(3);
    }
    string path = argv[1];
    string output = argv[2];
    clog << "Step 0: preprocessing config files ... " << flush;
    featuresType features = preprocessConfigfiles(path);
    clog << "Step 1: generate all elementary sets ... " << flush;
    setOfElementarySetsType allElementarySubsets = generateAllElementarySubsets(features.size());
    clog << "Step 2: generate all systems ... " << flush;
    setOfSystemsType allSystems = generateAllSystems(features.size(),allElementarySubsets);
    clog << "Step 3: processing config files ... " << flush;
    setOfSystemsType challenge = processConfigfiles(path,features,allSystems);
    clog << "Step 4: compute elementary subsets given " << challenge.size() << " config files ... " << flush;
    computeElementarySubsets(challenge,allElementarySubsets,features,output);
    clog << "FINISHED" << endl;
}
