{
  description = "Raspberry Pi Pico C/C++ SDK dev shell";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        pico-sdk = pkgs.pico-sdk.override { withSubmodules = true; };
      in
      {
        devShells.default = pkgs.mkShell {
          name = "pico-dev";

          nativeBuildInputs = with pkgs; [
            clang-tools
            cmake
            gcc-arm-embedded
            git
            ninja
            picotool
            python3
          ];

          env = {
            CMAKE_EXPORT_COMPILE_COMMANDS = "ON";
            CMAKE_GENERATOR = "Ninja";
            PICO_SDK_PATH = "${pico-sdk}/lib/pico-sdk";
          };
        };
      }
    );
}
