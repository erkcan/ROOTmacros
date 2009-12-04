// A function to divide a canvas to a given number of pads in a
//   way that is mostly pleasing to look at.
// (Inspired by Gokhan's similar function.)
unsigned int DivideCanvas(TPad *canvas, unsigned int npads) const {
  unsigned int xdiv=1, ydiv=1;
  for (;;) {
    if (npads<=xdiv*ydiv ) break; xdiv++;
    if (npads<=xdiv*ydiv ) break; ydiv++; }
  canvas->Divide(xdiv,ydiv);
  return ydiv; // return number of rows
}
