<Qucs Schematic 0.0.16>
<Properties>
  <View=-42,-55,917,959,0.911468,0,28>
  <Grid=10,10,0>
  <DataSet=OpAmp-Magneto.dat>
  <DataDisplay=OpAmp-Magneto.dpl>
  <OpenDisplay=1>
  <Script=OpAmp-Magneto.m>
  <RunScript=0>
  <showFrame=0>
  <FrameText0=\x041D\x0430\x0437\x0432\x0430\x043D\x0438\x0435>
  <FrameText1=\x0427\x0435\x0440\x0442\x0438\x043B:>
  <FrameText2=\x0414\x0430\x0442\x0430:>
  <FrameText3=\x0412\x0435\x0440\x0441\x0438\x044F:>
</Properties>
<Symbol>
</Symbol>
<Components>
  <R R3 1 240 350 -26 15 0 0 "2k" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R4 1 330 350 -26 15 0 0 "2k" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <OpAmp OP1 1 230 250 -26 42 0 0 "1e6" 1 "15 V" 0>
  <OpAmp OP2 1 400 250 -26 42 0 0 "1e6" 1 "15 V" 0>
  <GND * 1 190 200 0 0 0 2>
  <GND * 1 300 160 0 0 0 3>
  <GND * 1 70 470 0 0 0 0>
  <Vdc V1 1 70 200 -58 -26 0 3 "5 V" 1>
  <GND * 1 70 170 0 0 0 2>
  <R R1 1 70 280 15 -26 0 1 "1k" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <.TR TR1 1 570 70 0 61 0 0 "lin" 1 "0" 1 "3 s" 1 "1001" 0 "Trapezoidal" 0 "2" 0 "1 ns" 0 "1e-16" 0 "150" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "26.85" 0 "1e-3" 0 "1e-6" 0 "1" 0 "CroutLU" 0 "no" 0 "yes" 0 "0" 0>
  <R R7 1 70 420 15 -26 0 1 "Rg" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <Eqn Eqn1 1 800 90 -35 17 0 0 "Rg=1k" 1 "yes" 0>
  <.DC DC1 1 570 260 0 38 0 0 "26.85" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "no" 0 "150" 0 "no" 0 "none" 0 "CroutLU" 0>
  <.SW SW1 0 580 340 0 61 0 0 "DC1" 1 "lin" 1 "Rg" 1 "0 Ohm" 1 "10k Ohm" 1 "1001" 1>
  <Vdc V3 1 180 90 18 -26 0 1 "1 V" 1>
  <Vac V2 1 330 160 -26 18 0 0 "89 mV" 1 "1 Hz" 0 "0" 0 "0" 0>
  <R R2 1 140 350 -26 15 0 0 "51k" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R5 1 410 350 -26 15 0 0 "51k" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
</Components>
<Wires>
  <170 350 190 350 "" 0 0 0 "">
  <270 350 280 350 "" 0 0 0 "">
  <190 350 210 350 "" 0 0 0 "">
  <360 350 380 350 "" 0 0 0 "">
  <360 270 370 270 "" 0 0 0 "">
  <360 270 360 350 "" 0 0 0 "">
  <440 350 450 350 "" 0 0 0 "">
  <440 250 450 250 "" 0 0 0 "">
  <450 250 450 350 "Vout" 480 260 39 "">
  <190 270 190 350 "" 0 0 0 "">
  <190 270 200 270 "" 0 0 0 "">
  <270 250 280 250 "" 0 0 0 "">
  <280 350 300 350 "" 0 0 0 "">
  <280 250 280 350 "" 0 0 0 "">
  <360 230 370 230 "" 0 0 0 "">
  <360 160 360 230 "Vinp" 390 140 11 "">
  <190 200 190 230 "" 0 0 0 "">
  <190 230 200 230 "" 0 0 0 "">
  <70 450 70 470 "" 0 0 0 "">
  <70 230 70 250 "" 0 0 0 "">
  <70 310 70 350 "" 0 0 0 "">
  <70 350 70 390 "" 0 0 0 "">
  <70 350 110 350 "Vref" 110 310 15 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
