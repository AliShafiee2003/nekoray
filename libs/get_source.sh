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

# Clone sing-box from MatsuriDayo fork (has custom boxapi package)
if [ ! -d "sing-box" ]; then
  git clone -b 1.9.7 https://github.com/MatsuriDayo/sing-box.git
fi

# Clone sing-quic
if [ ! -d "sing-quic" ]; then
  git clone https://github.com/MatsuriDayo/sing-quic.git
fi
pushd sing-quic
git checkout "e396733db4de15266f0cfdb43c392aca0759324a" 2>/dev/null || true
popd

# Clone libneko
if [ ! -d "libneko" ]; then
  git clone https://github.com/MatsuriDayo/libneko.git
fi
pushd libneko
git checkout "5277a5bfc889ee7a89462695b0e678c1bd4909b1" 2>/dev/null || true
popd

####
if [ ! -d "Xray-core" ]; then
  git clone --no-checkout https://github.com/MatsuriDayo/Xray-core.git
fi
pushd Xray-core
git checkout "$COMMIT_MATSURI_XRAY"
popd

popd
