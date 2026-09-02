#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD I2C address to 0x27, with a size of 16 columns and 2 rows
// 设置LCD I2C地址为0x27，尺寸为16列2行
LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int switchPin = 2;    // Toggle switch connected to digital pin 2 (拨动开关引脚)
const int greenLedPin = 12; // Green LED for RUNNING state (绿灯代表运行状态，引脚12)
const int redLedPin = 13;   // Red LED for STOPPED state (红灯代表停止状态，引脚13)

String message = "Selamat datang ke Politeknik Ibrahim Sultan"; // Welcome message to scroll

// Record the last switch state to avoid flickering when static
// 记录上一次的开关状态，用于防止静态时的无意义刷新闪烁
int lastSwitchState = -1; 

void setup() {
  lcd.init();          // Initialize the LCD screen (初始化LCD屏幕)
  lcd.backlight();     // Turn on the LCD backlight (打开LCD背光)
  
  pinMode(switchPin, INPUT_PULLUP); // Activate internal pull-up resistor on pin 2 (激活内部上拉电阻)
  pinMode(greenLedPin, OUTPUT);     // Set green LED pin as output (设置绿灯引脚为输出)
  pinMode(redLedPin, OUTPUT);       // Set red LED pin as output (设置红灯引脚为输出)
}

void loop() {
  // Read current state of toggle switch (读取当前拨动开关状态)
  int currentSwitchState = digitalRead(switchPin);

  // If the switch is ON (LOW)
  // 如果开关处于 ON 状态（低电平 LOW）
  if (currentSwitchState == LOW) {
    // Turn ON Green LED, turn OFF Red LED (绿灯亮，红灯灭)
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin, LOW);

    // Execute marquee scrolling with controlled speed
    // 运行调整好速度的跑马灯滚动
    scrollMessageClassic(message);
    
    // Update state record (更新状态记录)
    lastSwitchState = LOW; 
  } 
  else {
    // If the switch is OFF (HIGH)
    // 如果开关处于 OFF 状态（断开，电平为 HIGH）
    
    // Turn OFF Green LED, turn ON Red LED (绿灯灭，红灯亮)
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);

    // Refresh display only ONCE when transitioning to OFF to prevent static flickering
    // 状态切换至 OFF 时仅清屏并重绘一次，保持画面静止且彻底无闪烁
    if (lastSwitchState != HIGH) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" System Stopped ");
      lcd.setCursor(0, 1);
      lcd.print(" Turn on to run ");
      
      lastSwitchState = HIGH;
    }
  }
}

// Custom function: Classic marquee scrolling without second-line status text
// 自定义函数：移除第二行状态文字的经典跑马灯滚动
void scrollMessageClassic(String msg) {
  // Pad the message with 16 spaces on both ends for smooth entry and exit
  // 在字符串前后加上 16 个空格，使文字自然进出 16 列屏幕
  String paddedMsg = "                " + msg + "              ";
  
  // Loop through the padded string
  for (int i = 0; i <= paddedMsg.length() - 16; i++) {
    // Immediate check during scroll: exit loop immediately if switch is flipped OFF
    // 滚动中途检测：若开关切到 OFF，立即跳出循环停止
    if (digitalRead(switchPin) == HIGH) {
      break; 
    }
    
    // Extract a 16-character slice of the message
    // 截取 16 个字符宽度的窗口
    String displayWindow = paddedMsg.substring(i, i + 16);
    
    // Clear and print for the retro digital display movement
    // 清屏并打印
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(displayWindow);
    
    // Second line left blank for a clean display
    // 第二行保持留空，使屏幕更加干净简洁
    lcd.setCursor(0, 1);
    lcd.print("                ");
    
    // Delay set to 280ms for a balanced, comfortable scrolling rhythm
    // 延迟设置为 280ms
    delay(280); 
  }
}
