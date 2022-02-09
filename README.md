# simulation
Basic example of calorimeter simulation in Geant4. 

Running on lxplus: 

export PATH=/cvmfs/sft.cern.ch/lcg/contrib/CMake/3.18.3/Linux-x86_64/bin/:${PATH}
source /cvmfs/sft.cern.ch/lcg/contrib/gcc/9.3.0/x86_64-centos7/setup.sh 
source /cvmfs/geant4.cern.ch/geant4/10.7/x86_64-centos7-gcc9-optdeb-MT/CMake-setup.sh 

export CXX=`which g++`
export CC=`which gcc`

mkdir build 
cd build 

cmake -DGeant4_DIR=/cvmfs/geant4.cern.ch/geant4/10.7/x86_64-centos7-gcc9-optdeb-MT/lib64/Geant4-10.7.0/ ..

make 
./ECAL ../run1.mac

'''
Submit job on condor.
'''
condor_submit condor.txt
