{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (system: let
      pkgs = nixpkgs.legacyPackages.${system};
      name = "tme1";
      buildPkgs = with pkgs; [
        gcc
        gnumake
        pkg-config
      ];
      libPkgs = with pkgs; [
        # Add any system libraries your project depends on
      ];
      devPkgs = with pkgs; [
        clang-tools
        gdb
        valgrind
        automake
        autoconf
        just
        watchexec
        bear
      ];
    in {
      packages.default = pkgs.stdenv.mkDerivation {
        pname = name;
        version = "1.0.0";
        src = ./.;
        nativeBuildInputs = buildPkgs;
        buildInputs = libPkgs;
        buildPhase = ''
          make
        '';
        installPhase = ''
          mkdir -p $out/bin
          cp ${name} $out/bin/
        '';
      };
      devShell = pkgs.mkShell {
        shellHook = ''
        '';
        nativeBuildInputs = buildPkgs;
        buildInputs = libPkgs ++ devPkgs;
      };
    });
}
