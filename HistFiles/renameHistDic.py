import ROOT

# Open in update mode
f = ROOT.TFile.Open("Hists05Jul2025-ZZ4l2017_Moriond_Systematics_copy.root", "UPDATE")

# Get the old directory
old_dir = f.Get("data_SingleElectron_Run2017B-UL2017_MiniAODv2-v1")
if not old_dir or not isinstance(old_dir, ROOT.TDirectory):
    raise RuntimeError("Directory 'oldName' not found!")

# Create the new directory
new_dir = f.mkdir("data")

# Copy objects from old directory to new directory
for key in old_dir.GetListOfKeys():
    obj = key.ReadObj()
    new_dir.cd()
    obj.Write(key.GetName())

# Remove the old directory from the file's memory
f.Delete("data_SingleElectron_Run2017B-UL2017_MiniAODv2-v1;*")
f.Close()
