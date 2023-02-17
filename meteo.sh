#!/bin/bash

wdir="$(cd "$(dirname "$0")" && pwd)"
#echo 'working dir : ' $wdir

# Functions
. $wdir/lib/functions.sh

# Constants
. $wdir/include/meteo.config.sh

# Variables
extractOptions=()
types=()
dates=()
locations=()
commandStrings=()
sortingMethod='--avl'
date1=-1
date2=-1
sortingOptionsNumber=0
dataFile=''
stationsString=''
datesString=$QUOTEDQUOTES  
dataMinDate=''
dataMaxDate=''

#### Main program
# create mandatory directories
createDirectories
# remove all files from tmp and output directories
emptyDirectories

options=$(getopt -o :t:p:whmd:FGSAOQf: -l avl,abr,tab,help -- "$@")
returnCode=$?

if [ $returnCode -ne 0 ]
then
    sendReturnCode 1
fi

if [ "$#" -eq 0 ]
then
    sendReturnCode $OPTIONERROR "no parameters given"
fi

params=("$@")

if ! [ -d $TMPDIR ]
then
    mkdir $TMPDIR
    retval=$?
    if [ $retval -gt 0 ]
    then
        sendReturnCode $OTHERERROR 'Unable to create tmp directory'
    fi
fi    
checkHelp "${params[@]}"
checkOptionsAndArguments "${params[@]}"

if [ "${#locations[@]}" -gt 0 ]
then
    filterWithLocation
fi

if [ "${#dates[@]}" -gt 0 ]
then
    filterWithDate
fi
extractData
compileCExec
launchProgram
generateGraphics
rm $CEXEC

exit 0