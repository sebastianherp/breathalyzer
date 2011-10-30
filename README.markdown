After ordering the MQ-3 gas sensor (alcohol) I had build something with it.

I also had a spare PIC18LF1230, added some LEDs and one lonely self test later this simple breathalyzer was born.

The values used to turn on the respective number of LEDs come from this self test and the following formula:

* BASE (resistance at calibration point 0,4 mg / L or 0,84 promille) ~ 2500
* R1 (resistance over the sensor) = 10000 * (1024 - ADC_value) / ADC_value
* Blood alcohol = pow(R1/BASE, -1.4888319) * 0.414506 * 2.1

[2.1 = factor between blood alcohol and breath alcohol]