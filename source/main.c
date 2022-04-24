#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

void initializeWii();
int getOperand();
bool getOperator();

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

int main(int argc, char **argv) {
    initializeWii();

    int a;
    int b;
    bool op;

    printf("Jacob's Wii Calculator\n");

    u32 input = false;
    printf("Press (A) to proceed. . .");
    do{
        WPAD_ScanPads();
        input = WPAD_ButtonsDown(0);
    }while(!input);

    switch(input) {
		case WPAD_BUTTON_A:
			break;
		case WPAD_BUTTON_HOME:
			printf("\n\nHome - Exiting Program");
			printf("\nSee ya!");
			exit(0);
		default:
			break;
	}
    
    a = getOperand();
    op = getOperator();
    b = getOperand();

    printf("\n\n Answer: %d", op ? a+b : a-b);
    return 0;
}

void initializeWii() {
    VIDEO_Init();
    WPAD_Init();

    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    console_init(xfb, 20, 20, rmode -> fbWidth, rmode -> xfbHeight, rmode -> fbWidth * VI_DISPLAY_PIX_SZ);
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);

    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();

    VIDEO_WaitVSync();
    if(rmode -> viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
    printf("\x1b[2;0H");
}

int getOperand() {
    printf("\e[1;1H\e[2J");
    int operand = 0;
    bool done = false;

    do{

        u32 input = false;
        printf("(A) Confirm (v) Decrease (^) Increase\n\nNumber: ");
		printf("%d", operand);

        do{
            WPAD_ScanPads();
            input = WPAD_ButtonsDown(0);
        }while(!input);
        
		switch(input) {
			case WPAD_BUTTON_A:
				return operand;
			case WPAD_BUTTON_UP:
				u32 held = false;
                do{
                    operand++;
                    WPAD_ScanPads();
                    held = WPAD_ButtonsHeld(0);
                    printf("\e[1;1H\e[2J");
                    sleep(.1);
                }while(held);
				break;
			case WPAD_BUTTON_DOWN:
				u32 held2 = false;
                do{
                    operand--;
                    WPAD_ScanPads();
                    held2 = WPAD_ButtonsHeld(0);
                    printf("\e[1;1H\e[2J");
                    sleep(.1);
                }while(held2);
				break;
			case WPAD_BUTTON_HOME:
				printf("\n\nHome - Exiting Program");
				printf("\nSee ya!");
				exit(0);
			default:
				break;
		}
        printf("\e[1;1H\e[2J");

    } while(!done);
    return operand;
}

bool getOperator() {
    printf("\e[1;1H\e[2J");
    u32 input = false;
    printf("(+) Add or (-) Subtract");

    do{
        WPAD_ScanPads();
        input = WPAD_ButtonsDown(0);
    }while(!input);

    switch(input) {
			case WPAD_BUTTON_PLUS:
				return true;
			case WPAD_BUTTON_MINUS:
				return false;
				break;
			case WPAD_BUTTON_HOME:
				printf("\n\nHome - Exiting Program");
				printf("\nSee ya!");
				exit(0);
			default:
				break;
		}
    return true;
}