{ lib, stdenv, fetchFromGitHub, gnumake, pkg-config, clang_11, SDL2, libuv
, ffmpeg-full, glfw, zlib, freetype }:

stdenv.mkDerivation {
  pname = "pcsx-redux";
  version = "41f7de6080eca7dc3e9d93a6083700471dbad194";

  src = fetchFromGitHub {
    owner = "grumpycoders";
    repo = "pcsx-redux";
    rev = "41f7de6080eca7dc3e9d93a6083700471dbad194";
    fetchSubmodules = true;
    sha256 = "sha256-Qqz6HoNBaFusXDbm5WuE9QYri0fBzHl5bxyf7fDQAbY=";
  };

  nativeBuildInputs = [ gnumake pkg-config clang_11 ];
  buildInputs = [ SDL2 libuv ffmpeg-full glfw zlib freetype ];

  # Triggers some clang errors which have to be silenced
  patches = [ ./0001-Fix-Makefile.patch ];

  # Has to be clang due to using coroutines
  makeFlags = [ "PREFIX=$(out)" "CC=clang" "CXX=clang++ -j4" ];

  meta = with lib; {
    description =
      "Latest iteration on the PCSX emulator, heavily focused on development, debuggability, and reverse engineering.";
    license = licenses.gpl2;
    platforms = platforms.all;
  };
}
