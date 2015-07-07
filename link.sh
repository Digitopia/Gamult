#!/bin/sh

#
# Author: 
#     Nuno Hespanhol
#
# Last Modified: 
#     7 Jul 2015
#
# Description:
#     Run this from the project root!
#     TODO: Better description goes here.
#

# PLATFORMS=("Gamult_ios" "Gamult_osx") 
PLATFORMS=("Gamult_ios") 

for platform in "${PLATFORMS[@]}"
do

    echo "$platform"
    cd $platform

    echo "\tsymlink src"  # symlink source files (non recursively) 
    cd src                # cd into platform src dir
    ln -sf ../../src/* .  # for the paths to be relative need to go into the dir and symlink from there

    echo "\tsymlink data" # symlink data folder and files (recursively)
    cd ../                # go up a dir
    mkdir -p bin/data     # create folder if doesn't exist
    
    cd ../data            # go to general data folder

    # get recursively all filepaths contained in data/, including dirs
    paths=`find . -name \* -print | cut -c3-`

    for path in $paths
    do
       
        # skip dirs
        if [[ -d $path ]]; then
            continue
        fi

        # skip DS_Store files
        if [[ $path == *"DS_Store"* ]]; then
            continue
        fi

        filename=`basename $path`
        folder=`echo $path | cut -d "/" -f 1 | rev | rev` # FIXME: not working for nested dirs
        # echo $path $folder $filename

        # mkdir -p ../$platform/bin/data     # go into dest symlink location
        cd ../$platform/bin/data     # go into dest symlink location
        echo $PWD
        echo
        # mkdir -p $folder             # create folder if not exists
        # cd $folder                   # go into dest
        # ln -sf ../../../$path .      # FIXME: how many folders back should I go?
        
    done
    
done
