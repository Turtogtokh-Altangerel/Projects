//CONTROLLER.MICRO830//
This script implements the control of a line model, which consists of input
and output conveyors as well as drilling and milling machine stations. The line of the machine station
section is placed perpendicular to both input and output lines; therefore, pushers are essentially
used to change the direction of an object. Conveyors are equipped with photosensors for position
detection. Limit switches are used to control the extension of pushers.
The description of the line model is that an object is detected on the first conveyor and forwarded to
the first pusher, from where the object is pushed to the machining station. At the end of the
machining, the object is once again pushed to the output line.
There are four function blocks to implement the whole process.
//AUTOMATED STORAGE//
The project controls the system for an automated high-bay warehouse system. Totes to be stored in the bays of the warehouse 
arrive on an input roller conveyor, which operates continuously. The totes, after being identified by their barcodes, are transferred 
to the appropriate level of the warehouse by a lift, where carts equipped with arms place them in the designated bay.
//TRAY SORTING//
The project implements the control software for a lifting table. The table sorts empty and loaded trays in a 
logistic system: out of the trays arriving at the input conveyor, the loaded ones shall be forwarded to the bottom output conveyor, 
while the empty ones shall be forwarded to the top output conveyor. Trays are sorted based on their weight, which is measured by 
a sensor located below the roller conveyor of the lifting table.