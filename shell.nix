let
  sources = import ./nix/sources.nix;
  pkgs = import sources.nixpkgs { };
  inherit (pkgs.lib) optional optionals;

  mach-nix = import (builtins.fetchGit {
    url = "https://github.com/DavHau/mach-nix/";
    ref = "refs/tags/3.1.1";
  }) {};
  customPython = mach-nix.mkPython {
  	requirements = ''
  		conan==1.31.3
   	'';
  };
in
pkgs.mkShell {
  buildInputs = [
    pkgs.bashInteractive
	pkgs.cmake
	
	customPython
	
	###pkgs.gcc9
  ];
}
