find ./ -type f -readable -writable -exec sed -i "s/Graviocoin/Graviocoin/g" {} \;
#find ./ -type f -readable -writable -exec sed -i "s/PART/GIO/g" {} \;
find ./ -type f -readable -writable -exec sed -i "s/GIO_/GIO_/g" {} \;
find ./ -type f -readable -writable -exec sed -i "s/GIO/GIO/g" {} \;
find ./ -type f -readable -writable -exec sed -i "s/graviocoin/graviocoin/g" {} \;
find ./ -type f -readable -writable -exec sed -i "s/graviocoind/graviocoind/g" {} \;

find ./ -execdir rename 's/graviocoin/graviocoin/' '{}' \+


