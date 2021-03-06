/*
 * IRremote: IRrecvDump - dump details of IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 * LG added by Darryl Smith (based on the JVC protocol)
 */

#include <IRremote.h>

/*
 *  Default is Arduino pin D11.
 *  You can change this to another available Arduino Pin.
 *  Your IR receiver should be connected to the pin defined here
 */
#if defined(ESP32)
int IR_RECEIVE_PIN = 15;
#else
int IR_RECEIVE_PIN = 11;
#endif

IRrecv irrecv(IR_RECEIVE_PIN);

// On the Zero and others we switch explicitly to SerialUSB
#if defined(ARDUINO_ARCH_SAMD)
#define Serial SerialUSB
#endif

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL)
    delay(2000); // To be able to connect Serial monitor after reset and before first printout
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__));
    irrecv.enableIRIn();  // Start the receiver
    irrecv.blink13(true); // Enable feedback LED

    Serial.print(F("Ready to receive IR signals at pin "));
    Serial.println(IR_RECEIVE_PIN);
}

void dump() {
    // Dumps out the decode_results structure.
    // Call this after IRrecv::decode()
    int count = irrecv.results.rawlen;
    if (irrecv.results.decode_type == UNKNOWN) {
        Serial.print("Unknown encoding: ");
    } else if (irrecv.results.decode_type == NEC_STANDARD) {
        Serial.print("Decoded NEC_STANDARD: ");
    } else if (irrecv.results.decode_type == NEC) {
        Serial.print("Decoded NEC: ");
    } else if (irrecv.results.decode_type == SONY) {
        Serial.print("Decoded SONY: ");
    } else if (irrecv.results.decode_type == RC5) {
        Serial.print("Decoded RC5: ");
    } else if (irrecv.results.decode_type == RC6) {
        Serial.print("Decoded RC6: ");
    } else if (irrecv.results.decode_type == PANASONIC) {
        Serial.print("Decoded PANASONIC - Address: ");
        Serial.print(irrecv.results.address, HEX);
        Serial.print(" Value: ");
    } else if (irrecv.results.decode_type == LG) {
        Serial.print("Decoded LG: ");
    } else if (irrecv.results.decode_type == JVC) {
        Serial.print("Decoded JVC: ");
    } else if (irrecv.results.decode_type == AIWA_RC_T501) {
        Serial.print("Decoded AIWA RC T501: ");
    } else if (irrecv.results.decode_type == WHYNTER) {
        Serial.print("Decoded Whynter: ");
    } else if (irrecv.results.decode_type == BOSEWAVE) {
        Serial.print("Decoded Bose Wave Radio / CD: ");
    }
    Serial.print(irrecv.results.value, HEX);
    Serial.print(" (");
    Serial.print(irrecv.results.bits, DEC);
    Serial.println(" bits)");
    Serial.print("Raw (");
    Serial.print(count, DEC);
    Serial.print("): ");

    for (int i = 1; i < count; i++) {
        if (i & 1) {
            Serial.print(irrecv.results.rawbuf[i] * MICROS_PER_TICK, DEC);
        } else {
            Serial.write('-');
            Serial.print((unsigned long) irrecv.results.rawbuf[i] * MICROS_PER_TICK, DEC);
        }
        Serial.print(" ");
    }
    Serial.println();
}

void loop() {
    if (irrecv.decode()) {
        Serial.println(irrecv.results.value, HEX);
        dump();
        irrecv.resume(); // Receive the next value
    }
}
