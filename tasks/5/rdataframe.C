ROOT::RVec<float> compute_dimuon_masses(const ROOT::RVec<float> &pt,
                                        const ROOT::RVec<float> &eta,
                                        const ROOT::RVec<float> &phi,
                                        const ROOT::RVec<float> &mass,
                                        const ROOT::RVec<int> &charge) {
    ROOT::RVec<float> dimuon_masses;
    const auto c = ROOT::VecOps::Combinations(pt, 2);
    for (auto i = 0; i < c[0].size(); i++) {
        const auto i1 = c[0][i];
        const auto i2 = c[1][i];
        if (charge[i1] == charge[i2]) continue;
        const ROOT::Math::PtEtaPhiMVector v1(pt[i1], eta[i1], phi[i1], mass[i1]);
        const ROOT::Math::PtEtaPhiMVector v2(pt[i2], eta[i2], phi[i2], mass[i2]);
        dimuon_masses.push_back((v1 + v2).mass());
    }
    return dimuon_masses;
};


void rdataframe() {
    ROOT::EnableImplicitMT();
    ROOT::RDataFrame df("Events", "root://eospublic.cern.ch//eos/root-eos/benchmark/Run2012B_SingleMu.root");
    auto h = df.Filter("nMuon >= 2")
               .Define("Dimuon_mass", compute_dimuon_masses, {"Muon_pt", "Muon_eta", "Muon_phi", "Muon_mass", "Muon_charge"})
               .Filter("Sum(Dimuon_mass > 60 && Dimuon_mass < 100) > 0")
               .Histo1D({"", ";MET (GeV);N_{Events}", 100, 0, 2000}, "MET_pt");

    TCanvas c;
    h->Draw();
    c.SaveAs("5_rdataframe.png");
}
