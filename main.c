#define TRUE  (1)
#define FALSE (0)

#include <built_in.h>

// LCD pin configuration
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D7 at RB3_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D4 at RB0_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D7_Direction at TRISB3_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D4_Direction at TRISB0_bit;

// IR sensor pin configuration
sbit IR_SENSOR_F at RC1_bit;
sbit IR_SENSOR_F_Direction at TRISC1_bit;
sbit IR_SENSOR_P at RC0_bit;
sbit IR_SENSOR_P_Direction at TRISC0_bit;
sbit IR_SENSOR_C at RC2_bit;
sbit IR_SENSOR_C_Direction at TRISC2_bit;

// LED pin configuration
sbit LED_GREEN at RC4_bit;
sbit LED_YELLOW at RC5_bit;
sbit LED_RED at RA2_bit;

sbit LED_GREEN_Direction at TRISC4_bit;
sbit LED_YELLOW_Direction at TRISC5_bit;
sbit LED_RED_Direction at TRISA2_bit;

// UART pin configuration
sbit UART_RX_Direction at TRISC7_bit; // RX pin
sbit UART_TX_Direction at TRISC6_bit; // TX pin

volatile unsigned long milliseconds = 0;

// Function to initialize Timer0
void Timer0_Init() {
    //Enable Global Interrupt pin
    INTCON.GIE = 1;
    //Enable peripheral Interrupts
    INTCON.PEIE = 1;
    //TMR1 Overflow Interrupt Enable bit
    PIE1.TMR1IE = 1;

    //preload value
    TMR1H = 0xEC;
    TMR1L = 0x78;

    //start Timer
    T1CON.TMR1ON = 1;
}

// Timer0 interrupt service routine
void interrupt() {
    //Check if Timer1 Overflow Interrupt Occured
    if (PIR1.TMR1IF == 1)
    {
      TMR1H = 0xEC;
      TMR1L = 0x78;
      milliseconds++;
      PIR1.TMR1IF = 0;
    }
}

// Function to initialize the LCD
void Lcd_Init_Custom() {
    Lcd_Init();
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Lcd_Cmd(_LCD_CLEAR);
}

// Function to display the cat face on the LCD
void display_cat_face(char *face1, char *face2, int F, int P, int C) {
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1, 1, face1);
    Lcd_Out(2, 1, face2);
    Lcd_Chr(1, 12, 'F');
    Lcd_Chr(1, 14, 'P');
    Lcd_Chr(1, 16, 'C');
    Lcd_Chr(2, 12, F + '0');
    Lcd_Chr(2, 13, ' ');
    Lcd_Chr(2, 14, P + '0');
    Lcd_Chr(2, 15, ' ');
    Lcd_Chr(2, 16, C + '0');
    delay_ms(1000); // Display for 1 second
}

// Function to initialize UART for Bluetooth communication
void UART_Init_Custom() {
    UART1_Init(9600); // Initialize UART at 9600 baud rate
    Delay_ms(100); // Wait for UART to stabilize
}

// Function to send a string via Bluetooth
void UART_Send_String(char *str) {
    while (*str) {
        UART1_Write(*str++);
    }
}

// Function to receive a string via Bluetooth
char UART_Receive() {
    if (UART1_Data_Ready()) {
        return UART1_Read();
    }
    return '\0'; // Return null if no data is available
}

void main() {
    int F = 5, P = 5, C = 5;
    int elapsed_time_P = 0, elapsed_time_F = 0, elapsed_time_C = 0;
    unsigned int sum;
    int last_ir_state_F = 0, last_ir_state_P = 0, last_ir_state_C = 0;
    unsigned long ir_start_time_F = 0, ir_start_time_P = 0, ir_start_time_C = 0;
    char bluetooth_command;

    // Define the ASCII cat faces

    char upset_bottom[] = "( - - )";
    char suicidal_bottom[] = "( x x )";
    char face_top[] = " ^ _ ^ ";
    char ecstatic_bottom[] = "( *0* )";
    char happy_bottom[] = "( 0.0 )";
    char neutral_bottom[] = "( o o )";


    // Initialize Timer0
    Timer0_Init();

    // Initialize the LCD
    Lcd_Init_Custom();

    // Initialize UART for Bluetooth communication
    UART_Init_Custom();

    // Set IR sensor pins as input
    IR_SENSOR_F_Direction = 1;
    IR_SENSOR_P_Direction = 1;
    IR_SENSOR_C_Direction = 1;

    // Set LED pins as output
    LED_GREEN_Direction = 0;
    LED_YELLOW_Direction = 0;
    LED_RED_Direction = 0;

    // Set UART pins
    UART_RX_Direction = 1; // Set RX pin as input
    UART_TX_Direction = 0; // Set TX pin as output

    // Initial display update
    display_cat_face(face_top, ecstatic_bottom, F, P, C);

    while (TRUE) {
        int elapsed_time = milliseconds / 1000; // Convert milliseconds to seconds
        int variable_changed = 0;

        // Check for Bluetooth commands
        bluetooth_command = UART_Receive();
        if (bluetooth_command != '\0') {
            // Process the command (example: increase or decrease values)
            switch (bluetooth_command) {
                case 'f': if (F < 5) F++; break;
                case 'p': if (P < 5) P++; break;
                case 'c': if (C < 5) C++; break;

                case 'F': if (F < 4) F+=2; break;
                case 'P': if (P < 4) P+=2; break;
                case 'C': if (C < 4) C+=2; break;

            }
            variable_changed = 1;
        }

        // Check if IR sensor F is triggered
        if (IR_SENSOR_F == 1) {
            if (last_ir_state_F == 0) { // IR sensor just triggered
                ir_start_time_F = milliseconds;
                variable_changed = 1;
            } else { // IR sensor still triggered
                if ((milliseconds - ir_start_time_F) >= 3000) { // 3 seconds have passed
                    if (F > 0) {
                        F--;
                        variable_changed = 1;
                    }
                    ir_start_time_F += 3000; // Reset the start time for the next 3 seconds
                }
            }
        } else {
            if (F < 5) {
                F++;
                variable_changed = 1;
            }
        }
        last_ir_state_F = IR_SENSOR_F;

        // Check if IR sensor P is triggered
        if (IR_SENSOR_P == 1) {
            if (last_ir_state_P == 0) { // IR sensor just triggered
                ir_start_time_P = milliseconds;
                variable_changed = 1;
            } else { // IR sensor still triggered
                if ((milliseconds - ir_start_time_P) >= 5000) { // 5 seconds have passed
                    if (P > 0) {
                        P--;
                        variable_changed = 1;
                    }
                    ir_start_time_P += 5000; // Reset the start time for the next 5 seconds
                }
            }
        } else {
            if (P < 5) {
                P++;
                variable_changed = 1;
            }
        }
        last_ir_state_P = IR_SENSOR_P;

        // Check if IR sensor C is triggered
        if (IR_SENSOR_C == 1) {
            if (last_ir_state_C == 0) { // IR sensor just triggered
                ir_start_time_C = milliseconds;
                variable_changed = 1;
            } else { // IR sensor still triggered
                if ((milliseconds - ir_start_time_C) >= 7000) { // 7 seconds have passed
                    if (C > 0) {
                        C--;
                        variable_changed = 1;
                    }
                    ir_start_time_C += 7000; // Reset the start time for the next 7 seconds
                }
            }
        } else {
            if (C < 5) {
                C++;
                variable_changed = 1;
            }
        }
        last_ir_state_C = IR_SENSOR_C;

        // Print the appropriate cat face if a variable has changed
        if (variable_changed) {
            sum = F + P +  C;
            if (sum == 15) {
                display_cat_face(face_top, ecstatic_bottom, F, P, C);
                LED_GREEN = 1;
                LED_YELLOW = 0;
                LED_RED = 0;
                UART_Send_String("Ecstatic\n");
            } else if (sum >= 10 && sum <= 14) {
                display_cat_face(face_top, happy_bottom, F, P, C);
                LED_GREEN = 1;
                LED_YELLOW = 0;
                LED_RED = 0;
                UART_Send_String("Happy\n");
            } else if (sum >= 6 && sum <= 9) {
                display_cat_face(face_top, neutral_bottom, F, P, C);
                LED_GREEN = 0;
                LED_YELLOW = 1;
                LED_RED = 0;
                UART_Send_String("Neutral\n");
            } else if (sum >= 1 && sum <= 5) {
                display_cat_face(face_top, upset_bottom, F, P, C);
                LED_GREEN = 0;
                LED_YELLOW = 0;
                LED_RED = 1;
                UART_Send_String("Upset\n");
            } else if (sum == 0) {
                display_cat_face(face_top, suicidal_bottom, F, P, C);
                LED_GREEN = 0;
                LED_YELLOW = 0;
                LED_RED = 1;
                UART_Send_String("Suicidal\n");
            }
        }

        // Update display every 100 milliseconds
        delay_ms(100);
    }
}

