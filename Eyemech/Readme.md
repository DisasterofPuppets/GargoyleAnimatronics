#Functions:
- Move both eyes with a joystick
- Function for wake up / flutter eyelids etc.
- Left Eye only (eye and blinker)
- Right Eye only (eye and blinker)
- Blink

# Eye Mech

### Hardware required

Power
With 8 MG996R servos @ 2.5 Amps each you need at least 20 Amps @ 5 volts. This may increase if additional servos are added.

#### Eyeballs

- 2 x 10 mm / 0.39 Inch Universal Joints with 3 mm / 0.12 Inch hole
- 2 x 40 mm / 1.57 Inch Clear cabochons with eyeball design
- 8 x 3mm / 0.12 Inch screw inserts
- 4 x MG 996R servos
- 8 x 3mm / 0.12 Inch thread x 10 mm / 0.39 Inch length machine screws

<img src="https://github.com/DisasterofPuppets/GargoyleAnimatronics/blob/main/Partimages/10mmuniversaljoint.jpg" height="100"></img>
<img src="https://github.com/DisasterofPuppets/GargoyleAnimatronics/blob/main/Partimages/Cabochon.jpg" height="100"></img>
<img src="https://github.com/DisasterofPuppets/GargoyleAnimatronics/blob/main/Partimages/3mmbrassscrewinsert.jpg" height="100"></img>
<img src="https://github.com/DisasterofPuppets/GargoyleAnimatronics/blob/main/Partimages/996R.jpg" height="100"></img>
<img src="https://github.com/DisasterofPuppets/GargoyleAnimatronics/blob/main/Partimages/screws.jpg" height="100"></img>


#### Eyelids

- 4 x 3mm / 0.12 Inch screw inserts
- 4 x MG 996R servos
- 4 x 3mm / 0.12 Inch thread x 10 mm / 0.39 Inch length machine screws
- 4 x 8mm / 0.31 Inch base 12mm / 0.48 Inch length (if you can get longer do so) earring base. You don't need the clips but I don't think you can purchase the stems alone.

<img src="https://github.com/DisasterofPuppets/GargoyleAnimatronics/blob/main/Partimages/3mmbrassscrewinsert.jpg" height="100"></img>
<img src="https://github.com/DisasterofPuppets/GargoyleAnimatronics/blob/main/Partimages/996R.jpg" height="100"></img>
<img src="https://github.com/DisasterofPuppets/GargoyleAnimatronics/blob/main/Partimages/screws.jpg" height="100"></img>
<img src="https://github.com/DisasterofPuppets/GargoyleAnimatronics/blob/main/Partimages/earring8mmx12mm.png" height="100"></img>

#### Mounting

- 4 mm / 0.16 Inch PTFE tubing
- 12 x 4mm / 0.16 Inch PTFE connector with 10 mm /0.39 Inch thread 
(This allows us to push the ptfe tube through to act as more of a guide for the wire)
- Braided stainless steel wire 1 mm / 0.39 Inch for eyeballs
- Braided stainless steel wire 1.4 mm / 0.55 Inch for eyelids 
- 12 x 3mm / 0.12 Inch thread x 10 mm / 0.39 Inch length machine screws

<img src="https://github.com/DisasterofPuppets/GargoyleAnimatronics/blob/main/Partimages/4mmptfe.jpg" height="100"></img>
<img src="https://github.com/DisasterofPuppets/GargoyleAnimatronics/blob/main/Partimages/4mmptfeconnector.jpg" height="100"></img>
<img src="https://github.com/DisasterofPuppets/GargoyleAnimatronics/blob/main/Partimages/wire.jpg" height="100"></img>
<img src="https://github.com/DisasterofPuppets/GargoyleAnimatronics/blob/main/Partimages/screws.jpg" height="100"></img>

---

# Code

This includes a blinker control (open / closed) aka Blink function.

Default eyelids closed on wake, clicking joystick button opens the eyes for the first time and they remain open.

Smoothing eyeball movement inspired by XRobots https://github.com/XRobots/ServoSmoothing<br>
https://youtu.be/jsXolwJskKM

## Pending Features
- Full movement test on initialisation
- Wink
---
