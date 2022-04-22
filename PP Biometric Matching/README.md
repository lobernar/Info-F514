

## TFHE Library
Installation:
https://tfhe.github.io/tfhe/installation.html
Usage:
https://tfhe.github.io/tfhe/usage.html

## Don't forget
Add those to your .bashrc / .zshrc, or execute this in your terminal before launching:
```
TFHE_PREFIX=/usr/local #the prefix where you installed tfhe
export C_INCLUDE_PATH=$C_INCLUDE_PATH:$TFHE_PREFIX/include
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$TFHE_PREFIX/include
export LIBRARY_PATH=$LIBRARY_PATH:$TFHE_PREFIX/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TFHE_PREFIX/lib
```