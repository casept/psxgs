{ lib, python3, python3Packages, fetchurl, gnused }:

python3.pkgs.buildPythonPackage rec {
  name = "flawfinder";
  version = "2.0.15";
  src = fetchurl {
    url =
      "https://github.com/david-a-wheeler/flawfinder/archive/${version}.tar.gz";
    sha256 = "sha256-KuBsKJdAo/tbxBId/p8QSDFhHqmfVNyR8XuEgCaHuCQ=";
  };
  buildInputs = with python3Packages; [ setuptools ];
  nativeBuildInputs = [ gnused ];
  # Manpage can't be built due to missing data in pypy dist
  patchPhase = "${gnused}/bin/sed -i '/flawfinder\\.1\\.gz/d' setup.py ";
}
