# Introduction to Robotics (2022 - 2023)
Introduction to Robotics laboratory homework, taken in the 3rd year at the Faculty of Mathematics and Computer Science, University of Bucharest. Each homework includes requirements, implementation details, code and image files.

<details>
<summary><h3>💻 Homework #0 : Setup</h3></summary>
<br>

**Task Requirements:** Create and setup a public GitHub repository containing info about the current and future homework for this course, and install the Arduino IDE.
</details>

<details>
<summary><h3>🚨 Homework #1 : RGB Led Controlled by Three Potentiometers</h3></summary>
<br>

**Task Requirements:** Use three separate potentiometers to control each color of an RGB led (Red, Green and Blue). The control must be done with digital electronics (aka the value of the potentiometer must be read with Arduino, and a mapped value must be written to each of the pins connected
to the led).

**Components Used:**
* 1 RBG LED
* 3 potentiometers
* resistors and wires (per logic)

**Setup Picture:** ![poza tema 1 lab robotica](https://user-images.githubusercontent.com/87432371/197353566-af59eeda-ca8c-477d-97d2-0b4c5ef624bc.jpg)

**Task Showcase:** https://www.youtube.com/watch?v=oJw1XVFhmSI&ab_channel=AlexandruSasu
</details>

<details>
<summary><h3>🚦 Homework #2 : Simulate the Traffic Lights for a Crosswalk</h3></summary>
<br>

**Task Requirements:** Build the traffic lights for a crosswalk, that emits sounds depending on the states of the semaphores. The crosswalk is initiated by pressing a button. Two LEDs must be used to represent the traffic lights for people (red and green) and 3 LEDs to represent the traffic lights for cars (red, yellow and green). Below are the states that the crosswalk will go through.
* State 1 (default, reinstated after state 4 ends): green light for cars, red light for people, no sounds. Duration: indefinite, changed by pressing the button.
* State 2 (initiated by counting down 8 seconds after a button press): the light should be yellow for cars, red for people, and no sounds. Duration: 3 seconds.
* State 3 (initiated after state 2 ends): red for cars, green for people, and a beeping sound from the buzzer at a constant interval. Duration: 8 seconds.
* State 4 (initiated after state 3 ends): red for cars, blinking green for people, and a beeping sound from the buzzer, at a constant interval, faster than the beeping in state 3. Duration: 4 seconds.
> Note: Pressing the button in any state other than state 1 does not yield any actions.

**Components Used:**
* 5 LEDs
* 1 push button
* 1 buzzer
* resistors and wires (per logic)

**Setup Picture:** ![poza tema 2 lab robotica](https://user-images.githubusercontent.com/87432371/199686660-a2e49f70-a826-460d-917b-6542ec478b71.jpg)

**Task Showcase:** https://www.youtube.com/watch?v=hJEe0grXwIo&ab_channel=AlexandruSasu
</details>

<details>
<summary><h3>🕹 Homework #3 : 7-Segment Display Controlled Through the Use of a Joystick</h3></summary>
<br>

**Task Requirements:** A joystick will be used in order to navigate through the LEDs on a 7-segment display, and change their states. The movement between LEDs should be natural (meaning they should jump from the current position only to neighbors, but without passing through ”walls”. Below are the states that the system will go through.
* State 1 (default, but also initiated after a button press in state 2): Current position blinking. Can use the joystick to move from one position to neighbors. Short pressing the button toggles state 2. Long pressing the button in state 1 resets the entire display by turning all the LEDs OFF, and moving the current position to the decimal point.
* State 2 (initiated after a button press in state 1): The current LED stops blinking, adopting the state of the LED before selection (ON or OFF). Toggling the X (or Y, either one is fine) axis should change the segment state from ON to OFF or from OFF to ON. Clicking the joystick should save the LED state and exit back to state 1. 
> Note: Long pressing the button to reset should only be available in state 1.

The movement for each LED:

| Current LED | UP  | DOWN | LEFT | RIGHT |
| :---------: | :-: | :--: | :--: | :---: |
|      a      | N/A |  g   |  f   |   b   |
|      b      |  a  |  g   |  f   |  N/A  |
|      c      |  g  |  d   |  e   |  dp   |
|      d      |  g  | N/A  |  e   |   c   |
|      e      |  g  |  d   | N/A  |   c   |
|      f      |  a  |  g   | N/A  |   b   |
|      g      |  a  |  d   | N/A  |  N/A  |
|     dp      | N/A | N/A  |  c   |  N/A  |

**Components Used:**
* 1 7-segment display
* 1 joystick
* resistors and wires (per logic)

**Setup Picture:** ![poza tema 3 lab robotica](https://user-images.githubusercontent.com/87432371/200939017-cff191bf-2ccd-4f08-a1a1-1c71f8ba71ed.jpg)

**Task Showcase:** https://www.youtube.com/watch?v=hIlzHWG2Sw4&ab_channel=AlexandruSasu
</details>

<details>
<summary><h3>🔢 Homework #4 : 4-Digit 7-Segment Display Controlled Through the Use of a Joystick</h3></summary>
<br>

**Task Requirements:** A joystick will be used as means of navigating through a 4-digit 7-segment display's digits and changing their values. Pressing the joystick's button will lock the system in on the current digit, and the Y axis will be made available, so as to increment or decrement the number shown on the current display digit. Keeping the button pressed will reset the values of all digits, and the system's current position will be moved to the rightmost digit, in the first state.
* State 1 (default, but also initiated after a button press in state 2): The X axis of the joystick can be used to cycle through the 4 digits; using the other axis does nothing. A blinking decimal point shows the current digit position. When pressing the joystick's button, the system locks in on the selected digit, and it enters the second state. Long pressing the button in state 1 resets the entire display by changing each display digit's value to the number 0, and moving the current position to the rightmost display digit.
* State 2 (initiated after a button press in state 1): The decimal point of the current digit stops blinking, adopting the ON state. In this state, the X axis can no longer be used to cycle through the 4 digits. Instead, by using the Y axis, the number on the current display digit can be incremented or decremented, being able to show numbers IN HEX (aka from 0 to F). Pressing the button again returns the system to state 2. Moreover, when changing the numbers, they should not be changed continuously, while holding the joystick in one direction, but rather, they should change only for every joystick movement. 
> Note: Long pressing the button to reset should only be available in state 1.

**Components Used:**
* 1 4-digit 7-segment display
* 1 74HC595 shift register
* 1 joystick
* resistors and wires (per logic)

**Setup Picture:** ![poza tema 4 lab robotica](https://user-images.githubusercontent.com/87432371/202314041-417979cb-df6a-4d71-b288-0a64b7b35a24.jpg)

**Task Showcase:** https://www.youtube.com/watch?v=mt6fiHb_Kcg&ab_channel=AlexandruSasu
</details>

<details>
<summary><h3>🎮 Homework #5 : LCD Menu for an Unimplemented Game on an 8x8 Matrix</h3></summary>
<br>

**Task Requirements:** Create a menu on an LCD for a game displayed on a matrix. In order to move through the menu, the user should be able to scroll on the LCD with the help of a joystick. The menu should include the following functionality:
1. When powering up a game, a greeting message should be shown for a few moments.
2. Should contain roughly the following categories:
   - Start game, starts the initial level of the game
   - Highscore:
     - Initially, there are 0 highscores.
     - Updated when the game is done. Highest possible score should be achieved by starting at a higher level.
     - Saves the top 5+ values in EEPROM with name and score.
   - Settings:
     - Enter name. The name should be shown in highscore. Maybe somewhere else, as well?
     - Starting level: Set the starting level value. The idea is to be able to start from a higher level as well. Can be replaced with difficulty.
     - LCD contrast control (optional, it replaces the potentiometer). Save it to EEPROM.
     - LCD brightness control (mandatory, must change LED wire that’s directly connected to 5v). Save it to EEPROM.
     - Matrix brightness control (see function setIntesnity from the ledControl library). Save it to EEPROM.
     - Sounds on or off. Save it to EEPROM.
     - Extra stuff can include items specific to the game mechanics, or other settings such as chosen theme song, etc. Again, save it to EEPROM.
   - About: should include details about the creator(s) of the game. At least game name, author, and GitHub link or user. (use scrolling text?)
   - How to play: short and informative description.
3. While playing the game: display all relevant info
   - Lives
   - Level
   - Score
   - Time?
   - Player name?
   - etc
4. Upon game ending:
   - Screen 1: a message such as "Congratulations on reaching level/score X". "You did better than y people.". etc. Switches to Screen 2 upon interaction (button press) or after a few moments.
   - Screen 2: display relevant game info: score, time, lives left, etc. Must inform the player if they beat the highscore. This menu should only be closed by the player, by pressing a button.
> Note: The menu should be intuitive and obvious how to use.

**Components Used:**
* 1 LCD
* 1 1088AS matrix
* 1 MAX7219 LED Driver
* 1 joystick
* 1 passive buzzer
* 1 potentiometer (for controlling the LCD contrast)
* 1 10μF, 50V electrolytic capacitor
* 1 (104) 0.1μF ceramic capacitor
* resistors and wires (per logic)

**Setup Picture:** ![poza tema 5 lab robotica](https://user-images.githubusercontent.com/87432371/206425397-38b0746b-3fba-4e1e-a923-cbf9a10006fd.jpg)

**Task Showcase:** https://www.youtube.com/watch?v=hDcGyL06Gk4&ab_channel=AlexandruSasu
</details>
