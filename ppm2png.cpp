/*
Joshua Kenneth Pinto 08-07-2022

Program to convert PPM file to PNG
http://paulbourke.net/dataformats/ppm/
*/

#include <iostream>
#include <lodepng.h>
#include <fstream>
#include <vector>

int main(){

    const char* fname = "image.ppm";
    const char* foutname = "image.png";

    std::ifstream f(fname, std::ios::in | std::ios::binary | std::ios::ate);

    // read the contents on the PPM file into memory
    char* imagebuffer = nullptr;
    std::streampos size;

    int WIDTH, HEIGHT;

    if(f.is_open()){
        size = f.tellg();
        imagebuffer = new char[size];
        f.seekg(0, std::ios::beg);
        f.read(imagebuffer, size);
        f.close();
    }else{
        std :: cout << "Error reading file!\n";
        return -1;
    }

    uint8_t WID_BYT_LEN, HEI_BYT_LEN, DAT_BYT_STRT;

    for(WID_BYT_LEN = 0;; WID_BYT_LEN++)
        if (imagebuffer[3 + WID_BYT_LEN] == ' ') break;

    for(HEI_BYT_LEN = 0;; HEI_BYT_LEN++)
        if (imagebuffer[3 + WID_BYT_LEN + 1 + HEI_BYT_LEN] == '\n') break;

    char* str_rep_wid = new char[WID_BYT_LEN];
    char* str_rep_hei = new char[HEI_BYT_LEN];

    errno_t e1 =  memcpy_s(str_rep_wid, WID_BYT_LEN, imagebuffer + 3, WID_BYT_LEN);
    errno_t e2 = memcpy_s(str_rep_hei, HEI_BYT_LEN, imagebuffer + 3 + WID_BYT_LEN + 1, HEI_BYT_LEN);

    WIDTH = atoi(str_rep_wid);
    HEIGHT = atoi(str_rep_hei);

    DAT_BYT_STRT = WID_BYT_LEN + HEI_BYT_LEN + 9;

    std ::vector<uint8_t> encodedPNG;
    std ::vector<uint8_t> tocodePNG;
    tocodePNG.reserve(WIDTH * HEIGHT * 4);

    for (int i = DAT_BYT_STRT; i < size; i = i + 3) {
        tocodePNG.push_back((int)(uint8_t)imagebuffer[i]);      // R
        tocodePNG.push_back((int)(uint8_t)imagebuffer[i + 1]);  // G
        tocodePNG.push_back((int)(uint8_t)imagebuffer[i + 2]);  // B
        tocodePNG.push_back(255);                               // A
    }

    
    unsigned error = lodepng::encode(encodedPNG, tocodePNG, WIDTH, HEIGHT);
    if (error)
        return -1;
    else
        lodepng::save_file(encodedPNG, foutname);

    delete fname;
    delete foutname;
    delete str_rep_hei;
    delete str_rep_wid;
    delete[] imagebuffer;
    
    return 0;
}

/*
HEADER -> P3\n1920 1080\n255\n<DATA>
^  
|
3 bytes - > "P3\n" takes 3 bytes which we can ignore (for now)

We need to figure out how many bytes the WIDTH (in this case 1920 takes 4 bytes) ascii char takes.
We also need to figure out how many bytes the HEIGHT (in this case 1080 takes 4 bytes) ascii char takes.

The WIDTH byte (WID_BYT_LEN) will start from 3 and end at ascii " " <- space char
The HEIGHT (HEI_BYT_LEN) byte will start from 3 + WID_BYT_LEN + 1 and end at \n char

The data will start from DAT_BYT_STRT = 3 + WID_BYT_LEN + 1 + HEI_BYT_LEN + 1 + '3' + 1 (in this case 17) bytes
so we need to forward the image data array by DAT_BYT_STRT - 1 bytes.
*/
