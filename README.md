# MediTag: Smart Medication Inventory System 💊  

A cutting-edge RFID and ESP8266-powered smart medication inventory system designed to enhance medication adherence and inventory management. MediTag combines real-time tracking, seamless communication, and automated notifications to revolutionize medication management.  

---

## ✨ Key Features  

- **RFID-Powered Tracking**:  
   - Utilizes RFID tags to monitor medication inventory in real-time.  
   - Tracks medication scans for precise adherence records.  

- **ESP8266 Microcontroller Integration**:  
   - Enables seamless communication with cloud-based services.  
   - Supports real-time email notifications for taken or missed doses.  

- **Energy Efficiency**:  
   - Innovative push-button switch powers the system only during use, conserving energy.  

- **Real-Time Notifications**:  
   - Sends email updates for adherence tracking and missed doses.  
   - Automated reminders ensure users stay consistent with prescriptions.  

---

## 🛠️ Technical Specifications  

### Hardware Components  
- **RFID Scanner**: RC522 scanner for identifying medication using 13.56MHz ISO 14443A RFID tags.  
- **ESP8266 Microcontroller**: Handles communication and triggers email notifications.  
- **Power Supply**:  
   - 9V battery for powering the ESP8266.  
   - Push-button switch optimizes energy usage.  

### Software Components  
- **Google Forms & Sheets**:  
   - Collects patient information (name, email, mobile number, medication, dosage, end date).  
   - Logs medication data for real-time tracking.  
   - [Access the Google Sheets Setup Here](https://bit.ly/MediTag).  
- **Google Apps Script**:  
   - Automates email reminders and confirmations.  
   - Highlights matched records in Google Sheets for better visualization.  

---

## 🚀 Working Principle  

1. **RFID Scanning**: Medication with RFID tags is scanned upon drawer entry/exit.  
2. **Data Logging**:  
   - Logs medication data in Google Sheets.  
   - Matches entries to ensure adherence and trigger notifications.  
3. **Notification System**:  
   - Sends confirmation emails for taken doses.  
   - Reminders are sent for missed doses based on time-based triggers.  
4. **Energy Efficiency**:  
   - Push-button switch controls the power to ESP8266, conserving energy.  

---

## 📈 System Performance  

- **Time-Based Triggers**:  
   - Email Confirmation: 0.02% error rate.  
   - Email Reminder: 0.06% error rate.  
   - Highlighting Matching Cells: 0.1% error rate.  
- **RFID Scanner Range**: Approximately 3 cm for accurate detection.  
- **Trigger Frequency**: Fires every 60 seconds, with function activation in ~90 seconds.  

---

## 🌟 Benefits  

- **Enhanced Adherence**: Ensures users follow prescribed medication schedules.  
- **Streamlined Management**: Simplifies tracking for both users and healthcare providers.  
- **Eco-Friendly**: Energy-efficient design minimizes power consumption.  
- **Scalable Solution**: Can be adapted for larger inventories and multi-patient setups.  

---

## 🎯 Future Enhancements  

- Integration with mobile applications for user-friendly alerts.  
- Advanced analytics for medication adherence trends.  
- Cloud-based storage for multi-device access and enhanced data security.  
- Direct notifications to healthcare providers for non-adherence alerts.  
