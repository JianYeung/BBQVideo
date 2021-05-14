#!/bin/bash

# assumption:
#  - pwd must be inside the repo's homed directory (android-ndk)
#  - upon completion, we are still in the same directory ( no change )

TMP_SETUP_FILENAME=versions_.txt

# parse all build.gradle to find the specified tokens' version
# usage:
#    retrive_versions  token version_file
# where
#    token: the token to search for inside build.grade
#           version string is right after the token string
#    version_file: file to hold the given versions
#                  one version at a line
retrieve_versions() {
#   $1: token; $2 version_file
    if [[ -z $1 ]] || [[ -z $2 ]]; then
        echo "input string(s) may be empty: token: $1; version_file: $2"
        return 1
    fi

    find . -type f -name 'build.gradle' -exec grep $1 {} +  | \
    sed "s/^.*$1//" | sed 's/[=+]//g' |   \
    sed 's/"//g' | sed "s/'//g" |     \
    sed 's/[[:space:]]//g' | \
    awk '!seen[$0]++' > $2
    
    return 0
}

## Retrieve all necessary Android Platforms and install them all
retrieve_versions compileSdkVersion $TMP_SETUP_FILENAME

# fixups
sed -i '/COMPILE_SDK_VERSION/d' $TMP_SETUP_FILENAME
# Install platforms
while read -r version_; do
	echo y | $ANDROID_HOME/tools/bin/sdkmanager "platforms;android-$version_";
done < $TMP_SETUP_FILENAME
#echo "Android platforms:"; cat $TMP_SETUP_FILENAME;rm -f $TMP_SETUP_FILENAME

rm -f $TMP_SETUP_FILENAME
