{ lib, stdenv, fetchurl, intltool, pkg-config, gtk3, SDL2, xorg, wrapGAppsHook
, libcdio, nasm, ffmpeg, file, fetchpatch, cmake, fetchFromGitHub }:

stdenv.mkDerivation {
  pname = "pcsxr-gdb";
  version = "1.9.94-gdb";

  src = fetchFromGitHub {
    owner = "XaviDCR92";
    repo = "pcsxr";
    rev = "ca3b98cc53a15a627c48e281a37e9665f36c879d";
    fetchSubmodules = true;
    sha256 = "sha256-3vPpZNMCpOASv0TMNjdnbhK1eb4tMfxmWiV21m7hPDk=";
  };

  nativeBuildInputs = [ intltool pkg-config wrapGAppsHook cmake ];
  buildInputs = [ gtk3 SDL2 xorg.libXv xorg.libXtst libcdio nasm ffmpeg file ];

  meta = with lib; {
    description = "Playstation 1 emulator";
    license = licenses.gpl2Plus;
    platforms = platforms.all;
  };
}
