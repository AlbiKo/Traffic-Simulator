#!/bin/sh
echo "I seguenti pacchetti saranno installati:"
echo " - Codeblocks"
echo " - G++ 4.9"
echo " - GTKmm 3.0"
echo " - Glade"
echo " "
echo "I seguenti pacchetti saranno scaricati:"
echo " - SFML 2.3.2"
echo " "
read -p "Vuoi continuare? (Y/N): " yn
	case $yn in
		[Yy]* ) break;;
		* ) exit;;
	esac
add-apt-repository ppa:ubuntu-toolchain-r/test -y
apt-get update
apt-get install codeblocks g++-4.9 libgtkmm-3.0-dev glade -y
wget -O SFML-2.3.2.tar.gz http://www.sfml-dev.org/files/SFML-2.3.2-linux-gcc-64-bit.tar.gz 
tar -zxvf SFML-2.3.2.tar.gz
rm SFML-2.3.2.tar.gz
echo " "
echo "Eseguito."
