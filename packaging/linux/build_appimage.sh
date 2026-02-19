#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build-appimage"
APPDIR="${ROOT_DIR}/AppDir"

cmake -S "${ROOT_DIR}" -B "${BUILD_DIR}" -DCMAKE_BUILD_TYPE=Release
cmake --build "${BUILD_DIR}" -j"$(nproc)"

rm -rf "${APPDIR}"
mkdir -p "${APPDIR}/usr/bin" "${APPDIR}/usr/share/applications"
cp "${BUILD_DIR}/NotaScore" "${APPDIR}/usr/bin/NotaScore"
cp "${ROOT_DIR}/packaging/linux/notascore.desktop" "${APPDIR}/usr/share/applications/notascore.desktop"
cp "${ROOT_DIR}/packaging/linux/AppRun" "${APPDIR}/AppRun"

if command -v appimagetool >/dev/null 2>&1; then
  appimagetool "${APPDIR}" "${ROOT_DIR}/NotaScore-x86_64.AppImage"
  echo "AppImage gerada em: ${ROOT_DIR}/NotaScore-x86_64.AppImage"
else
  echo "appimagetool não encontrado. AppDir pronto em ${APPDIR}"
fi
