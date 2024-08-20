#!/bin/bash
#
# Copyright (c) ETSI 2018
# Realeased under the ETSI Software License
#
# https://forge.etsi.org/etsi-software-license

YANG_MODULES="yang/yang/*"
OUTPUT="etsi-nfv-descriptors.html"
IMG_NAME="sol006"
VERSION=latest
IMG="$IMG_NAME:$VERSION"

CMT=$(git rev-parse --short HEAD)

docker build --tag "$IMG" .
docker run "$IMG"

echo "Building HTML tree"
docker run -v"$(pwd)/src/yang/:/yang/" mjethanandani/pyang \
	"/bin/sh" -c "cd /yang/ && pyang -f jstree -p /yang/ \
	/yang/etsi-nfv-descriptors.yang > etsi-nfv.html"

sed -r -i 's|<a href=\"http://www.tail-f.com">|<a href="http://etsi.org">|g' src/yang/etsi-nfv.html
sed -r -i 's|data\:image/gif\;base64,R0lGODlhS.*RCAA7|https://www.etsi.org/images/ETSILogoTwitter.png\" height=\"100|g' src/yang/etsi-nfv.html

sed -r -i "s|<div class=\"app\">|<p>Commit: <a href=\"https://forge.etsi.org/gitlab/nfv/SOL006/commit/${CMT}\">$CMT</a>, Build:<a href=\"$BUILD_URL\">$BUILD_NUMBER</a></p><div class=\"app\">|g" src/yang/etsi-nfv.html


exit $?

