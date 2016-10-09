#!/bin/sh
echo "I seguenti pacchetti saranno installati:"
echo " - GTKmm 3.0"
echo " - SFML"
echo " "
read -p "Vuoi continuare? (Y/N): " yn
	case $yn in
		[Yy]* ) break;;
		* ) exit;;
	esac
apt-get update
apt-get install libgtkmm-3.0-dev libsfml-dev -y
echo " "
echo "Eseguito."
