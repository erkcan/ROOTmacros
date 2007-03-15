// A set of functions to get the ordinal number names and
// suffixes correct.

TString OrdinalAbb(int number)
{
  TString output;
  output+=number;
  return output+"^{"+OrdinalSuffix(number)+"}";
}

TString OrdinalSuffix(int number)
{
  int absn = abs(number);
  if (absn/10%10 != 1) {
    if (absn%10 == 1) return "st";
    if (absn%10 == 2) return "nd";
    if (absn%10 == 3) return "rd"; }
  return "th";
}
