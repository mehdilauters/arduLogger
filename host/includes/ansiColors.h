/*  Ce fichier sert a utiliser les couleurs dans un programme en mode console en utilisant les sequences d'echappement ansi.


*/
#include <iostream>
#include <map>
#ifndef ANSICOLORS_H_
#define ANSICOLORS_H_

class ansiColors
	{
 private:
         std::map<std::string,std::string> colorsFont;
         std::map<std::string,std::string> colorsBackround;
         std::map<std::string,std::string> policeStyle;
 public:
 ansiColors();
 std::string colorStart(std::string _fontColor, std::string _backgroundColor = "black", std::string _policeStyle = "normal");
 std::string clear();
 std::string colorStop();
 
 };

#endif