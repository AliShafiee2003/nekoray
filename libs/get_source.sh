#!/bin/bash
set -e

source libs/env_deploy.sh
ENV_NEKORAY=1
source libs/get_source_env.sh
pushd ..

####
if [ ! -d "sing-box-extra" ]; then
  git clone --no-checkout https://github.com/MatsuriDayo/sing-box-extra.git
fi
pushd sing-box-extra
git checkout "$COMMIT_SING_BOX_EXTRA"
popd

# Clone sing-box from upstream SagerNet (MatsuriDayo fork is archived/broken)
ENV_SING_BOX_EXTRA=1
source $SRC_ROOT/libs/get_source_env.sh

if [ ! -d "sing-box" ]; then
  git clone --no-checkout https://github.com/SagerNet/sing-box.git
fi
pushd sing-box
git fetch --all
git checkout "$COMMIT_SING_BOX"
popd

# Clone sing-quic
if [ ! -d "sing-quic" ]; then
  git clone --no-checkout https://github.com/SagerNet/sing-quic.git
fi
pushd sing-quic
git fetch --all
git checkout "$COMMIT_SING_QUIC"
popd

# Clone libneko
if [ ! -d "libneko" ]; then
  git clone --no-checkout https://github.com/MatsuriDayo/libneko.git
fi
pushd libneko
git fetch --all
git checkout "$COMMIT_LIBNEKO"
popd

####
if [ ! -d "Xray-core" ]; then
  git clone --no-checkout https://github.com/MatsuriDayo/Xray-core.git
fi
pushd Xray-core
git checkout "$COMMIT_MATSURI_XRAY"
popd

popd
