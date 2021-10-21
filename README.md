# Hardare_iGEM_Bettencourt


Here is the repository for all the documentation on the Hardware design of Paris Bettencourt team 

Members of this project: 
- Michael Sedbon
- Daria Fedorova
- Sarah Haggenmüller
- Etienne Lemiere
- Nathália Raquel de Souza Fernandes
- Juliette Bellengier


We wished to create our bioreactor to automatize our biological process and increase the biosafety.


# Working principle

To meet the objective described above, we produced a prototype version of the bioreactors system, testing parts and principles. We present here a working proof of concept with a breakdown of the components used as well as ideas to improve some of the parts and the general working principle of the system.

A complete bill of material can be found here 
The Arduino script can be downloaded here
The TouchDesigner network running the Graphical User Interface can be downloaded here.

The actual version of the working prototype consists of two tanks
- One, growing the bacterial culture 
- The second aimed to separate minicells from mother cells and to start pigment production through inputs of chemical cue (Arabinose)

Each tank is temperature-controlled with separated heating systems and thermometers.
Stirring and aeration of the two tanks are achieved through individually controlled peristaltic pumps.
Transfer from the growth tank to the minicell expression tank is handled by a peristaltic pump.
Filtering of the minicells has not been considered for this proof of concept but some research has been done regarding the filtering mechanism to be used.
Control of the system is handled through a single Arduino Uno. 
High voltage devices (Peristaltic pump and heating mats) are switched through relays.

## Overall design 

### 3D Printed lid 

**For the growth thank**

The optical density (OD) sensor is held on a floating 3D printed tube allowing the sensor to always sit right at the surface of the culture
4 holes were placed on the lid to fit cable glands holding stainless steel tubing.
A 4mm internal diameter (ID) stainless steel tube is maintaining the temperature sensor in place (hence always measuring temperature from the same location).

The 3, 8mm ID stainless steel tubes are carrying stirring through bubbling and liquid handling from one tank to the other.

**For the Pigment production tank**

The design of the lid for the pigment production tank is similar to the one of the growth tank except that optical density is not measured here, hence, the mechanism holding the OD sensor has been removed.

Here, stainless steel tubes were used as they were readily available but Polyvinyl chloride (PVC) tubes might be considered to lower the costs

## Laser cutted Panel

A panel holding all the electronic components (Arduino, peristaltic pumps, relays, power and sensor connectors) was designed and laser cut out of 3mm thick transparent Acrylic. 
The panel was mounted on 20x20mm V slot aluminum extrusion
This setup allows for fast and clean prototyping, scalability while keeping costs minimal.
3mm Acrylic proved to be slightly too fragile and other laser cuttable materials might be considered. Another alternative would be to stiffen the V Slot aluminum profile construction to have as little weight as possible on the acrylic panel.
The DXF file for laser cutting the panel can be found here


## Heating mats

### What and why

Two off the shelf heating pads were purchased from a pet shop (These heating pads were originally designed to be used in terrariums for maintaining the required temperature for reptiles).
These mats were wrapped around both tanks and hooked up to individual channels of the relay.
This setup proved to reach the required temperature (up to 42*c from 21*c ambient room temperature).
Future improvements
Two key learning points were extracted from experimenting with this setup:
Even if the required temperature is ultimately achieved, this process remains slow (up to 3 hours)
As we used relays to control the heating pads, the only available control mechanisms were through thresholding at specific temperatures. This process is hard to tune and does not encompass the inertia of the system (even if turned off, the pad remain hot for some times) and only allow for binary values (on or off)

To approach these problems we propose to replace the heating pads with Peletier Elements (allowing to heat the tank faster). The trade-off for such change is that a special frame holding several Peletier elements as well as heatsink systems will need to be designed, hence increasing cost and rendering the entire setup heavier and harder to build. Such change should, therefore, only be considered for use cases where heating faster is required.

To improve control of the temperature, a Mosfet-based system should be considered to allow for non-binary switching of the heating mat (Using a Mosfet-based system would allow to turn on the heating pad at any arbitrarily defined temperature).
With an analog temperature control system, a proper proportional–integral–derivative (PID) controller could be implemented in the control firmware


## Temperature sensor

### What sensors 

Two (one in each tank) DS18B20 temperature sensors were used.
They were chosen for their temperature range (From -55*c to 125*c ), their price (2€ in a package with stainless steel probe and water proof cabling pre-setup), their availability (available at low cost at most online suppliers and local electronic/hobbyist shop), and their convenience to use (these sensors work on with the One-Wire protocol, and, therefore require less installation).
The sensor and the cable were fit through a 4mm Internal diameter stainless tube to insure stability of the system against shaking, possible transport and to increase overall robustness of the design. 
Optical Density (OD) sensor

### What sensors 

To measure the optical density of the growing bacterial culture (triggering the transfer of fully grown bacterias to the pigment expression tank), the DFRobot Gravity Analog Turbidity Sensor was used.
It has been tested and shows sufficient sensitivity to the changes of turbidity specific to the growth of our bacterial cultures.



## Peristaltic pump

### What pump 

Although the original design was cleverly using gravity as a mean to transfer liquids from one tanks to the other, thus preventing the cost relative to the peristaltic pumps, we decided to use these pumps for the proof of concept version of the bioreactor because the design and implementation of watertight systems required for such setup was judged of secondary importance to the proof of concept.
DIY peristaltic pumps were purchased from Amazon.fr and proved sufficient for liquid handling and stirring through bubbling.

### Future improvements

In the actual setup, the liquid handling and stirring pumps are controlled through individual relays. They are, therefore, unidirectional. Being able to control the direction of the pump would allow for more precise liquid handling and better stirring.
To achieve that, H-Bridge component can be added to the system change the direction of the pumps

## Arduino uno

### What 

Arduino is an open-source electronics platform based on easy-to-use hardware and software that is very popular among DIY communities, as a tool to teach and learn STEAMS as well as in research labs across the globe for its low cost and high flexibility/modularity characteristics. 

We based the control mechanism of our design on an Arduino Uno

## Future improvements (Making a PCB ?)
In its current state, wiring of all the components (The temperature and Optical Density sensors, the relays and the OLED screen displaying real-time data) is done through Jumper cables and breadboards. The design of a custom Printed Circuit board (PCB) in the form of an Arduino shield would greatly help reproduce the setup as much less wiring would be required, thus, preventing human mistakes, saving time and cost in production as well as providing a cleaner workbench.


## Graphical user interface (GUI)

### What (TOUCH_DESIGNER)

We developed a graphical user interface (GUI) that displays the current state of the sensor's measurement, of the relays controlling the pumps and the recorded data.

It offers the possibility to overwrite the automated program and to manually control the liquid handling pump (for example to extract liquid from the growth tank in order to perform some measurement).

The software also comes with a Python Base Data Logger recording the state of the system at a customisable rate on a CSV file.

The GUI also offers the possibility to showcase the recorded data in real time and to load files and instantaneously graph data from previous experiments.

The implementation was based on TouchDesigner, a node based programming environment primarily designed for the interaction design community allowing for extremely fast prototyping through a high level of modularisation.
If a TouchDesigner license has to be purchased for commercial purpose, the GUI has been designed in order to run on the free version of touchdesigner (available both for Windows and MacOS)


## Relays

### What 

We used an 8 channel optocoupler driven 5v Relay array to control the heating mats and the 3 peristaltic pumps. All together, these components takes 5 of the 8 channels leaving some free to hook up other devices (such as other pumps or valves for chemical inputs)

### Future improvements (Using optically driven relay to spare the noise?)
As they get triggered these mechanical relays produce a clicking sound. It is subtle but can be heard and problematic to implement in specific places (such as retail or other sound sensitive environments). As an alternative, Solid State relays might be used.

## OLED Screen

### What 

An SSD 1306 OLED Screen has been mounted on the panel to display the sensor's data in real time. This display has been chosen for its availability (available at low cost in most online suppliers and local electronic/hobbyist shops), its convenience (simple integration with the Adafruit_SSD1306 Library and simple 4 pin wiring) and its price (less than 5€ on amazon).

## Future improvements
Despite its many advantages, this display remains small and with a limited resolution (124x64 px). Reading information might be challenging and another display can be considered.



