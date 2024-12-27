{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  nativeBuildInputs = with pkgs.buildPackages; [
    cmake
    pkg-config
    libsForQt5.full
  ];

  buildInputs = with pkgs.buildPackages; [
    pugixml
  ];
}
