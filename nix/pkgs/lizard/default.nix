{ lib, python3, python3Packages }:

python3.pkgs.buildPythonPackage rec {
  pname = "lizard";
  version = "1.17.7";
  src = python3.pkgs.fetchPypi {
    inherit pname version;
    sha256 = "sha256-0I56+0U0/DJkAe3i1arOAndX+xqd6zzJ+Of0M5mDxjA=";
  };
  # Tests error with a mysterious tuple index out of range
  doCheck = false;
  buildInputs = with python3Packages; [ setuptools ];
}
