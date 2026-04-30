#!/bin/bash

# stop the script if any command fails
set -e

# go to the folder where the script is located
cd "$(dirname "$0")"

# create a data directory in Installer/packages/com.vendor/product
mkdir packages/com.vendor.product/data

# copy the EPIC_UI executable file from build folder to com.vendor.product/data
cp ../build/EPIC_UI packages/com.vendor.product/data

# build the EPIC installer
bin/binarycreator --offline-only -c config/config.xml -p packages EPIC_Installer

echo "Installer Created Successfully: EPIC_Installer"