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

**Task Requirements:** Use three separate potentiometers in controlling each color of the RGB led (Red, Green and Blue). The control must be done
with digital electronics (aka the value of the potentiometer must be read with Arduino, and a mapped value must be written to each of the pins connected
to the led).

**Components Used:**
* 1 RBG LED
* 3 potentiometers
* resistors and wires (per logic)

**Setup Picture:** ![poza tema 1 lab robotica](https://user-images.githubusercontent.com/87432371/197353566-af59eeda-ca8c-477d-97d2-0b4c5ef624bc.jpg)

**Task Showcase:** https://www.youtube.com/watch?v=oJw1XVFhmSI&ab_channel=AlexSasu
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

**Task Showcase:** https://www.youtube.com/watch?v=hJEe0grXwIo&ab_channel=AlexSasu
</details>
