# C compiler
let
  sources = import ./nix/sources.nix;
  niv = import sources.niv { inherit sources; };
  pkgs = import sources.nixpkgs { };
  flawfinder = (pkgs.callPackage ./nix/pkgs/flawfinder { });
  lizard = (pkgs.callPackage ./nix/pkgs/lizard { });
  pcsxr-gdb = (pkgs.callPackage ./nix/pkgs/pcsxr-gdb { });
  mkpsxiso = (pkgs.callPackage ./nix/pkgs/mkpsxiso { });
in pkgs.mkShell {
  buildInputs = [
    # Nix-specific
    niv.niv
    # Devtools
    pkgs.gnumake
    pkgs.bear
    pkgs.clang-tools
    pkgs.cppcheck
    pkgs.include-what-you-use
    flawfinder
    lizard
    # Dependencies of PSn00bSDK tooling
    pkgs.gcc
    pkgs.tinyxml-2
    pkgs.pkg-config
    mkpsxiso
    # Emulator for running samples
    pcsxr-gdb
  ];
}
