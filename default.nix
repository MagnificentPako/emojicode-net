with import <nixpkgs> {};

stdenv.mkDerivation {
    name = "emojicode-net";
    src  = ./.;
    buildInputs = [ cmake 
                    (import "/etc/nixos/pkgs/emojicode") 
                    gcc 
                    ninja
                    openssl
                  ];
    doCheck = false;
}