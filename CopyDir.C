int CopyDir(TDirectory *sdir, TDirectory *tdir, TString indent="") {

  if ( sdir == 0 || tdir == 0 ) return 0;
  cout << indent << sdir->GetName() << "/" << endl;
  TList *dirlist = sdir->GetListOfKeys();
  for (int i=0; i<dirlist->GetEntries(); i++) {
    TKey *member = (TKey*)dirlist->At(i);
    if ( member->ReadObj()->InheritsFrom("TDirectory") ) {
      CopyDir( (TDirectory*)(member->ReadObj()),
	       tdir->mkdir(member->GetName()), indent+" " ); }
    else {
      tdir->cd();
      member->ReadObj()->Write();
      sdir->cd(); }
    //cout << member->GetName() << endl;
  }
  //sdir->ReadAll();
  //tdir->cd();
  //sdir->GetList()->Write();
  //sdir->cd();
}
