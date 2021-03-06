// Function to copy one TDirectory to another TDirectory with all its
//  subdirectories recursively. An example use:

// TFile *srcfile = TFile::Open("BigfileWithManyDirectoies.root");
// TFile *targetfile = new TFile("NewfileWithContentsOfOneDirOnly.root","new");
// srcfile->cd("/DirectoryToCopy/");
// CopyDir(gDirectory, targetfile->GetDirectory("/"));
// targetfile->Write();

Long64_t CopyDir(TDirectory *sdir, TDirectory *tdir, TString indent="") {

  if ( sdir == 0 || tdir == 0 ) return 0;
  cout << indent << sdir->GetName() << "/" << endl;
  TList *dirlist = sdir->GetListOfKeys();
  Long64_t nbytes = 0;
  for (int i=0; i<dirlist->GetEntries(); i++) {
    TKey *member = (TKey*)dirlist->At(i);
    if ( member->ReadObj()->InheritsFrom("TDirectory") ) {
      nbytes += CopyDir( (TDirectory*)(member->ReadObj()),
			 tdir->mkdir(member->GetName()), indent+" " );
      continue; }
    tdir->cd();
    nbytes += member->ReadObj()->Write();
    sdir->cd();
  }
  return nbytes;
}
