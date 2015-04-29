#!/bin/bash
#---------------------------------------------------------------------------------------------------
# Install qjets plugin to be able to fully compile MitPhysics.
#
# This situation should be temporary but for now it is required to work with the contributions to
# fastjet and the most up to date versions.
#
#                                                              May 29, 2014 - V0 Leonardo Di Matteo
#                                                              Apr 26, 2015 - V1 Yutaro Iiyama
#---------------------------------------------------------------------------------------------------
function configureScram {
  local EXTERNAL=$1

  echo ""
  echo " INFO - configuring SCRAM to use Qjets at: $EXTERNAL/Qjets"
  echo ""    

  # add local fastjet external to scarm config
  mv $CMSSW_BASE/config/toolbox/$SCRAM_ARCH/tools/selected/qjets.xml \
     $CMSSW_BASE/config/toolbox/$SCRAM_ARCH/tools/selected/qjets.xml-last.$(date +%s)

  echo \
'  <tool name="qjets" version="2">
    <info url="http://jets.physics.harvard.edu/Qjets/html/Welcome.html"/>
    <lib name="qjets"/>
    <client>
      <environment name="QJETS_BASE" default="'$EXTERNAL'/Qjets"/>
      <environment name="LIBDIR" default="$QJETS_BASE/lib"/>
      <environment name="INCLUDE" default="$QJETS_BASE"/>
    </client>
  </tool>' > $CMSSW_BASE/config/toolbox/$SCRAM_ARCH/tools/selected/qjets.xml

  # commit the scram config changes
  cd $CMSSW_BASE/src
  scram setup qjets
}

# test whether environment is clean and we can start
if [ -z "$CMSSW_BASE" ]
then
  echo ""
  echo " ERROR - cmssw is not setup. Please, setup release directory and checkout MitPhysics."
  echo ""
  exit 1
fi
if [ -d "$CMSSW_BASE/src/MitPhysics" ]
then
  echo ""
  echo " INFO - found MitPhysics location at: $CMSSW_BASE/src/MitPhysics"
  echo ""
else
  echo ""
  echo " ERROR - MitPhysics is not in your release. Please check it from GITHUB/CVS."
  echo ""
  exit 1
fi

# the default location
EXTERNAL=/cvmfs/cvmfs.cmsaf.mit.edu/hidsk0001/cmsprod/cms/external
if [ -d $EXTERNAL/Qjets ]
then
  configureScram $EXTERNAL
  exit 0
fi

EXTERNAL=/home/cmsprod/cms/external
if [ -d $EXTERNAL/Qjets ]
then
  configureScram $EXTERNAL
  exit 0
fi

EXTERNAL="/home/$USER/cms/external"
echo " INFO - make own repository at: $EXTERNAL/Qjets"
echo ""

# Here the real work starts (qjets plugin and tweaks)

mkdir -p $EXTERNAL

# install best qjets plugin version
#-----------------------------

# set all relevant variables
QJETS_URL="http://t3serv001.mit.edu/~cmsprod"
QJETS_TGZ="Qjets.tar.gz"
QJETS_DIR=`echo $EXTERNAL/$QJETS_TGZ | sed 's/.tar.gz//'`

# in the right location
cd $EXTERNAL

# now do the download
echo " INFO - download starting"
echo ""
wget "$QJETS_URL/$QJETS_TGZ" -O $QJETS_TGZ

# unpack it (no debug, could add 't' option)
echo " INFO - unpacking"
echo ""
tar fzx $QJETS_TGZ

# cleanup to avoid junk
echo " INFO - cleaning up"
echo ""
rm -rf $QJETS_TGZ

# installing qjets plugin
echo " INFO - installing"
echo ""
cd `echo $QJETS_TGZ | sed 's/.tar.gz//'`
make

# make shared libraries for the plugin
g++ -shared -fPIC -o $QJETS_DIR/lib/libqjets.so -Wl,-soname,libqjets.so $QJETS_DIR/[A-Z]*.o 

# final adjustment to scram configuration
configureScram $EXTERNAL

exit 0
