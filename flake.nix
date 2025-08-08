{
  inputs = { nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable"; };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
    in {
      packages.${system} = rec {
        pshash-gui = pkgs.stdenv.mkDerivation {
          pname = "pshash-gui";
          version = "1.0";
          src = ../gui;
          buildInputs = with pkgs; [
              gcc
              wxGTK32
          ];
          buildPhase = ''
              g++ main.cpp -O5 -o pshash-gui $(wx-config --cxxflags --libs)
          '';
          installPhase = ''
              mkdir -p $out/bin
              mv pshash-gui $out/bin/
          '';
        };
        default = pshash-gui;
      };
      apps.${system}.default = {
        type = "app";
        program = "${self.packages.${system}.default}/bin/pshash-gui";
      };
      devShells.${system} = rec {
        pshash-gui = pkgs.mkShell {
          shellHook = ''
            export ZDOTDIR="$XDG_CONFIG_HOME/nix-develop"
          '';
          nativeBuildInputs = with pkgs; [
            bear
            coccinelle
            valgrind
            gcc
            wxGTK32
          ];
        };
        default = pshash-gui;
      };
    };
}
