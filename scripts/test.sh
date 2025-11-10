#!/bin/bash

RSA=./build/bin/rsa
PUB=./build/bin/rsa.pub
KEY=./build/bin/rsa.key
ENC=./build/bin/enc.txt
DEC=./build/bin/dec.txt


gen() {
    time $RSA gen --bits $1 --pubkey $PUB --key $KEY
}

enc() {
    if [[ $1 == '-' ]]; then
        time (echo "Hello World" | $RSA enc --pubkey $PUB --out $ENC)
    else
        time $RSA enc --pubkey $PUB --out $ENC --in $KEY
    fi
}

dec() {
    if [[ $1 == '-' ]]; then
        time $RSA dec --key $KEY --in $ENC
    else
        time $RSA dec --key $KEY --in $ENC --out $DEC
    fi
}

dif() {
    diff $KEY $DEC
}

if [[ "${BASH_SOURCE[0]}" == "$0" ]]; then
  case "${1:-}" in
    gen) shift; gen "$@" ;;
    enc) shift; enc "$@" ;;
    dec) shift; dec "$@" ;;
    dif) shift; dif "$@" ;;
    *)
      ;;
  esac
fi