<h1 align="center">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&weight=700&size=30&pause=1000&color=6C63FF&center=true&vCenter=true&width=600&lines=🏭+Smart+Weight+Measurement;+%26+Grading+System;for+Rahimafrooz+🔋" alt="Typing SVG" />
</h1>

<p align="center">
  <img src="https://img.shields.io/badge/Client-Rahimafrooz%20Batteries%20Ltd.-red?style=for-the-badge&logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSI0MCIgaGVpZ2h0PSI0MCIgdmlld0JveD0iMCAwIDQwIDQwIj48Y2lyY2xlIGN4PSIyMCIgY3k9IjIwIiByPSIxOCIgZmlsbD0iI2ZmZmZmZiIgc3Ryb2tlPSIjZWIwMDAwIiBzdHJva2Utd2lkdGg9IjIiLz48cGF0aCBkPSJNMTAgMTBoMjB2MjBIMTB6IiBmaWxsPSIjZWIwMDAwIi8+PC9zdmc+&logoColor=white" />
  <img src="https://img.shields.io/badge/Platform-Arduino%20Uno-00979D?style=for-the-badge&logo=arduino&logoColor=white" />
  <img src="https://img.shields.io/badge/Sensor-HX711%20Load%20Cell-FF6F00?style=for-the-badge&logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSI0MCIgaGVpZ2h0PSI0MCIgdmlld0JveD0iMCAwIDQwIDQwIj48cmVjdCB4PSI1IiB5PSI1IiB3aWR0aD0iMzAiIGhlaWdodD0iMzAiIGZpbGw9IiNmZmZmZmYiIHN0cm9rZT0iIzAwMCIgc3Ryb2tlLXdpZHRoPSIyIi8+PC9zdmc+&logoColor=white" />
  <img src="https://img.shields.io/badge/Storage-EEPROM%20(512B)-4A90E2?style=for-the-badge&logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSI0MCIgaGVpZ2h0PSI0MCIgdmlld0JveD0iMCAwIDQwIDQwIj48cmVjdCB4PSI1IiB5PSI1IiB3aWR0aD0iMzAiIGhlaWdodD0iMzAiIGZpbGw9IiM0QTkwRTIiLz48L3N2Zz4=&logoColor=white" />
  <img src="https://img.shields.io/badge/Status-Production%20Ready-brightgreen?style=for-the-badge" />
</p>

<p align="center">
  <i>An industrial-grade, Arduino-based solution for precise battery/material weight classification,</i>
  <br />
  <i>custom-built for Rahimafrooz Batteries Limited’s quality control & R&D labs.</i>
</p>

---

## 🏢 About Rahimafrooz & This Project

**Rahimafrooz Batteries Limited** is one of South Asia's leading battery manufacturers, renowned for its innovation in energy storage.

This project was developed **specifically for their industrial quality control (QC) process** to automate and standardize weight-based grading of battery components. Instead of manual weighing, this system provides:

- ✅ **Rapid, accurate weight measurement** using a strain-gauge load cell (HX711).
- ✅ **Automated classification** into "Low", "Medium", or "High" grades.
- ✅ **Visual & Audio feedback** (LEDs + Buzzer) for operator efficiency.
- ✅ **Persistent calibration storage** (EEPROM) — retains settings even after power loss.
- ✅ **Group selection** for different product batches (pre-set weight thresholds).

> 📌 *This project replaces manual error-prone weighing with a semi-automated, affordable smart station.*

---

## 🏗️ System Architecture & Workflow

The system follows a sequential logic flow:

```mermaid
flowchart TD
    A[Power ON / Reset] --> B[Load Calibration from EEPROM]
    B --> C{Calibration Valid?}
    C -->|No| D[Enter Calibration Mode]
    C -->|Yes| E[Ready: Select Group 1/2/3]
    D --> F[Place Calibration Weight]
    F --> G[Save to EEPROM]
    G --> E

    E --> H[Place Item on Scale]
    H --> I[HX711 Reads Weight]
    I --> J{Compare with Thresholds}
    J -->|Below Low| K[Grade: LOW - Red LED]
    J -->|Medium| L[Grade: MEDIUM - Yellow LED]
    J -->|High| M[Grade: HIGH - Green LED]
    K & L & M --> N[Display Grade + Weight on LCD]
    N --> O[Buzzer Beep to Acknowledge]
    O --> P{Button Press?}
    P -->|Reset/Tare| H
    P -->|Next Item| H
