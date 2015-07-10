#define BVK_SHEEMfindLen	4	// длина запроса для поиска БВК-ШИМ			 	
#define BVK_SHEEM_BuLen		4+8
#define BVK_SHEEM_Err			0xff
#define BVK_SHEEM_CAN_ID	16
#define defPWMkey1 0
#define defPWMkey2 0
#define defPWMkey3 0
#define defPWMkey4 0
#define defPWMkey5 0
#define defPWMkey6 0
#define defPWMkey7 0
#define defPWMkey8 0

void FindBVK_SHEEM(unsigned char	addr);
void BVK_SHEEM_Ask01(void);
void BVK_SHEEM_Ask02(void);
void BVK_SHEEM_Send05(void);
void BVK_SHEEM_Send06(void);
void BVK_SHEEMPause(void);
unsigned char BVK_SHEEM_WorkCAN(unsigned char command);
extern unsigned char BVK_SHEEMinf[BVK_SHEEM_BuLen];
extern unsigned char BVK_SHEEMkey;

