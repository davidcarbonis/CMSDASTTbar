export CMS_DAS_TTbar_INSTALL=`pwd`

gcc_version="4.9.1"
source /afs/cern.ch/sw/lcg/external/gcc/$gcc_version/x86_64-slc6/setup.sh ""
unset gcc_version

root_version="5.34.24"
source /afs/cern.ch/sw/lcg/app/releases/ROOT/$root_version/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh
unset root_version
