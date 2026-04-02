#include <STC89C5xRC.H>

//数码管位置
void DisplaySelect(unsigned char Location)
{
    switch(Location)
    {
        case 1: P24=1; P23=1; P22=1; break;
        case 2: P24=1; P23=1; P22=0; break;
        case 3: P24=1; P23=0; P22=1; break;
        case 4: P24=1; P23=0; P22=0; break;
        case 5: P24=0; P23=1; P22=1; break;
        case 6: P24=0; P23=1; P22=0; break;
        case 7: P24=0; P23=0; P22=1; break;
        case 8: P24=0; P23=0; P22=0; break;
    }
}