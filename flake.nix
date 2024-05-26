{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs?ref=nixos-24.05";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        packages = rec {
          default = fullscreentools;

          fullscreentools = pkgs.stdenv.mkDerivation rec {
            pname = "fullscreentools";
            version = "0.0.0";

            src = ./.;

            nativeBuildInputs = with pkgs; [
              cmake
              pkg-config
            ];

            buildInputs = with pkgs; [
              SDL
              fmt
              gtkmm2
              motif
              xorg.libX11
            ];

            postFixup = ''
               sed -i "s#libsdl-hack.so#$out/lib/libsdl-hack.so#" $out/bin/sdl-hack.sh
            '';
          };
        };
      }
    );
}
