import ROOT

# 1. Open input file
f_in = ROOT.TFile.Open("Hists05Jul2025-ZZ4l2017_Moriond_Systematics.root", "READ")
h = f_in.Get("zz4l-powheg/Mass_eeee")

if not h:
    print(" Histogram not found!")
else:
    # 2. Clone and normalize
    h_norm = h.Clone("Mass_eeee_norm")
    h_norm.SetDirectory(0)
    integral = h_norm.Integral()
    if integral != 0:
        h_norm.Scale(1.0 / integral)
    else:
        print(" Histogram has zero integral, not scaling.")

    # 3. Write to new ROOT file
    f_out = ROOT.TFile.Open("normalized.root", "RECREATE")
    h_norm.Write()
    f_out.Close()
    f_in.Close()

    print("Saved normalized histogram as 'Mass_eeee_norm' in normalized.root")

