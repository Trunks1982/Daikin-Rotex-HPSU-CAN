#!/bin/bash
echo "path of bash script: `pwd`"
cp .github/workflows/resources/external_components_dev.yaml .
sed -i 's/!/?/g' full.yaml # make yaml pyyaml conform for cels
cels patch full.yaml external_components_dev.yaml > full_patched.yaml
cat full_patched.yaml > full.yaml
sed -i 's/?/!/g' full.yaml # make yaml esphome conform
