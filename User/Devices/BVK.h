#define BVKfindLen	4	// длина запроса для поиска БВК 			 	
#define BVKbuLen		4+8
#define BVK_CAN_ID	8
#define cDefBVKkey  0
void FindBVK(unsigned char	addr);
void BVK_Ask01(void);
void BVK_Ask02(void);
void BVK_Send04(void);
void BVK_Send06(void);
unsigned char BVK_WorkCAN(unsigned char command);
extern unsigned char BVKinf[BVKbuLen];
extern unsigned char BVKkey;
