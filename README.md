# Digital Code Lock – Microcontroller Project

## 📌 Project Overview
This project implements a **4-digit digital code lock** on a microcontroller (Atmega88PA) with the following features:

- Input a 4-digit code using three buttons
- Display feedback on an LCD
- Store the code in **EEPROM** (persistent memory)
- Feedback via LEDs (green = correct, red = incorrect)
- Feedback via UART to a PC
- Ability to **update the stored code**
- Controlled as a **finite state machine**

This project is part of the *Microcontroller* course at Hochschule Heilbronn.

---

## ⚙️ Functional Requirements

### 1. Initial State
- On first power-up, a predefined code (e.g., **1111**) is shown on the display.
- This code is stored in EEPROM and persists even when the microcontroller is powered off.
- The initial code is displayed **only once**.

### 2. Code Entry
- Input via **Button 1 (Select Number)** and **Button 2 (Select Position)**:
  - Button 1: Numbers 1 → 2 → … → 0
  - Button 2: Positions 1 → 2 → 3 → 4
- Input is shown live on **line 2** of the display.

### 3. Code Verification
- Press **Button 3** to confirm the entered code.
- Comparison with EEPROM-stored code:
  - **Correct:**  
    - Green LED lights up  
    - UART sends "Code correct"  
    - User enters **update mode**
  - **Incorrect:**  
    - Red LED blinks  
    - UART sends "Code incorrect"  
    - Display shows "Code incorrect"  
    - Return to code entry

### 4. Code Update
- After correct entry, a new code can be entered.
- Pressing Button 3 saves the new code to EEPROM (**overwrites old code**).
- Display shows "Old code: new".
- Flow returns to code entry.

### 5. State Machine
The program has **three main states**:

1. **Initial State** – Display first-time code  
2. **Verification State** – Enter and verify code  
3. **Update State** – Enter and save new code  

State transitions occur only via defined events (button presses, code verification).

---

## 🧠 Program Flow Overview

1. **Init()**
   - Initialize all peripherals
   - Clear the display
   - Write starting code to EEPROM
   - Set startup flags

2. **Main Loop**
   - Triggered every 100 ms via `Timer0_get_100msState()`

3. **Button Handling**
   - `Keys_get_state()` detects button press
   - Switch-Case handles Button 1, 2, 3, KEYS_NOT_PRESSED

4. **Button Actions**
   - **Button 1:** Increment number
   - **Button 2:** Select code position and store number
   - **Button 3:** 
     - State 0 → Verify code
     - State 1 → Update code (EEPROM)

5. **Display Handling**
   Controlled by flags:
   - `ucMerker1` → "Old code"
   - `ucMerker2` → "Code incorrect"
   - `ucMerker3` → "New code"
   - `ucMerker5` → "Old code: new"
   - `ucMerkerAnf` → Initial display

6. **LEDs & UART**
   - Error: Red LED toggles every 90 ms
   - Success: Green LED ON
   - UART sends corresponding text messages

---

## 🔧 Functions Used

| Function | Description |
|----------|-------------|
| `Init()` | Initializes all required modules |
| `MerkerA1245_0()` | Resets flags except `ucMerker3` |
| `alleLEDs_Aus()` | Turns off all LEDs |
| `Schriftweg()` | Clears text on display |
| `LED_ro_toggle()` | Toggles red LED |
| `UART_send_Data(char *data)` | Sends text to PC |
| `eeprom_update_block()` | Writes code to EEPROM |
| `eeprom_read_block()` | Reads code from EEPROM |

---

## 🔢 Key Variables

| Variable | Description |
|----------|-------------|
| `ucCode[4]` | Stores the entered code |
| `ucInkrement` | Number increment 1 → 0 |
| `ucStelle` | Current code position (0–3) |
| `ucFehlervariable` | Error flag if code mismatch |
| `ucMerker1..5` | State/display flags |
| `ucMerkerAnf` | Initial startup flag |
| `uckey` | Button state |
| `pucEPByte[4]` | EEPROM storage array |
| `ucFeld[4]` | Array for EEPROM/code comparison |
| `i` | Loop index |
| Timer variables | 90 ms / 100 ms trigger |
| UART variables | Index + buffer for sending characters |

---

## 🔁 State Diagram
The system works as a **finite state machine** with three main states:

1. **Initial State**  
   Displays stored initial code (first-time only)

2. **Verification State**  
   User enters code → checked → error/success

3. **Update State**  
   User enters new code → stored in EEPROM → back to verification

---

## 📝 Summary
This project implements a complete digital code lock with:

- Persistent memory (EEPROM)  
- Visual and serial feedback  
- Display guidance  
- Structured state machine  

The modular implementation ensures reliable and maintainable behavior.
