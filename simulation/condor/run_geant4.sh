#!/bin/bash                                                                     

JOBID=$1

echo 'Test Run...'

export PATH=/cvmfs/sft.cern.ch/lcg/contrib/CMake/3.18.3/Linux-x86_64/bin/:${PATH} 
source /cvmfs/sft.cern.ch/lcg/contrib/gcc/9.3.0/x86_64-centos7/setup.sh
source /cvmfs/geant4.cern.ch/geant4/10.7/x86_64-centos7-gcc9-optdeb-MT/CMake-setup.sh

filename="run1_job${JOBID}.mac"
cp run1.mac ${filename}

search_string="/analysis/ntuple/setFileName 0 ntuple"
replace_string="/analysis/ntuple/setFileName 0 ntuple_${JOBID}.root"

sed -i --expression "s@${search_string}@${replace_string}@" ${filename}

#sed -i "s/${search_string}/${replace_string}/" ${filename}
./ECAL ${filename}
rm -f ${filename}
