#!/bin/bash

set -e

if [[ "$1" == "" ]]; then
	echo "Expected argument: version number" >&2
	exit 1
fi
VERSION="$1"

echo -ne "\nLinux clean build\n\n"
pushd build
	rm -fv PathFollower.ext.2.tf2/{*.o,*.so}
	ambuild
popd

echo -ne "\nWindows rebuild\n\n"
ssh jgvm-win8 $(cat <<EOF
	net use /persistent:no G: \\\\jgpc\\pool\\Game;
	cd G:/SourceMod/PathFollower/msvc;
	cmd /c "C:\\VS2015\\VsMSBuildCmd.bat & msbuild /m:8 /t:Rebuild";
EOF
)

echo -ne "\nGenerate SYM file from PDB\n\n"
ssh jgvm-win8 $(cat <<EOF
	net use /persistent:no G: \\\\jgpc\\pool\\Game;
	cd G:/SourceMod/PathFollower/msvc/Release*;
	../breakpad/dump_syms.exe PathFollower.ext.2.tf2.pdb >PathFollower.ext.2.tf2.sym;
EOF
)
sudo -n chown srcds:users -R msvc
dos2unix -r -v msvc/Release*/PathFollower.ext.2.tf2.sym

echo -ne "\nCreate package archive\n\n"
rm -frv "/tmp/PathFollower-$VERSION"
mkdir "/tmp/PathFollower-$VERSION"
cp -av -t "/tmp/PathFollower-$VERSION" build/package/*
cp -av -t "/tmp/PathFollower-$VERSION/addons/sourcemod/extensions" msvc/Release*/PathFollower.ext.2.tf2.{dll,pdb,sym}
sudo -n chown srcds:users -R "/tmp/PathFollower-$VERSION" || true
pushd /tmp
	tar -cv "PathFollower-$VERSION" | xz -9 -T1 >"PathFollower-$VERSION.tar.xz"
popd
rm -frv "/tmp/PathFollower-$VERSION"

exit 0
