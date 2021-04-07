{ lib, stdenv, cmake, pkg-config, tinyxml-2, fetchFromGitHub }:

stdenv.mkDerivation {
  pname = "mkpsxiso";
  version = "git";

  src = fetchFromGitHub {
    owner = "Lameguy64";
    repo = "mkpsxiso";
    rev = "c44b78e37bbc115591717ac4dd534af6db499ea4";
    sha256 = "sha256-JJG2Ed7SY7chcInjTphBCF74Arrpn/fmbL1IpS+xDik=";
  };

  nativeBuildInputs = [ pkg-config cmake ];
  buildInputs = [ tinyxml-2 ];

  installPhase = ''
    mkdir -p $out/bin
    cp bin_nix/mkpsxiso $out/bin
  '';

  meta = with lib; {
    description =
      "A modern clone of BUILDCD used for building CD images of PlayStation games in the official development tools";
    license = licenses.gpl2;
    platforms = platforms.all;
  };
}
