<Qucs Schematic 0.0.16>
<Properties>
  <View=0,0,800,800,1,0,0>
  <Grid=10,10,1>
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
  <OpAmp OP1 1 190 130 -26 42 0 0 "1e6" 1 "15 V" 0>
  <GND * 1 30 130 0 0 0 0>
  <Vac V1 1 90 110 -26 18 0 0 "0.9 V" 1 "1 GHz" 0 "0" 0 "0" 0>
  <GND * 1 230 330 0 0 0 0>
  <R R2 1 230 290 15 -26 0 1 "50 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <R R1 1 170 230 -26 15 0 0 "50 Ohm" 1 "26.85" 0 "0.0" 0 "0.0" 0 "26.85" 0 "european" 0>
  <VProbe Pr1 1 320 110 28 -31 0 0>
  <GND * 1 330 150 0 0 0 0>
  <.TR TR1 1 520 70 0 57 0 0 "lin" 1 "0" 1 "1 ms" 1 "11" 0 "Trapezoidal" 0 "2" 0 "1 ns" 0 "1e-16" 0 "150" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "26.85" 0 "1e-3" 0 "1e-6" 0 "1" 0 "CroutLU" 0 "no" 0 "yes" 0 "0" 0>
  <.DC DC1 1 520 250 0 57 0 0 "26.85" 0 "0.001" 0 "1 pA" 0 "1 uV" 0 "no" 0 "150" 0 "no" 0 "none" 0 "CroutLU" 0>
</Components>
<Wires>
  <120 110 160 110 "" 0 0 0 "">
  <30 110 30 130 "" 0 0 0 "">
  <30 110 60 110 "" 0 0 0 "">
  <230 320 230 330 "" 0 0 0 "">
  <230 230 230 260 "" 0 0 0 "">
  <200 230 230 230 "" 0 0 0 "">
  <140 150 160 150 "" 0 0 0 "">
  <140 150 140 230 "" 0 0 0 "">
  <230 130 230 230 "" 0 0 0 "">
  <230 130 310 130 "" 0 0 0 "">
  <330 130 330 150 "" 0 0 0 "">
</Wires>
<Diagrams>
</Diagrams>
<Paintings>
</Paintings>
