#include "nco.h"
#include "MG32x02z__RegAddress.h"
#include "MG32x02z_APB.h"
#include "core.h"


void nco_init(uint8_t mode) {
  RB(APB_NCO00_b3)=
    APB_NCO00_NCO0_CKS_ck_apb_b3 |   // 12 МГц
    mode |
    APB_NCO00_NCO0_EN_enable_b3;     // enable
  RW(APB_NCO01_w)= APB_NCO01_NCO0_PWS_ck4_w; // APB_NCO0_PWS = DIV/4
}


void nco_set(uint32_t k) {
  RH(APB_NCO00_h0) = k & 0xffff;
  RB(APB_NCO00_b2) = k >> 16;
}

