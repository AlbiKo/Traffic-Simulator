#!/bin/sh
echo "I seguenti pacchetti saranno installati:"
echo " - Codeblocks"
echo " - G++ 4.9"
echo " - GTKmm 3.0"
echo " - Glade"
echo " - SFML"
echo " - Git"
echo " "
read -p "Vuoi continuare? (Y/N): " yn
	case $yn in
		[Yy]* ) break;;
		* ) exit;;
	esac
add-apt-repository ppa:ubuntu-toolchain-r/test -y
apt-get update
apt-get install g++ -y
apt-get install codeblocks g++-4.9 libgtkmm-3.0-dev glade libsfml-dev git -y
echo " "
echo "Eseguito."
