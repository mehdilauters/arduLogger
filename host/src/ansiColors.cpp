#include "ansiColors.h"

using namespace std;

ansiColors::ansiColors()
{
 colorsFont["normal"] = "0m";
 colorsFont["black"] = "30m"; 
 colorsFont["red"] = "31m"; 
 colorsFont["green"] = "32m"; 
 colorsFont["yellow"] = "33m"; 
 colorsFont["blue"] = "34m"; 
 colorsFont["magenta"] = "35m"; 
 colorsFont["cyan"] = "36m"; 
 colorsFont["white"] = "37m"; 
 
 colorsBackround["normal"] = "";
 colorsBackround["black"] = "30"; 
 colorsBackround["red"] = "41"; 
 colorsBackround["green"] = "42"; 
 colorsBackround["yellow"] = "43"; 
 colorsBackround["blue"] = "44"; 
 colorsBackround["magenta"] = "45"; 
 colorsBackround["cyan"] = "46"; 
 colorsBackround["white"] = "47"; 
 
 policeStyle["normal"]="0";
 policeStyle["bold"]="1";
 policeStyle["notBold"]="21";
 policeStyle["underlined"]="4";
 policeStyle["notUnderlined"]="24";
 policeStyle["blinking"]="5";
 policeStyle["notBlinking"]="25";
 policeStyle["inversed"]="7";
 policeStyle["notInversed"]="27";
 
}



std::string ansiColors::colorStart(std::string _fontColor, std::string _backgroundColor, std::string _policeStyle)
{
    //printf("%s%s%s",policestyle,colorfond,colortext);
    return "\033[" + policeStyle[_policeStyle] + ";" + colorsBackround[_backgroundColor] + ";" + colorsFont[_fontColor];
}

 std::string ansiColors::clear()
 {
  return   "\033[2J";
 }
 
 
 std::string ansiColors::colorStop()
 {
  return "\033[0m";
 }