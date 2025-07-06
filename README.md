# 🧠 IR-Based Customer Detection Robot

This project is an IR-beacon-based autonomous robot designed to detect the presence of a "customer" using analog signal decoding. The robot uses IR sensors and analog ICs to identify a specific IR pulse pattern and respond with real-time motor control. It was built as part of an embedded systems project focusing on analog circuit design and signal-based classification.

---

## 🔧 Tech Stack

- **Hardware**: Arduino Uno, IR sensors, Analog ICs, Motors
- **Programming**: Arduino IDE
- **Features**: Analog signal decoding, real-time classification, motor control

---

## ⚙️ Description

The robot continuously scans for IR beacon signals and classifies them based on a custom analog logic circuit. Once a signal is classified as a "valid customer", ie. when beacon on customer is outputting at 1500Hz the robot stops and the Arduino triggers a motor response to launch a ball inside the customer. The analog circuit acts as a filter to eliminate noise and ambient IR interference.

---

## 🧠 Solution Method

- Designed a signal conditioning and decoding circuit using **operational amplifiers and logic gates** to detect modulated IR patterns.
- Implemented **real-time classification** logic using Arduino to distinguish between beaconed and ambient IR.
- Added motor control logic to trigger an action when a valid customer is detected.
- Used thresholding and pulse-width filtering to avoid false positives.

---

## ✅ Results

- Achieved **100% detection accuracy in detection up to 25 cm** in controlled lighting conditions.
- Stable and repeatable behavior in real-time operation.

---

## 🎥 Demo

https://www.youtube.com/playlist?list=PLRxSesE54WqeDLyvl41pq5ajG2-KKp3UH


