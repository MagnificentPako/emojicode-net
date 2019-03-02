# emojicode-net

An Emojicode lib adding support for HTTPS and WSS. Currently this also includes
a regex function and wrappers around [json](https://github.com/nlohmann/json) 
which still have to be separated.

Note that this uses my emojicode [nix-expr](https://gist.github.com/MagnificentPako/ff66a6c2f0703ad842a58f703eb09665) 
to build correctly and you have to change the path for it to work (and of course
download the .nix file).

# Licenses

+ easywsclient: MIT
+ json: MIT
+ openssl: Apache 2
+ mbedtls: Apache 2