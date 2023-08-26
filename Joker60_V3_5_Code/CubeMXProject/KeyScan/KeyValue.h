#ifndef __KEYVALUE_H__
#define __KEYVALUE_H__

#define KeyFN 0
#define KeyPN 0

//a-z
#define KeyA 0x04
#define KeyB 0x05
#define KeyC 0x06
#define KeyD 0x07
#define KeyE 0x08
#define KeyF 0x09
#define KeyG 0x0A
#define KeyH 0x0B
#define KeyI 0x0C
#define KeyJ 0x0D
#define KeyK 0x0E
#define KeyL 0x0F
#define KeyM 0x10
#define KeyN 0x11
#define KeyO 0x12
#define KeyP 0x13
#define KeyQ 0x14
#define KeyR 0x15
#define KeyS 0x16
#define KeyT 0x17
#define KeyU 0x18
#define KeyV 0x19
#define KeyW 0x1A
#define KeyX 0x1B
#define KeyY 0x1C
#define KeyZ 0x1D

//1-0
#define Key1 0x1E
#define Key2 0x1F
#define Key3 0x20
#define Key4 0x21
#define Key5 0x22
#define Key6 0x23
#define Key7 0x24
#define Key8 0x25
#define Key9 0x26
#define Key0 0x27

//F1 - F12
#define KeyF1 0x3A
#define KeyF2 0x3B
#define KeyF3 0x3C
#define KeyF4 0x3D
#define KeyF5 0x3E
#define KeyF6 0x3F
#define KeyF7 0x40
#define KeyF8 0x41
#define KeyF9 0x42
#define KeyF10 0x43
#define KeyF11 0x44
#define KeyF12 0x45

//OTHER
#define KeyRETURN 0x28
#define KeyESC 0x29
#define KeyBACKSPACE 0x2A
#define KeyTAB 0x2B
#define KeySPACE 0x2C
#define KeyMINUS 0x2D       //  - _
#define KeyEQUAL 0x2E
#define KeyLBRACKET 0x2F    //  [ {
#define KeyRBRACKET 0x30    //  ] }
#define KeyBSLASH 0x31      //  \\ |
//#define Key0 0x32 
#define KeySEMICOLON 0x33   //  ;
#define KeyQUOTA 0x34       //  '
#define KeyGRAVE 0x35       //  ` ~
#define KeyCOMMA 0x36       //  , <
#define KeyPERIOD 0x37      //  . >
#define KeyFSLASH 0x38      // / ?
#define KeyCAPSLOCK 0x39       //

#define KeyPRINTSCREEN 0x46
#define KeySCROLLLOCK 0x47
#define KeyPAUSE 0x48
#define KeyINSERT 0x49
#define KeyHOME 0x4A
#define KeyPAGEUP 0x4B
#define KeyDELETE 0x4C
#define KeyEND 0x4D
#define KeyPAGEDOWN 0x4E
#define KeyRARROW 0x4F
#define KeyLARROW 0x50
#define KeyDARROW 0x51
#define KeyUARROW 0x52
#define KeyNUMLOCK 0x53

// Modifiers
#define ModifierStart 0xDF //if KeyValue > ModifierStart then it is modifiers

#define KeyLCtrl 0xE0
#define KeyLShift 0xE1
#define KeyLAlt 0xE2
#define KeyLGui 0xE3
#define KeyRCtrl 0xE4
#define KeyRShift 0xE5
#define KeyRAlt 0xE6
#define KeyRGui 0xE7



#endif
