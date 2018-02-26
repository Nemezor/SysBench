#include <intel/tcc.h>
#include <stdint.h>
#include <msr.h>

char intel_tcc_contains(char* a, char* b) {
    for (int i = 0, j = 0; i < 48; i++) {
        if (b[j] == '\0') {
            return 1;
        }else if (a[i] == '\0') {
            return 0;
        }else if (a[i] == b[j]) {
            j++;
        }else{
            j = 0;
        }
    }
    return 0;
}

uint8_t intel_tcc_get(char* brand_string) {
    if (intel_tcc_contains(brand_string, "Celeron")) {
        if (intel_tcc_contains(brand_string, "T1700") ||
            intel_tcc_contains(brand_string, "T1600") ||
            intel_tcc_contains(brand_string, "560") ||
            intel_tcc_contains(brand_string, "550") ||
            intel_tcc_contains(brand_string, "540") ||
            intel_tcc_contains(brand_string, "530") ||
            intel_tcc_contains(brand_string, "SU2300")) {
            return 100;
        }else if (intel_tcc_contains(brand_string, "900") ||
                  intel_tcc_contains(brand_string, "U3400")) {
            return 105;
        }else if (intel_tcc_contains(brand_string, "P4505") ||
                  intel_tcc_contains(brand_string, "P4500")) {
            return 90;
        }
    }else if (intel_tcc_contains(brand_string, "Xeon")) {
        if (intel_tcc_contains(brand_string, "5080") ||
            intel_tcc_contains(brand_string, "5063") ||
            intel_tcc_contains(brand_string, "5060") ||
            intel_tcc_contains(brand_string, "5050") ||
            intel_tcc_contains(brand_string, "5030") ||
            intel_tcc_contains(brand_string, "X5282") ||
            intel_tcc_contains(brand_string, "X5272") ||
            intel_tcc_contains(brand_string, "X5270") ||
            intel_tcc_contains(brand_string, "X5260") ||
            intel_tcc_contains(brand_string, "E5240") ||
            intel_tcc_contains(brand_string, "E5220")) {
            return 90;
        }else if (intel_tcc_contains(brand_string, "5160") ||
                  intel_tcc_contains(brand_string, "5150") ||
                  intel_tcc_contains(brand_string, "5148") ||
                  intel_tcc_contains(brand_string, "5140") ||
                  intel_tcc_contains(brand_string, "5130") ||
                  intel_tcc_contains(brand_string, "5120") ||
                  intel_tcc_contains(brand_string, "5110") ||
                  intel_tcc_contains(brand_string, "E5000")) {
            return 80;
        }else if (intel_tcc_contains(brand_string, "L5138")) {
            return 100;
        }else if (intel_tcc_contains(brand_string, "X5000") ||
                  intel_tcc_contains(brand_string, "L5318") ||
                  intel_tcc_contains(brand_string, "L5238") ||
                  intel_tcc_contains(brand_string, "L5408") ||
                  intel_tcc_contains(brand_string, "L5215")) {
            return 95;
        }else if (intel_tcc_contains(brand_string, "L5000") ||
                  intel_tcc_contains(brand_string, "E5205") ||
                  intel_tcc_contains(brand_string, "L5430") ||
                  intel_tcc_contains(brand_string, "L5420") ||
                  intel_tcc_contains(brand_string, "L5410") ||
                  intel_tcc_contains(brand_string, "E5240")) {
            return 70;
        }else if (intel_tcc_contains(brand_string, "X5492") ||
                  intel_tcc_contains(brand_string, "X5482") ||
                  intel_tcc_contains(brand_string, "X5472") ||
                  intel_tcc_contains(brand_string, "X5470") ||
                  intel_tcc_contains(brand_string, "X5460") ||
                  intel_tcc_contains(brand_string, "X5450") ||
                  intel_tcc_contains(brand_string, "E5472") ||
                  intel_tcc_contains(brand_string, "E5462") ||
                  intel_tcc_contains(brand_string, "E5450") ||
                  intel_tcc_contains(brand_string, "E5440") ||
                  intel_tcc_contains(brand_string, "E5430") ||
                  intel_tcc_contains(brand_string, "E5420") ||
                  intel_tcc_contains(brand_string, "E5410") ||
                  intel_tcc_contains(brand_string, "E5405")) {
            return 85;
        }
    }else if (intel_tcc_contains(brand_string, "Core(TM)2")) {
        if (intel_tcc_contains(brand_string, "T5800") ||
            intel_tcc_contains(brand_string, "T5750") ||
            intel_tcc_contains(brand_string, "T5200")) {
            return 85;
        }else if (intel_tcc_contains(brand_string, "T6600") ||
                  intel_tcc_contains(brand_string, "P7550") ||
                  intel_tcc_contains(brand_string, "P7450")) {
            return 90;
        }else if (intel_tcc_contains(brand_string, "X9100") ||
                  intel_tcc_contains(brand_string, "X9000") ||
                  intel_tcc_contains(brand_string, "P9700") ||
                  intel_tcc_contains(brand_string, "P9600") ||
                  intel_tcc_contains(brand_string, "P9500") ||
                  intel_tcc_contains(brand_string, "P8800") ||
                  intel_tcc_contains(brand_string, "P8700") ||
                  intel_tcc_contains(brand_string, "P8600") ||
                  intel_tcc_contains(brand_string, "P8400") ||
                  intel_tcc_contains(brand_string, "P7570") ||
                  intel_tcc_contains(brand_string, "SU9600") ||
                  intel_tcc_contains(brand_string, "SU9400") ||
                  intel_tcc_contains(brand_string, "SU9300") ||
                  intel_tcc_contains(brand_string, "SP9600") ||
                  intel_tcc_contains(brand_string, "SP9400") ||
                  intel_tcc_contains(brand_string, "SL9600") ||
                  intel_tcc_contains(brand_string, "SL9400") ||
                  intel_tcc_contains(brand_string, "SL9380") ||
                  intel_tcc_contains(brand_string, "SL9300") ||
                  intel_tcc_contains(brand_string, "T9900") ||
                  intel_tcc_contains(brand_string, "T9800") ||
                  intel_tcc_contains(brand_string, "T9600") ||
                  intel_tcc_contains(brand_string, "T9550") ||
                  intel_tcc_contains(brand_string, "T9500") ||
                  intel_tcc_contains(brand_string, "T9400") ||
                  intel_tcc_contains(brand_string, "T9300") ||
                  intel_tcc_contains(brand_string, "T8300") ||
                  intel_tcc_contains(brand_string, "T8100") ||
                  intel_tcc_contains(brand_string, "T6670") ||
                  intel_tcc_contains(brand_string, "T6500") ||
                  intel_tcc_contains(brand_string, "T6400")) {
            return 105;
        }else{
            return 100;
        }
    }else if (intel_tcc_contains(brand_string, "Core(TM) i7")) {
        if (intel_tcc_contains(brand_string, "940XM") ||
            intel_tcc_contains(brand_string, "920XM") ||
            intel_tcc_contains(brand_string, "840QM") ||
            intel_tcc_contains(brand_string, "820QM") ||
            intel_tcc_contains(brand_string, "740QM") ||
            intel_tcc_contains(brand_string, "720QM") ||
            intel_tcc_contains(brand_string, "980X")) {
            return 100;
        }else if (intel_tcc_contains(brand_string, "2600")) {
            return 98;
        }else if (intel_tcc_contains(brand_string, "3770T")) {
            return 94;
        }else if (intel_tcc_contains(brand_string, "3770S")) {
            return 103;
        }else if (intel_tcc_contains(brand_string, "660UM") ||
                  intel_tcc_contains(brand_string, "640UM") ||
                  intel_tcc_contains(brand_string, "620UM") ||
                  intel_tcc_contains(brand_string, "640LM") ||
                  intel_tcc_contains(brand_string, "620LM") ||
                  intel_tcc_contains(brand_string, "620M") ||
                  intel_tcc_contains(brand_string, "610E") ||
                  intel_tcc_contains(brand_string, "3770") ||
                  intel_tcc_contains(brand_string, "3770K") ||
                  intel_tcc_contains(brand_string, "3920XM") ||
                  intel_tcc_contains(brand_string, "3820QM") ||
                  intel_tcc_contains(brand_string, "3720QM") ||
                  intel_tcc_contains(brand_string, "3667U") ||
                  intel_tcc_contains(brand_string, "3520M")) {
            return 105;
        }
    }else if (intel_tcc_contains(brand_string, "Core(TM) i5")) {
        if (intel_tcc_contains(brand_string, "540UM") ||
            intel_tcc_contains(brand_string, "520UM") ||
            intel_tcc_contains(brand_string, "430UM") ||
            intel_tcc_contains(brand_string, "540M") ||
            intel_tcc_contains(brand_string, "520M") ||
            intel_tcc_contains(brand_string, "450M") ||
            intel_tcc_contains(brand_string, "430M") ||
            intel_tcc_contains(brand_string, "3570K") ||
            intel_tcc_contains(brand_string, "3570") ||
            intel_tcc_contains(brand_string, "3550") ||
            intel_tcc_contains(brand_string, "3470") ||
            intel_tcc_contains(brand_string, "3450") ||
            intel_tcc_contains(brand_string, "3427U") ||
            intel_tcc_contains(brand_string, "3360M") ||
            intel_tcc_contains(brand_string, "3320M")) {
            return 105;
        }else if (intel_tcc_contains(brand_string, "3470T")) {
            return 91;
        }else if (intel_tcc_contains(brand_string, "3570T")) {
            return 94;
        }else if (intel_tcc_contains(brand_string, "3570S") ||
                  intel_tcc_contains(brand_string, "3550S") ||
                  intel_tcc_contains(brand_string, "3475S") ||
                  intel_tcc_contains(brand_string, "3470S") ||
                  intel_tcc_contains(brand_string, "3450S")) {
            return 103;
        }
    }else if (intel_tcc_contains(brand_string, "Core(TM) i3")) {
        if (intel_tcc_contains(brand_string, "330UM") ||
            intel_tcc_contains(brand_string, "330E") ||
            intel_tcc_contains(brand_string, "370M") ||
            intel_tcc_contains(brand_string, "350M") ||
            intel_tcc_contains(brand_string, "330M")) {
            return 105;
        }
    }else if (intel_tcc_contains(brand_string, "Core(TM)")) {
        return 100;
    }else if (intel_tcc_contains(brand_string, "Atom")) {
        if (intel_tcc_contains(brand_string, "330") ||
            intel_tcc_contains(brand_string, "320")) {
            return 125;
        }else if (intel_tcc_contains(brand_string, "S1240")) {
            return 102;
        }else if (intel_tcc_contains(brand_string, "CE4170") ||
                  intel_tcc_contains(brand_string, "CE4150") ||
                  intel_tcc_contains(brand_string, "CE4110") ||
                  intel_tcc_contains(brand_string, "E665CT") ||
                  intel_tcc_contains(brand_string, "E645CT") ||
                  intel_tcc_contains(brand_string, "E680T") ||
                  intel_tcc_contains(brand_string, "E660T") ||
                  intel_tcc_contains(brand_string, "E640T") ||
                  intel_tcc_contains(brand_string, "E620T")) {
            return 110;
        }else if (intel_tcc_contains(brand_string, "N570") ||
                  intel_tcc_contains(brand_string, "N550") ||
                  intel_tcc_contains(brand_string, "N475") ||
                  intel_tcc_contains(brand_string, "N470") ||
                  intel_tcc_contains(brand_string, "N455") ||
                  intel_tcc_contains(brand_string, "N450") ||
                  intel_tcc_contains(brand_string, "D525") ||
                  intel_tcc_contains(brand_string, "D510") ||
                  intel_tcc_contains(brand_string, "D425") ||
                  intel_tcc_contains(brand_string, "D410") ||
                  intel_tcc_contains(brand_string, "K525") ||
                  intel_tcc_contains(brand_string, "K510") ||
                  intel_tcc_contains(brand_string, "K425") ||
                  intel_tcc_contains(brand_string, "K410") ||
                  intel_tcc_contains(brand_string, "D2700") ||
                  intel_tcc_contains(brand_string, "D2550") ||
                  intel_tcc_contains(brand_string, "D2500") ||
                  intel_tcc_contains(brand_string, "N2850") ||
                  intel_tcc_contains(brand_string, "N2800") ||
                  intel_tcc_contains(brand_string, "N2650") ||
                  intel_tcc_contains(brand_string, "N2600")) {
            return 100;
        }else if (intel_tcc_contains(brand_string, "S1260") ||
                  intel_tcc_contains(brand_string, "S1220")) {
            return 95;
        }else{
            return 90;
        }
    }
    return 90;
}
