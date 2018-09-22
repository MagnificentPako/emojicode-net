with import <nixpkgs> {};

stdenv.mkDerivation {
    name = "emojicode-net";
    src  = ./.;
    buildInputs = [ cmake 
                    (import "/etc/nixos/pkgs/emojicode") 
                    gcc 
                    ninja
                    openssl
                    (import "/home/paul/doc/dev/Cpp/uWebSockets")
                    zlib
                  ];
    doCheck = false;
}