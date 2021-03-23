ROOT::RVec<int> good_jets(const ROOT::RVec<float> &eta1,
                          const ROOT::RVec<float> &phi1,
                          const ROOT::RVec<float> &pt2,
                          const ROOT::RVec<float> &eta2,
                          const ROOT::RVec<float> &phi2) {
    ROOT::RVec<int> mask(eta1.size(), 1);
    if (eta2.size() == 0) {
        return mask;
    }

    const auto eta3 = eta2[pt2 > 10];
    const auto phi3 = phi2[pt2 > 10];
    if (eta3.size() == 0) {
        return mask;
    }

    const auto c = ROOT::VecOps::Combinations(eta1, eta3);
    for (auto i = 0; i < c[0].size(); i++) {
        const auto i1 = c[0][i];
        const auto i2 = c[1][i];
        const auto dr = ROOT::VecOps::DeltaR(eta1[i1], phi1[i1], eta2[i2], phi2[i2]);
        if (dr < 0.4) mask[i1] = 0;
    }
    return mask;
}


void rdataframe() {
    ROOT::EnableImplicitMT();
    ROOT::RDataFrame df("Events", "root://eospublic.cern.ch//eos/root-eos/benchmark/Run2012B_SingleMu.root");
    auto h = df.Filter("nJet >= 1")
               .Define("goodJet_antiMuon", good_jets, {"Jet_eta", "Jet_phi", "Muon_pt", "Muon_eta", "Muon_phi"})
               .Define("goodJet_antiElectron", good_jets, {"Jet_eta", "Jet_phi", "Electron_pt", "Electron_eta", "Electron_phi"})
               .Define("goodJet", "goodJet_antiMuon || goodJet_antiElectron")
               .Filter("Sum(goodJet) > 0")
               .Define("goodJet_sumPt", "Sum(Jet_pt[goodJet])")
               .Histo1D({"", ";Jet p_{T} sum (GeV);N_{Events}", 100, 15, 200}, "goodJet_sumPt");

    TCanvas c;
    h->Draw();
    c.SaveAs("7_rdataframe.png");
}
